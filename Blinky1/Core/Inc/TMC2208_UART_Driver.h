#ifndef INC_TMC2208_UART_DRIVER_H_
#define INC_TMC2208_UART_DRIVER_H_

#include "stm32f0xx_hal.h"
typedef struct{
	uint8_t sync;
	uint8_t node_rw;
	uint8_t address;
	uint8_t data[4];
	uint8_t crc;
} TMC2208_Frame_t;

void TMC2208_UART_Init(void);

//bool TMC2208_SyncUART(UART_HandleTypeDef *huart);
uint8_t CalcCRC(const uint8_t *data,uint32_t length);

void TMC2208_WriteReg(uint8_t address, uint32_t value);
bool TMC2208_ReadReg(uint8_t address, uint32_t *read_value);

void TMC2208_ConfigureBasic(uint16_t irun_ihold, uint8_t microsteps);

#endif /* INC_TMC2208_UART_DRIVER_H_ */
