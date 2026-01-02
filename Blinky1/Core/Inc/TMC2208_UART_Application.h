#ifndef INC_TMC2208_UART_APPLICATION_H_
#define INC_TMC2208_UART_APPLICATION_H_

#include "main.h"
#include "TMC2208_UART_Driver.h"

void TMC2208_ApplicationInit(void);

void TMC2208_RunTest(void);

void TMC2208_ConfigureMotor(float irun_amps, float ihold_amps, uint8_t microsteps);

#endif /* INC_TMC2208_UART_APPLICATION_H_ */
