#include "LCDApplication.h"

uint8_t prev_mode = 255;
uint8_t current_mode = 0;

extern uint8_t local_mode;
extern uint8_t remote_mode;

void UpdateDisplay(void)
{
    static uint32_t last_mode_change = 0;
    static uint8_t display_mode = 0;  // Separate variable for display

    // Update display when mode changes
    if (current_mode != prev_mode)
    {
        LCD_SetCursor(0, 0);
        switch (current_mode)
        {
            case 3:
                LCD_Print("Mode: HOLD      ");
                display_mode = 3;
                break;
            case 2:
                LCD_Print("Mode: DOUBLE    ");
                display_mode = 2;
                last_mode_change = HAL_GetTick();
                break;
            case 1:
                LCD_Print("Mode: SINGLE    ");
                display_mode = 1;
                last_mode_change = HAL_GetTick();
                break;
            case 0:
                // Only update to IDLE if not in a temporary display state
                if (display_mode == 0 || display_mode == 3)
                {
                    LCD_Print("Mode: IDLE      ");
                    display_mode = 0;
                }
                break;
            default:
                LCD_Print("Mode: IDLE      ");
                display_mode = 0;
                break;
        }
        prev_mode = current_mode;
    }

    // Auto-clear temporary mode display after 2 seconds
    if ((display_mode == 1 || display_mode == 2) &&
        (HAL_GetTick() - last_mode_change >= 2000))
    {
        LCD_SetCursor(0, 0);
        LCD_Print("Mode: IDLE      ");
        display_mode = 0;
    }

    // Update LED status on second line
    char buffer[20];
    LCD_SetCursor(1, 0);

    if (local_mode == 0 && remote_mode == 0)
    {
        snprintf(buffer, sizeof(buffer), "LED: OFF        ");
    }
    else if (local_mode == 3 || remote_mode == 3)
    {
        snprintf(buffer, sizeof(buffer), "LED: ON (HOLD)  ");
    }
    else if (local_mode == 1 || remote_mode == 1)
    {
        snprintf(buffer, sizeof(buffer), "LED: FAST BLINK ");
    }
    else if (local_mode == 2 || remote_mode == 2)
    {
        snprintf(buffer, sizeof(buffer), "LED: SLOW BLINK ");
    }

    LCD_Print(buffer);
}
