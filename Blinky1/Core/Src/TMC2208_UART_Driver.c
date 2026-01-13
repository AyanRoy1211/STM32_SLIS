#include "TMC2208_UART_Driver.h"

uint8_t txframe[8];
uint8_t rxframe[12]={0};
uint8_t crc = 0;

void TMC2208_UART_Init(void)
{

}

void UART_FlushRx(UART_HandleTypeDef *huart)
{
   volatile uint32_t tmp;
   while (__HAL_UART_GET_FLAG(huart, UART_FLAG_RXNE))
   {
       tmp = huart->Instance->RDR;
       (void)tmp;
   }
}

bool TMC2208_SyncUART(UART_HandleTypeDef *huart)
{
    if (huart == NULL)
        return false;

    uint32_t dummy;
    UART_FlushRx(huart);
    TMC2208_ReadReg(0x00, &dummy);
    HAL_Delay(2);
    UART_FlushRx(huart);

    return true;
}

uint8_t CalcCRC(const uint8_t *datagram, uint32_t len)
{
    crc = 0;
    for (int i = 0; i < len; i++) {
        uint8_t currentByte = datagram[i];
        for (int j = 0; j < 8; j++) {
            if ((crc >> 7) ^ (currentByte & 0x01)) {
                crc = (crc << 1) ^ 0x07;
            } else {
                crc = (crc << 1);
            }
            currentByte = currentByte >> 1;
        }
    }
    return crc;
}

void TMC2208_WriteReg(uint8_t address, uint32_t value)
{

    txframe[0] = 0x05;                      // Sync
    txframe[1] = 0x00;                      // Node 0, write
    txframe[2] = address;                   // Register address
    txframe[3] = (uint8_t)(value >> 24);          // LSB
    txframe[4] = (uint8_t)(value >> 16);
    txframe[5] = (uint8_t)(value >> 8);
    txframe[6] = (uint8_t)(value);    // MSB
    txframe[7] = CalcCRC(txframe, 7);

    HAL_UART_Transmit(&huart2, txframe, 8, 100);

}

bool TMC2208_ReadReg(uint8_t address, uint32_t *read_value)
{

    txframe[0] = 0x05;     // Sync
    txframe[1] = 0x00;     // Slave address 0, read bit
    txframe[2] = address;  // Register address
    txframe[3] = CalcCRC(txframe, 3);

    HAL_UART_AbortReceive_IT(&huart2);
    memset(rxframe,0,sizeof(rxframe));



    HAL_StatusTypeDef rxstatus = HAL_UART_Receive_IT(&huart2, rxframe, 12);
    if(rxstatus != HAL_OK)
    {
    	return false;
    }

    HAL_StatusTypeDef txstatus = HAL_UART_Transmit(&huart2, txframe, 4, 100);
    if (txstatus != HAL_OK) {
        return false;
    }

    __HAL_UART_CLEAR_FLAG(&huart2,UART_CLEAR_OREF);

    // Validate sync byte
    if (rxframe[4] != 0x05) {
        return false;
    }

    // Validate CRC
    uint8_t calc_crc = CalcCRC(&rxframe[4], 7);
    if (calc_crc != rxframe[11]) {
        return false;
    }

    *read_value = (rxframe[7] << 24) | (rxframe[8] << 16) |
                          (rxframe[9] << 8) | rxframe[10];

    return true;
}
