#include "TMC2208_UART_Driver.h"
#include <string.h>

// Global variables for debugging
uint32_t tmc_last_write_addr = 0;
uint32_t tmc_last_write_value = 0;
uint32_t tmc_last_read_addr = 0;
uint32_t tmc_last_read_value = 0xDEADBEEF;
uint8_t tmc_read_success = 0;
uint8_t tmc_read_failed = 0;

// CRC-8 lookup table (CRC-ATM: poly=0x07, init=0x00, xor_out=0x00)
static const uint8_t crc8_table[256] = {
    0x00, 0x07, 0x0E, 0x09, 0x1C, 0x1B, 0x12, 0x15,
    0x38, 0x3F, 0x36, 0x31, 0x24, 0x23, 0x2A, 0x2D,
    0x70, 0x77, 0x7E, 0x79, 0x6C, 0x6B, 0x62, 0x65,
    0x48, 0x4F, 0x46, 0x41, 0x54, 0x53, 0x5A, 0x5D,
    0xE0, 0xE7, 0xEE, 0xE9, 0xFC, 0xFB, 0xF2, 0xF5,
    0xD8, 0xDF, 0xD6, 0xD1, 0xC4, 0xC3, 0xCA, 0xCD,
    0x90, 0x97, 0x9E, 0x99, 0x8C, 0x8B, 0x82, 0x85,
    0xA8, 0xAF, 0xA6, 0xA1, 0xB4, 0xB3, 0xBA, 0xBD,
    0xC7, 0xC0, 0xC9, 0xCE, 0xDB, 0xDC, 0xD5, 0xD2,
    0xFF, 0xF8, 0xF1, 0xF6, 0xE3, 0xE4, 0xED, 0xEA,
    0xB7, 0xB0, 0xB9, 0xBE, 0xAB, 0xAC, 0xA5, 0xA2,
    0x8F, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9D, 0x9A,
    0x27, 0x20, 0x29, 0x2E, 0x3B, 0x3C, 0x35, 0x32,
    0x1F, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0D, 0x0A,
    0x57, 0x50, 0x59, 0x5E, 0x4B, 0x4C, 0x45, 0x42,
    0x6F, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7D, 0x7A,
    0x89, 0x8E, 0x87, 0x80, 0x95, 0x92, 0x9B, 0x9C,
    0xB1, 0xB6, 0xBF, 0xB8, 0xAD, 0xAA, 0xA3, 0xA4,
    0xF9, 0xFE, 0xF7, 0xF0, 0xE5, 0xE2, 0xEB, 0xEC,
    0xC1, 0xC6, 0xCF, 0xC8, 0xDD, 0xDA, 0xD3, 0xD4,
    0x69, 0x6E, 0x67, 0x60, 0x75, 0x72, 0x7B, 0x7C,
    0x51, 0x56, 0x5F, 0x58, 0x4D, 0x4A, 0x43, 0x44,
    0x1D, 0x1A, 0x13, 0x14, 0x01, 0x06, 0x0F, 0x08,
    0x25, 0x22, 0x2B, 0x2C, 0x39, 0x3E, 0x37, 0x30,
    0x58, 0x5F, 0x56, 0x51, 0x44, 0x43, 0x4A, 0x4D,
    0x60, 0x67, 0x6E, 0x69, 0x7C, 0x7B, 0x72, 0x75,
    0x28, 0x2F, 0x26, 0x21, 0x34, 0x33, 0x3A, 0x3D,
    0x10, 0x17, 0x1E, 0x19, 0x0C, 0x0B, 0x02, 0x05,
    0xB8, 0xBF, 0xB6, 0xB1, 0xA4, 0xA3, 0xAA, 0xAD,
    0x80, 0x87, 0x8E, 0x89, 0x9C, 0x9B, 0x92, 0x95,
    0xC8, 0xCF, 0xC6, 0xC1, 0xD4, 0xD3, 0xDA, 0xDD,
    0xF0, 0xF7, 0xFE, 0xF9, 0xEC, 0xEB, 0xE2, 0xE5
};

void TMC2208_UART_Init(void)
{

}

uint8_t CalcCRC(const uint8_t *data, uint32_t length)
{
    uint8_t crc = 0;
    for (uint32_t i = 0; i < length; i++) {
        crc = crc8_table[crc ^ data[i]];
    }
    return crc;
}

HAL_StatusTypeDef TMC2208_WriteReg(uint8_t address, uint32_t value)
{
    uint8_t frame[8];

    frame[0] = 0x05;                  // Sync nibble
    frame[1] = 0x00;                  // Node 0, write (R/W = 0)
    frame[2] = address;               // Register address
    frame[3] = (uint8_t)(value);      // Data byte 0 (LSB)
    frame[4] = (uint8_t)(value >> 8); // Data byte 1
    frame[5] = (uint8_t)(value >> 16); // Data byte 2
    frame[6] = (uint8_t)(value >> 24); // Data byte 3 (MSB)

    // Calculate CRC over bytes 0-6
    frame[7] = CalcCRC(frame, 7);

    // Track the write
        tmc_last_write_addr = address;
        tmc_last_write_value = value;

    // Send via USART2 (half-duplex, TX only)
    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, frame, 8, 100);
    // Small delay for driver to process
    HAL_Delay(2);

    return status;
}

