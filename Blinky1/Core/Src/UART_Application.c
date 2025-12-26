#include "UART_Application.h"

extern UART_HandleTypeDef huart1;

static uint8_t uart_rx_byte = 0;					//Rx byte buffer(1 byte at a time

void UART_AppInit(void)
{
	HAL_UART_Receive_IT(&huart1, &uart_rx_byte, 1);
}

void UART_SendEvent(uint8_t evt)
{
	HAL_UART_Transmit(&huart1, &evt, 1, 10);
}

void UART_OnByteReceived(void)
{
	extern uint8_t current_mode;
	extern int8_t Direction;
	extern int8_t Delay_index;

	switch(uart_rx_byte)
	{
		case EVT_SINGLE_CLICK:
			Delay_index += Direction;
			WrapIndex();
			current_mode = 1;
			break;
		case EVT_DOUBLE_CLICK:
			Direction = -Direction;
			WrapIndex();
			current_mode = 2;
			break;
		case EVT_HOLD_START:
			current_mode = 3;
			break;
		case EVT_HOLD_END:
			current_mode = 0;
			break;
		default:
			break;
	}
	HAL_UART_Receive_IT(&huart1, &uart_rx_byte, 1);
}




