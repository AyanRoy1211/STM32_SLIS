#include "ButtonCore.h"

static BUTTON_EVENT_TYPEDEF_ENUM pending_event = BUTTON_EVENT_NONE;
static ButtonStates_t button = { 0, 0, 0, 0, 0, 0, 0, 0 };

static void Button_Debounce(uint8_t raw, uint32_t now) {
	if (raw != button.btn_stable) {
		button.btn_debounce++;
		if (button.btn_debounce >= DEBOUNCE_TIME_MS) {
			button.btn_stable = raw;
			button.btn_debounce = 0;
		}
	} else {
		button.btn_debounce = 0;
	}
}

void ButtonCore_Update(void) {
	uint8_t btn_raw =
			(HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin) == GPIO_PIN_SET) ? 1 : 0;
	uint32_t now = HAL_GetTick();

	Button_Debounce(btn_raw, now);

	if (button.btn_stable && !button.btn_prev) {
		button.btn_press_start = now;
		if (!button.btn_hold) {
			button.btn_clicks++;
			if (button.btn_clicks > 2)
				button.btn_clicks = 2;
		}
		button.btn_release_start = 0;
	}

	if (button.btn_stable && !button.btn_hold
			&& (now - button.btn_press_start >= HOLD_TIME_MS)) {
		button.btn_hold = 1;
		button.btn_clicks = 0;
		pending_event = BUTTON_EVENT_HOLD_START;
	}

	if (!button.btn_stable && button.btn_prev) {
		if (button.btn_hold) {
			pending_event = BUTTON_EVENT_HOLD_END;
			button.btn_hold = 0;
		} else {
			button.btn_release_start = now;
		}
		button.btn_press_start = 0;
	}

	if (!button.btn_hold && !button.btn_stable && (button.btn_clicks > 0)) {
		button.btn_click_timer++;
		if (button.btn_click_timer >= CLICK_WINDOW_MS) {
			if (button.btn_clicks == 1) {
				pending_event = BUTTON_EVENT_SINGLE_CLICK;
			} else if (button.btn_clicks == 2) {
				pending_event = BUTTON_EVENT_DOUBLE_CLICK;
			}

			button.btn_clicks = 0;
			button.btn_click_timer = 0;
		}
	}

	button.btn_prev = button.btn_stable;
}

BUTTON_EVENT_TYPEDEF_ENUM GetEvent(void) {
	BUTTON_EVENT_TYPEDEF_ENUM ev = pending_event;
	pending_event = BUTTON_EVENT_NONE;
	return ev;
}
