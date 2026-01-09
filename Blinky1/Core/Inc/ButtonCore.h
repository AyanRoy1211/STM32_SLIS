#ifndef BUTTON_CORE_H
#define BUTTON_CORE_H

#include "main.h"

typedef struct {
	uint8_t btn_stable;
	uint8_t btn_prev;
	int32_t btn_debounce;
	uint32_t btn_press_start;
	uint32_t btn_release_start;
	uint8_t btn_clicks;
	uint8_t btn_hold;
	uint32_t btn_click_timer;
} ButtonStates_t;

typedef enum {
    BUTTON_EVENT_NONE = 0,
    BUTTON_EVENT_SINGLE_CLICK,
    BUTTON_EVENT_DOUBLE_CLICK,
    BUTTON_EVENT_HOLD_START,
    BUTTON_EVENT_HOLD_END
} BUTTON_EVENT_TYPEDEF_ENUM;

#define HOLD_TIME_MS 175
#define CLICK_WINDOW_MS 150
#define DEBOUNCE_TIME_MS 10

void ButtonCore_Update(void);
BUTTON_EVENT_TYPEDEF_ENUM GetEvent(void);

#endif
