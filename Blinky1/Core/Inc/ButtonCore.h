#ifndef BUTTON_CORE_H
#define BUTTON_CORE_H

#include "main.h"
#include "ButtonHandler.h"

#define HOLD_TIME_MS 175
#define CLICK_WINDOW_MS 150
#define DEBOUNCE_TIME_MS 10

typedef enum {
    BUTTON_EVENT_NONE = 0,
    BUTTON_EVENT_SINGLE_CLICK,
    BUTTON_EVENT_DOUBLE_CLICK,
    BUTTON_EVENT_HOLD_START,
    BUTTON_EVENT_HOLD_END
} ButtonEvent_t;

void ButtonCore_Update(void);
ButtonEvent_t GetEvent(void);

#endif
