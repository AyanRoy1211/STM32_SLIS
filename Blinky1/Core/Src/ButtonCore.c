#include "ButtonCore.h"

uint8_t btn_stable = 0;
uint8_t btn_prev = 0;
int32_t btn_debounce = 0;
uint32_t btn_press_start = 0;
uint32_t btn_release_start = 0;
uint8_t btn_clicks = 0;
uint8_t btn_hold = 0;
uint32_t btn_click_timer = 0;
static BUTTON_EVENT_TYPEDEF_ENUM pending_event = BUTTON_EVENT_NONE;

static void Button_Debounce(uint8_t raw,uint32_t now)
{
	if(raw != btn_stable)
	{
		btn_debounce++;
		if(btn_debounce >= DEBOUNCE_TIME_MS)
		{
			btn_stable = raw;
			btn_debounce = 0;
		}
	}
	else
	{
		btn_debounce = 0;
	}
}

void ButtonCore_Update(void)
{
    uint8_t btn_raw = (HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin) == GPIO_PIN_SET) ? 1 : 0;
    uint32_t now = HAL_GetTick();

    Button_Debounce(btn_raw, now);

    if (btn_stable && !btn_prev) {
        btn_press_start = now;
        if (!btn_hold)
        {
            btn_clicks++;
            if (btn_clicks > 2)
            	btn_clicks = 2;
        }
        btn_release_start = 0;
    }

    if (btn_stable && !btn_hold && (now - btn_press_start >= HOLD_TIME_MS)) {
        btn_hold = 1;
        btn_clicks = 0;
        pending_event = BUTTON_EVENT_HOLD_START;
    }

    if (!btn_stable && btn_prev) {
        if (btn_hold)
        {
            pending_event = BUTTON_EVENT_HOLD_END;
            btn_hold = 0;
        }
        else
        {
            btn_release_start = now;
        }
        btn_press_start = 0;
    }

    if (!btn_hold && !btn_stable && (btn_clicks > 0))
    {
        btn_click_timer++;
        if (btn_click_timer >= CLICK_WINDOW_MS)
        {
            if (btn_clicks == 1)
            {
                pending_event = BUTTON_EVENT_SINGLE_CLICK;
            } else if(btn_clicks == 2)
            {
                pending_event = BUTTON_EVENT_DOUBLE_CLICK;
            }

            btn_clicks = 0;
            btn_click_timer = 0;
        }
    }

    btn_prev = btn_stable;
}

BUTTON_EVENT_TYPEDEF_ENUM GetEvent(void)
{
	BUTTON_EVENT_TYPEDEF_ENUM ev = pending_event;
	pending_event = BUTTON_EVENT_NONE;
	return ev;
}
