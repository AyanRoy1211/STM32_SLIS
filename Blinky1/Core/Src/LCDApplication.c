#include "LCDApplication.h"
#include "LCDCore.h"
#include <stdio.h>
#define TEMP_DISPLAY_DURATION_MS 2000

static DisplayMode_t current_mode = DISPLAY_MODE_IDLE;
static DisplayMode_t prev_mode = 255;
static DisplayMode_t display_mode = DISPLAY_MODE_IDLE;
static uint32_t last_mode_change = 0;

static LEDStatus_t local_status = LED_STATUS_OFF;
static LEDStatus_t remote_status = LED_STATUS_OFF;
static uint8_t led_status_changed = 0;

static void UpdateModeDisplay(void);
static void UpdateLEDDisplay(void);
static const char* GetLEDStatusString(LEDStatus_t local, LEDStatus_t remote);

void LCDApplication_Init(void)
{
    LCD_Init();

    LCD_Driver_SetCursor(0, 0);
    LCD_Driver_Print("Mode: IDLE      ");
    LCD_Driver_SetCursor(1, 0);
    LCD_Driver_Print("LED: OFF        ");

    current_mode = DISPLAY_MODE_IDLE;
    prev_mode = DISPLAY_MODE_IDLE;
    display_mode = DISPLAY_MODE_IDLE;
    last_mode_change = 0;
    led_status_changed = 1;
}

void LCDApplication_UpdateMode(DisplayMode_t mode)
{
    current_mode = mode;
}

void LCDApplication_UpdateLEDStatus(LEDStatus_t local, LEDStatus_t remote)
{
    if (local_status != local || remote_status != remote)
    {
        local_status = local;
        remote_status = remote;
        led_status_changed = 1;
    }
}

void LCDApplication_Process(void)
{
    UpdateModeDisplay();

    if (led_status_changed)
    {
        UpdateLEDDisplay();
        led_status_changed = 0;
    }
}

static void UpdateModeDisplay(void)
{

    if (current_mode != prev_mode)
    {
        LCD_Driver_SetCursor(0, 0);

        switch (current_mode)
        {
            case DISPLAY_MODE_HOLD:
                LCD_Driver_Print("Mode: HOLD      ");
                display_mode = DISPLAY_MODE_HOLD;
                break;

            case DISPLAY_MODE_DOUBLE:
                LCD_Driver_Print("Mode: DOUBLE    ");
                display_mode = DISPLAY_MODE_DOUBLE;
                last_mode_change = HAL_GetTick();
                break;
            case DISPLAY_MODE_SINGLE:
                           LCD_Driver_Print("Mode: SINGLE    ");
                           display_mode = DISPLAY_MODE_SINGLE;
                           last_mode_change = HAL_GetTick();
                           break;

                       case DISPLAY_MODE_IDLE:
                           // Only update to IDLE if not in a temporary display state
                           if (display_mode == DISPLAY_MODE_IDLE || display_mode == DISPLAY_MODE_HOLD)
                           {
                               LCD_Driver_Print("Mode: IDLE      ");
                               display_mode = DISPLAY_MODE_IDLE;
                           }
                           break;

                       default:
                           LCD_Driver_Print("Mode: IDLE      ");
                           display_mode = DISPLAY_MODE_IDLE;
                           break;
                   }

                   prev_mode = current_mode;
    }
    if ((display_mode == DISPLAY_MODE_SINGLE || display_mode == DISPLAY_MODE_DOUBLE) &&
           (HAL_GetTick() - last_mode_change >= TEMP_DISPLAY_DURATION_MS))
       {
           LCD_Driver_SetCursor(0, 0);
           LCD_Driver_Print("Mode: IDLE      ");
           display_mode = DISPLAY_MODE_IDLE;
       }
   }

static void UpdateLEDDisplay(void)
{
    char buffer[20];
    LCD_Driver_SetCursor(1, 0);

    const char* status_str = GetLEDStatusString(local_status, remote_status);
    snprintf(buffer, sizeof(buffer), "LED: %-11s", status_str);
    LCD_Driver_Print(buffer);
}
static const char* GetLEDStatusString(LEDStatus_t local, LEDStatus_t remote)
{
    if (local == LED_STATUS_OFF && remote == LED_STATUS_OFF)
    {
        return "OFF";
    }
    else if (local == LED_STATUS_HOLD || remote == LED_STATUS_HOLD)
    {
        return "ON (HOLD)";
    }
    else if (local == LED_STATUS_FAST_BLINK || remote == LED_STATUS_FAST_BLINK)
    {
        return "FAST BLINK";
    }
    else if (local == LED_STATUS_SLOW_BLINK || remote == LED_STATUS_SLOW_BLINK)
    {
        return "SLOW BLINK";
    }

    return "OFF";
}
