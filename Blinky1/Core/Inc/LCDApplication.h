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

void LCDApplication_Init(void);
void LCDApplication_UpdateMode(DisplayMode_t mode);
void LCDApplication_UpdateLEDStatus(LEDStatus_t local_status, LEDStatus_t remote_status);
void LCDApplication_Process(void);

#endif
