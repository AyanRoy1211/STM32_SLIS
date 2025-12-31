#include "LCDApplication.h"
#include "LCDCore.h"
#include <stdio.h>
#define TEMP_DISPLAY_DURATION_MS 1000

typedef enum {
    LCD_INIT_RESET,      // Screen just cleared
    LCD_INIT_LABELS,     // Need to print "L-MODE:", "R-LED:", etc.
    LCD_INIT_READY       // Labels are done, start updating values
} LCD_InitState_t;

static LCD_InitState_t init_state = LCD_INIT_RESET;

static DisplayMode_t local_mode = DISPLAY_MODE_IDLE;
static DisplayMode_t local_prev_mode = 255;
static uint32_t local_last_mode_change = 0;
static LEDStatus_t local_led_status = LED_STATUS_OFF;
static DisplayMode_t remote_mode = DISPLAY_MODE_IDLE;
static DisplayMode_t remote_prev_mode = 255;
static LEDStatus_t local_prev_led_status = 255;
static LEDStatus_t remote_prev_led_status = 255;
static LEDStatus_t remote_led_status = LED_STATUS_OFF;
static uint32_t remote_last_mode_change = 0;

static void DisplayLocalMode(void);
static void DisplayLocalLED(void);
static void DisplayRemoteMode(void);
static void DisplayRemoteLED(void);

void LCD_UpdateLocalMode(DisplayMode_t mode);
void LCD_UpdateLocalLED(LEDStatus_t status);
void LCD_UpdateRemoteMode(DisplayMode_t mode);
void LCD_UpdateRemoteLED(LEDStatus_t status);


#define ROW_LOCAL_MODE   0
#define ROW_LOCAL_LED    1
#define ROW_REMOTE_MODE  2
#define ROW_REMOTE_LED   3


#define COL_MODE_VALUE   8
#define COL_LED_VALUE    8

void LCD_UpdateLocalMode(DisplayMode_t mode)
{
    if (local_mode != mode)
    {
        local_mode = mode;
        local_prev_mode = 255;
        local_last_mode_change = HAL_GetTick();
    }
}
void LCD_UpdateLocalLED(LEDStatus_t status)
{
	if (local_led_status != status)
	    {
	        local_led_status = status;
	        local_prev_led_status = 255;
	    }
}
void LCD_UpdateRemoteMode(DisplayMode_t mode)
{
    if (remote_mode != mode)
    {
        remote_mode = mode;
        remote_prev_mode = 255;
        remote_last_mode_change=HAL_GetTick();

    }
}

void LCD_UpdateRemoteLED(LEDStatus_t status)
{
	if (remote_led_status != status)
	    {
	        remote_led_status = status;
	        remote_prev_led_status = 255;
	    }
}

void LCDApplication_Init(void)
{
		LCD_Init();

	    LCD_Driver_Clear();
	    init_state = LCD_INIT_RESET;
	    local_prev_mode = remote_prev_mode = 255;
	    local_prev_led_status = remote_prev_led_status = 255;
}


void LCDApplication_Process(void)
{

	    if (init_state == LCD_INIT_RESET)
	    {

	        LCD_Driver_SetCursor(ROW_LOCAL_MODE, 0);  LCD_Driver_Print("L-MODE: ");
	        LCD_Driver_SetCursor(ROW_LOCAL_LED, 0);   LCD_Driver_Print("L-LED : ");
	        LCD_Driver_SetCursor(ROW_REMOTE_MODE, 0); LCD_Driver_Print("R-MODE: ");
	        LCD_Driver_SetCursor(ROW_REMOTE_LED, 0);  LCD_Driver_Print("R-LED : ");

	        init_state = LCD_INIT_READY;
	        return;
	    }

	    if ((local_mode == DISPLAY_MODE_SINGLE || local_mode == DISPLAY_MODE_DOUBLE) &&
	        (HAL_GetTick() - local_last_mode_change >= TEMP_DISPLAY_DURATION_MS))
	    {
	        local_mode = DISPLAY_MODE_IDLE;
	    }

	    if ((remote_mode == DISPLAY_MODE_SINGLE || remote_mode == DISPLAY_MODE_DOUBLE) &&
	        (HAL_GetTick() - remote_last_mode_change >= TEMP_DISPLAY_DURATION_MS))
	    {
	        remote_mode = DISPLAY_MODE_IDLE;
	    }

	    DisplayLocalMode();
	    DisplayLocalLED();
	    DisplayRemoteMode();
	    DisplayRemoteLED();
}
static void DisplayLocalMode(void)
{
	 if (local_mode == local_prev_mode) return;

	    LCD_Driver_SetCursor(ROW_LOCAL_MODE, COL_MODE_VALUE);

	    switch (local_mode)
	    {
	        case DISPLAY_MODE_SINGLE: LCD_Driver_Print("SINGLE "); break;
	        case DISPLAY_MODE_DOUBLE: LCD_Driver_Print("DOUBLE "); break;
	        case DISPLAY_MODE_HOLD:   LCD_Driver_Print("HOLD   "); break;
	        default:                  LCD_Driver_Print("IDLE   "); break;
	    }
	    local_prev_mode = local_mode;

}
static void DisplayLocalLED(void)
{
	 if (local_led_status == local_prev_led_status) return;

	    LCD_Driver_SetCursor(ROW_LOCAL_LED, COL_LED_VALUE);

	    switch (local_led_status)
	    {
	        case LED_STATUS_HOLD:       LCD_Driver_Print("ON (HOLD) "); break;
	        case LED_STATUS_FAST_BLINK: LCD_Driver_Print("FAST BLNK "); break;
	        case LED_STATUS_SLOW_BLINK: LCD_Driver_Print("SLOW BLNK "); break;
	        default:                    LCD_Driver_Print("OFF       "); break;
	    }

	    local_prev_led_status = local_led_status;
}
static void DisplayRemoteMode(void)
{

	if (remote_mode == remote_prev_mode) return;

	    LCD_Driver_SetCursor(ROW_REMOTE_MODE, COL_MODE_VALUE);

	    switch (remote_mode)
	    {
	        case DISPLAY_MODE_SINGLE: LCD_Driver_Print("SINGLE "); break;
	        case DISPLAY_MODE_DOUBLE: LCD_Driver_Print("DOUBLE "); break;
	        case DISPLAY_MODE_HOLD:   LCD_Driver_Print("HOLD   "); break;
	        default:                  LCD_Driver_Print("IDLE   "); break;
	    }

	    remote_prev_mode = remote_mode;
}
static void DisplayRemoteLED(void)
{
	 if (remote_led_status == remote_prev_led_status) return;

	    LCD_Driver_SetCursor(ROW_REMOTE_LED, COL_LED_VALUE);

	    switch (remote_led_status)
	    {
	        case LED_STATUS_HOLD:       LCD_Driver_Print("ON (HOLD) "); break;
	        case LED_STATUS_FAST_BLINK: LCD_Driver_Print("FAST BLNK "); break;
	        case LED_STATUS_SLOW_BLINK: LCD_Driver_Print("SLOW BLNK "); break;
	        default:                    LCD_Driver_Print("OFF       "); break;
	    }

	    remote_prev_led_status = remote_led_status;
}
