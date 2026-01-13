#include "UART_Application.h"

extern UART_HandleTypeDef huart1;

UARTApplicationTypedefStruct UartData;

static uint8_t UartRxByte = 0;

void UART_Receive() {
	__HAL_UART_CLEAR_FLAG(&huart1, UART_CLEAR_FEF | UART_CLEAR_NEF | UART_CLEAR_OREF);
	HAL_UART_Receive_IT(&huart1, &UartRxByte, 1);
}

void UART_AppInit(void) {
	UART_Receive();
}

void UART_SendEvent(uint8_t event) {
	HAL_UART_Transmit(&huart1, &event, 1, 10);
}

void UART_OnByteReceived(void) {
	UartData.PreviousEvent = UartData.CurrentEvent;
	UartData.CurrentEvent = UartRxByte;
	UartData.NewEventReceived = true;

	UART_Receive();
}
