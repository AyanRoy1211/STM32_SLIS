#ifndef INC_LCDAPPLICATION_H_
#define INC_LCDAPPLICATION_H_

#include "main.h"
typedef enum {
    DISPLAY_MODE_IDLE = 0,
    DISPLAY_MODE_SINGLE = 1,
    DISPLAY_MODE_DOUBLE = 2,
    DISPLAY_MODE_HOLD = 3
} DisplayMode_t;

typedef enum {
    LED_STATUS_OFF = 0,
    LED_STATUS_FAST_BLINK = 1,
    LED_STATUS_SLOW_BLINK = 2,
    LED_STATUS_HOLD = 3
} LEDStatus_t;

typedef struct {
	DisplayMode_t mode;
	DisplayMode_t prev_mode;
	uint32_t last_mode_change;
	LEDStatus_t led_status;
	LEDStatus_t prev_led_status;
	uint8_t row_mode;
	uint8_t row_led;
	const char *label_mode;
	const char *label_led;
} LCD_Entity_t;

void LCDApplication_Init(void);

void LCDApplication_UpdateLocalLED(LEDStatus_t status);
void LCD_UpdateLocalMode(uint8_t mode);
void LCD_UpdateLocalLED(uint8_t status);
void LCDApplication_UpdateRemoteMode(DisplayMode_t mode);
void LCDApplication_UpdateRemoteLED(LEDStatus_t status);
void LCD_UpdateRemoteMode(uint8_t mode);
void LCD_UpdateRemoteLED(uint8_t status);
void LCD_SyncLocal(uint8_t event);
void LCD_SyncRemote(uint8_t event);
void LCD_SyncLocalLED(uint8_t status);
void LCD_SyncRemoteLED(uint8_t status);
void LCDApplication_Process(void);


#endif
