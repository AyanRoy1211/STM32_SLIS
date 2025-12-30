#ifndef INC_UART_APPLICATION_H_
#define INC_UART_APPLICATION_H_

#include "main.h"

typedef struct {
	EVENT_CODES_ENUM PreviousEvent;
	EVENT_CODES_ENUM CurrentEvent;

	bool NewEventReceived;
} UARTApplicationTypedefStruct;

void UART_AppInit(void);
void UART_SendEvent(uint8_t evt);
void UART_OnByteReceived(void);

#endif /* INC_UART_APPLICATION_H_ */
