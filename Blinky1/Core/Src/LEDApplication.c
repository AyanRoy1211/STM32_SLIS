#include "LEDApplication.h"

uint32_t LED_delays[DELAY_COUNT] = {250, 500, 1000, 2000};
uint32_t active_LED_delays[DELAY_COUNT] = {250, 500, 1000, 2000};

/* LED state */
uint32_t led_timer = 0;
uint8_t toggle_count = 0;
int8_t Delay_index = 0;
int8_t Direction = 1;

extern uint8_t current_mode;

void WrapIndex(void)
{
    if (Delay_index >= DELAY_COUNT)
        Delay_index = 0;
    else if (Delay_index < 0)
        Delay_index = DELAY_COUNT - 1;
}

void UpdateLED(void)
{
    led_timer++;

    switch (current_mode)
    {
        case 3:
            active_LED_delays[Delay_index] = LED_delays[Delay_index] / 2u;
            if (active_LED_delays[Delay_index] == 0u)
                    active_LED_delays[Delay_index] = 1u;
            break;
        case 2:
            Direction = -Direction;
            WrapIndex();
            break;
        case 1:
            Delay_index += Direction;
            WrapIndex();
            break;
        default:
            break;
    }

    if (led_timer >= active_LED_delays[Delay_index])
    {
        led_timer = 0;
        HAL_GPIO_TogglePin(LD3_GPIO_Port, LD3_Pin);

        toggle_count++;

        /* After 2 toggles (one complete blink: ON→OFF), move to next delay */
        if (toggle_count >= 2)
        {
            toggle_count = 0;
            Delay_index += Direction;
            WrapIndex();
        }
        active_LED_delays[Delay_index] = LED_delays[Delay_index];
    }
}