HAL_StatusTypeDef TMC2208_ReadReg(uint8_t address)
{
    uint8_t tx_frame[8];
    uint8_t rx_frame[8];
    uint8_t frame[8];

    frame[0] = 0x05;           // Sync
    frame[1] = 0x80;           // Node 0, read (R/W = 1)
    frame[2] = address;        // Register address
    frame[3] = 0x00;
    frame[4] = 0x00;
    frame[5] = 0x00;
    frame[6] = 0x00;

    frame[7] = CalcCRC(frame, 7);

    // Send read request
    HAL_StatusTypeDef tx_status = HAL_UART_Transmit(&huart2, tx_frame, 8, 100);
    if (tx_status != HAL_OK)
    {
        tmc_read_failed++;
        tmc_last_read_value = 0xFAFA0001;
        return tx_status;
    }

//    HAL_Delay(10);

    // Receive response
    HAL_StatusTypeDef rx_status = HAL_UART_Receive(&huart2, rx_frame, 8, 100);
    if (rx_status != HAL_OK)
    {
        tmc_read_failed++;
        tmc_last_read_value = 0xDEADC0DE;
        return rx_status;
    }

    // Validate sync byte
    if (rx_frame[0] != 0x05)
    {
        tmc_read_failed++;
        tmc_last_read_value = 0xBADC0FFE;
        return HAL_ERROR;
    }

    // Validate CRC
    uint8_t calc_crc = CalcCRC(rx_frame, 7);
    if (calc_crc != rx_frame[7])
    {
        tmc_read_failed++;
        tmc_last_read_value = 0xC0DEDBAD;
        return HAL_ERROR;
    }

    // Extract and track the register value
    tmc_last_read_addr = address;
    tmc_last_read_value = (rx_frame[6] << 24) | (rx_frame[5] << 16) |
                          (rx_frame[4] << 8) | rx_frame[3];
    tmc_read_success++;

    return HAL_OK;
}

void TMC2208_ConfigureBasic(uint16_t irun_ihold, uint8_t microsteps)
{
    // Register addresses (from TMC2208 datasheet)
    #define GCONF_ADDR      0x00
    #define IHOLD_IRUN_ADDR 0x10
    #define CHOPCONF_ADDR   0x6C
    #define PWMCONF_ADDR    0x70

    // 1. Set GCONF: internalRsense=1, pdndisable=1
    uint32_t gconf = (1 << 1) | (1 << 6);  // 0x42
    TMC2208_WriteReg(GCONF_ADDR, gconf);

    // 2. Set IHOLD_IRUN
    TMC2208_WriteReg(IHOLD_IRUN_ADDR, irun_ihold);

    // 3. Set CHOPCONF: intpol=1 at bit 4
    uint32_t chopconf = (1 << 4);
    TMC2208_WriteReg(CHOPCONF_ADDR, chopconf);
}

uint8_t TMC2208_TestConnection(void)
{
    uint8_t tx_frame[8];
    uint8_t rx_frame[8];

    // Build a read request for GCONF (register 0x00)
    tx_frame[0] = 0x05;           // Sync
    tx_frame[1] = 0x80;           // Read request (R/W = 1)
    tx_frame[2] = 0x00;           // GCONF address
    tx_frame[3] = 0x00;
    tx_frame[4] = 0x00;
    tx_frame[5] = 0x00;
    tx_frame[6] = 0x00;
    tx_frame[7] = CalcCRC(tx_frame, 7);

    // Send test frame
    HAL_StatusTypeDef tx_status = HAL_UART_Transmit(&huart2, tx_frame, 8, 100);
    if (tx_status != HAL_OK)
    {
    	tmc_read_failed++;
        return 0;  // TX failed
    }

    // Wait for driver to respond
    HAL_Delay(10);

    // Receive response
    HAL_StatusTypeDef rx_status = HAL_UART_Receive(&huart2, rx_frame, 8, 100);
    if (rx_status != HAL_OK)
    {
    	tmc_read_failed++;
        return 0;  // RX failed
    }

    // Validate response
    // Check sync byte
    if (rx_frame[0] != 0x05)
    {
    	tmc_read_failed++;
        return 0;  // Invalid sync
    }

    // Verify CRC
    uint8_t calculated_crc = CalcCRC(rx_frame, 7);
    if (calculated_crc != rx_frame[7])
    {
    	tmc_read_failed++;
        return 0;  // CRC mismatch
    }

    tmc_read_success++;
    tmc_last_read_value = (rx_frame[6] << 24) | (rx_frame[5] << 16) |
                          (rx_frame[4] << 8) | rx_frame[3];
    // If we got here, communication successful
    return 1;
}
