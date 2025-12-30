#include "LEDApplication.h"
#include "LED_Controller.h"

LED_Controller_t local_ctrl =
{ LED_MODE_IDLE, 0, 0 };  // LD3
LED_Controller_t remote_ctrl =
{ LED_MODE_IDLE, 0, 0 };  // LD4

void LED_Init()
{
	HAL_GPIO_WritePin(LD3_GPIO_Port, LD3_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(LD4_GPIO_Port, LD4_Pin, GPIO_PIN_RESET);
}

void UpdateLED(void)
{
	LED_Controller_Update(&local_ctrl, LD3_GPIO_Port, LD3_Pin);
	LED_Controller_Update(&remote_ctrl, LD4_GPIO_Port, LD4_Pin);
}

void SetRemoteMode(EVENT_CODES_ENUM event)
{
	switch (event)
	{
	case EVT_SINGLE_CLICK:
		remote_ctrl.mode = LED_MODE_FAST_BLINK;
		break;
	case EVT_DOUBLE_CLICK:
		remote_ctrl.mode = LED_MODE_SLOW_BLINK;
		break;
	case EVT_HOLD_START:
		remote_ctrl.mode = LED_MODE_HOLD_ON;
		break;
	case EVT_HOLD_END:
		remote_ctrl.mode = LED_MODE_IDLE;
		break;
	}
}

void SetLocalMode(EVENT_CODES_ENUM event)
{
	switch (event)
	{
	case EVT_SINGLE_CLICK:
		local_ctrl.mode = LED_MODE_FAST_BLINK;
		break;
	case EVT_DOUBLE_CLICK:
		local_ctrl.mode = LED_MODE_SLOW_BLINK;
		break;
	case EVT_HOLD_START:
		local_ctrl.mode = LED_MODE_HOLD_ON;
		break;
	case EVT_HOLD_END:
		local_ctrl.mode = LED_MODE_IDLE;
		break;
	}
}
