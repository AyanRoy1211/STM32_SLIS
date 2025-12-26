#ifndef INC_UART_APPLICATION_H_
#define INC_UART_APPLICATION_H_

#include "main.h"
#include "LCDApplication.h"
#include "LEDApplication.h"

//UART Event Codes
#define EVT_SINGLE_CLICK 0x01
#define EVT_DOUBLE_CLICK 0x02
#define EVT_HOLD_START   0x03
#define EVT_HOLD_END     0x04

void UART_AppInit(void);
void UART_SendEvent(uint8_t evt);
void UART_OnByteReceived(void);

#endif /* INC_UART_APPLICATION_H_ */
