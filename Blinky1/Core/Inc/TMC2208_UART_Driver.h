#ifndef INC_TMC2208_UART_DRIVER_H_
#define INC_TMC2208_UART_DRIVER_H_

#include "stm32f0xx_hal.h"
#include "usart.h"

typedef struct{
	uint8_t sync;
	uint8_t node_rw;
	uint8_t address;
	uint8_t data[4];
	uint8_t crc;
} TMC2208_Frame_t;

// Global variables for debugging via live expressions
extern uint32_t tmc_last_write_addr;
extern uint32_t tmc_last_write_value;
extern uint32_t tmc_last_read_addr;
extern uint32_t tmc_last_read_value;
extern uint8_t tmc_read_success;
extern uint8_t tmc_read_failed;

void TMC2208_UART_Init(void);

uint8_t CalcCRC(const uint8_t *data,uint32_t length);

// Write a 32-bit register value
void TMC2208_WriteReg(uint8_t address, uint32_t value);
// Read a register(sends request,returns value)
bool TMC2208_ReadReg(uint8_t address, uint32_t *read_value);

void TMC2208_ConfigureBasic(uint16_t irun_ihold, uint8_t microsteps);

uint8_t TMC2208_TestConnection(void);

#endif /* INC_TMC2208_UART_DRIVER_H_ */
