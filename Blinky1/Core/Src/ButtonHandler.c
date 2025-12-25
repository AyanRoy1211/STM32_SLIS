#include "ButtonHandler.h"

#define HOLD_TIME_MS 200
#define CLICK_WINDOW_MS  200
#define DEBOUNCE_TIME_MS 30

extern uint8_t current_mode;

/* Button state */
uint8_t btn_stable = 0;
uint8_t btn_prev = 0;
int32_t btn_debounce = 0;
uint32_t btn_press_time = 0;
uint32_t btn_click_timer = 0;
uint8_t btn_clicks = 0;
uint8_t btn_hold = 0;

void UpdateButton(void)
{
    /* Button with PULLDOWN: pressed = HIGH (1), released = LOW (0) */
    uint8_t btn_raw = (HAL_GPIO_ReadPin(Button_GPIO_Port, Button_Pin) == GPIO_PIN_SET) ? 1 : 0;

    /* --- Debounce --- */
    if (btn_raw != btn_stable)
    {
        btn_debounce++;
        if (btn_debounce >= DEBOUNCE_TIME_MS)
        {
            btn_stable = btn_raw;
            btn_debounce = 0;
        }
    }
    else
    {
        btn_debounce = 0;
    }

    /* --- Rising edge (press detected) --- */
    if (btn_stable && !btn_prev)
    {
        btn_press_time = 0;

        if (btn_clicks < 2)
            btn_clicks++;

        btn_click_timer = 0;
    }

    /* --- While pressed --- */
    if (btn_stable)
    {
        btn_press_time++;

        /* Hold detection */
        if (!btn_hold && (btn_press_time >= HOLD_TIME_MS))
        {
            btn_hold = 1;
            btn_clicks = 0;
            btn_click_timer = 0;
            current_mode = 3;  /* Set HOLD mode for display */
        }
    }

    /* --- Falling edge (release detected) --- */
    if (!btn_stable && btn_prev)
    {
        btn_press_time = 0;

        if (btn_hold)
        {
            current_mode = 0;  /* Back to IDLE */
        }

        btn_hold = 0;
        btn_click_timer = 0;
    }

    /* --- Click resolution (button released, not holding) --- */
    if (!btn_hold && !btn_stable && (btn_clicks > 0))
    {
        btn_click_timer++;

        if (btn_click_timer >= CLICK_WINDOW_MS)
        {
            if (btn_clicks == 1)
            {
                current_mode = 1;  /* SINGLE mode for display */
            }
            else
            {
                current_mode = 2;  /* DOUBLE mode for display */
            }

            btn_clicks = 0;
            btn_click_timer = 0;
        }
    }

    btn_prev = btn_stable;
}
