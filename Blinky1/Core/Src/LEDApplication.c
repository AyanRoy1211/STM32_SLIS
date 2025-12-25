#include "LEDApplication.h"

uint32_t base_delays[DELAY_COUNT] = {250, 500, 1000, 2000};
uint32_t current_delays[DELAY_COUNT] = {250, 500, 1000, 2000};

/* LED state */
uint32_t led_timer = 0;
uint8_t toggle_count = 0;
int8_t delay_idx = 0;
int8_t direction = 1;

void WrapIndex(void)
{
    if (delay_idx >= DELAY_COUNT)
        delay_idx = 0;
    else if (delay_idx < 0)
        delay_idx = DELAY_COUNT - 1;
}

/* LED update logic (called every ms) */
void UpdateLED(void)
{
    led_timer++;

    if (led_timer >= current_delays[delay_idx])
    {
        led_timer = 0;
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

        toggle_count++;

        /* After 2 toggles (one complete blink: ON→OFF), move to next delay */
        if (toggle_count >= 2)
        {
            toggle_count = 0;
            delay_idx += direction;
            WrapIndex();
        }
    }
}


