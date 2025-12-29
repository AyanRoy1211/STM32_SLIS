#include "ButtonHandler.h"

#define HOLD_TIME_MS 175
#define CLICK_WINDOW_MS 150
#define DEBOUNCE_TIME_MS 10

extern uint8_t current_mode;

uint8_t btn_stable = 0;
uint8_t btn_prev = 0;
int32_t btn_debounce = 0;
uint32_t btn_press_start = 0;    // ← FIXED: timestamp, not counter
uint32_t btn_release_start = 0;  // ← FIXED: timestamp, not counter
uint8_t btn_clicks = 0;
uint8_t btn_hold = 0;
uint32_t btn_click_timer = 0;

void UpdateButton(void) {
    uint8_t btn_raw = (HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin) == GPIO_PIN_SET) ? 1 : 0;
    uint32_t now = HAL_GetTick();  // ← ADD: real timestamp

    // Debounce (unchanged)
    if (btn_raw != btn_stable)
    {
        btn_debounce++;
        if (btn_debounce >= DEBOUNCE_TIME_MS)
        {
            btn_stable = btn_raw;
            btn_debounce = 0;
        }
    } else {
        btn_debounce = 0;
    }

    // Press edge (FIXED)
    if (btn_stable && !btn_prev) {
        btn_press_start = now;
        if (!btn_hold) {  // Only count clicks if not holding
            btn_clicks++;
            if (btn_clicks > 2)
            	btn_clicks = 2;
        }
        btn_release_start = 0;
    }

    // Hold detect (FIXED)
    if (btn_stable && !btn_hold && (now - btn_press_start >= HOLD_TIME_MS)) {
        btn_hold = 1;
        btn_clicks = 0;
        current_mode = 3;
        UART_SendEvent(EVT_HOLD_START);
    }

    // Release edge (FIXED)
    if (!btn_stable && btn_prev) {
        if (btn_hold) {
            current_mode = 0;
            UART_SendEvent(EVT_HOLD_END);
            btn_hold = 0;
        } else {
            btn_release_start = now;  // ← Capture timestamp
        }
        btn_press_start = 0;
    }

    // Click resolve (FIXED)
    if (!btn_hold && !btn_stable && (btn_clicks > 0))
    {
        btn_click_timer++;
        if (btn_click_timer >= CLICK_WINDOW_MS)
        {
            if (btn_clicks == 1)
            {
                current_mode = 1;
                UART_SendEvent(EVT_SINGLE_CLICK);
            } else if(btn_clicks == 2)
            {
                current_mode = 2;
                UART_SendEvent(EVT_DOUBLE_CLICK);
            }
            // CRITICAL: Reset state machine
            btn_clicks = 0;
            btn_click_timer = 0;
        }
    }

    btn_prev = btn_stable;
}
