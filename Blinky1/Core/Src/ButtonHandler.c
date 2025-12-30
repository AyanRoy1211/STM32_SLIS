#include "ButtonHandler.h"

extern uint8_t current_mode;

void UpdateButton(void)
{
	ButtonCore_Update();
	ButtonEvent_t ev = GetEvent();
	switch(ev){
	case BUTTON_EVENT_SINGLE_CLICK:
		current_mode = 1;
		UART_SendEvent(EVT_SINGLE_CLICK);
		break;
	case BUTTON_EVENT_DOUBLE_CLICK:
		current_mode = 2;
		UART_SendEvent(EVT_DOUBLE_CLICK);
		break;
	case BUTTON_EVENT_HOLD_START:
		current_mode = 3;
		UART_SendEvent(EVT_HOLD_START);
		break;
	case BUTTON_EVENT_HOLD_END:
		current_mode = 0;
		UART_SendEvent(EVT_HOLD_END);
		break;
	default:
		break;
	}

}
