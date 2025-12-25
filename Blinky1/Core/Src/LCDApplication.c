#include "LCDApplication.h"

uint8_t prev_mode = 255;
uint32_t prev_delay = 0;
uint8_t current_mode = 0;

extern uint32_t base_delays[DELAY_COUNT];
extern uint32_t current_delays[DELAY_COUNT];

/* LED state */
extern uint32_t led_timer;
extern uint8_t toggle_count;
extern int8_t delay_idx;
extern int8_t direction;


void UpdateDisplay(void)
{
    //static uint32_t last_mode_change = 0;

    /* Update mode display */
    if (current_mode != prev_mode)
    {
        LCD_SetCursor(0,0);
        switch (current_mode)
        {
            case 3:
                LCD_Print("Mode: HOLD      ");
                break;
            case 2:
                LCD_Print("Mode: DOUBLE    ");
                //last_mode_change = HAL_GetTick();
                break;
            case 1:
                LCD_Print("Mode: SINGLE    ");
                //last_mode_change = HAL_GetTick();
                break;
            default:
                LCD_Print("Mode: IDLE      ");
                break;
        }
        prev_mode = current_mode;
    }

    /* Auto-reset mode to IDLE after 1 second */
//    if ((current_mode == 1 || current_mode == 2) &&
//        (HAL_GetTick() - last_mode_change >= 1000))
//    {
//        current_mode = 0;
//    }

    /* Update delay display */
    if (current_delays[delay_idx] != prev_delay)
    {
        char buffer[20];
        LCD_SetCursor(1, 0);
        snprintf(buffer, sizeof(buffer), "Delay: %lu ms   ", current_delays[delay_idx]);
        LCD_Print(buffer);
        prev_delay = current_delays[delay_idx];
    }
}
