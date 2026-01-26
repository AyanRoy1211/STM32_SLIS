#ifndef KEYPADHANDLER_H
#define KEYPADHANDLER_H

#include "Keypad.h"
#include "Common.h"

typedef struct {
    EVENT_CODES_ENUM CurrentEvent;
    EVENT_CODES_ENUM PreviousEvent;
    bool NewEventReceived;
    Keypad_Key_t CurrentKey;
    char CurrentChar;
} KeypadApplicationTypedefStruct;

void KeypadHandler_Init(void);
void UpdateKeypad(void);

#endif
