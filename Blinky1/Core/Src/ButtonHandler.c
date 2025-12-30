#include "ButtonHandler.h"
#include "ButtonCore.h"

ButtonApplicationTypedefStruct ButtonData;

void UpdateButton(void)
{
	ButtonCore_Update();
	BUTTON_EVENT_TYPEDEF_ENUM ev = GetEvent();

	if (ev == BUTTON_EVENT_NONE) {
		return;
	}

	ButtonData.PreviousEvent = ButtonData.CurrentEvent;
	ButtonData.NewEventReceived = true;

	switch(ev){
	case BUTTON_EVENT_SINGLE_CLICK:
		ButtonData.CurrentEvent = EVT_SINGLE_CLICK;
		break;
	case BUTTON_EVENT_DOUBLE_CLICK:
		ButtonData.CurrentEvent = EVT_DOUBLE_CLICK;
		break;
	case BUTTON_EVENT_HOLD_START:
		ButtonData.CurrentEvent = EVT_HOLD_START;
		break;
	case BUTTON_EVENT_HOLD_END:
		ButtonData.CurrentEvent = EVT_HOLD_END;
		break;
	case BUTTON_EVENT_NONE:
		break;
	}

}
