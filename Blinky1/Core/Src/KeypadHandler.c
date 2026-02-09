#include "KeypadHandler.h"
#include "Keypad.h"

Keypad_t HardwareKeypad;
KeypadApplicationTypedefStruct KeypadData;

static KEYPAD_EVENT_CODES_ENUM MapKeyToEvent(Keypad_Key_t key, Keypad_Event_t event_type)
{
    switch (key) {
        case KEY_GRAIN_LEFT:
            if (event_type == KEYPAD_EVENT_KEY_PRESSED) {
                return MOTOR_CLOCKWISE;
            }
            break;

        case KEY_GRAIN_RIGHT:
            if (event_type == KEYPAD_EVENT_KEY_PRESSED) {
                return MOTOR_ANTICLOCKWISE;
            }
            break;

        case KEY_WASS:
            if (event_type == KEYPAD_EVENT_KEY_HOLD) {
                              return MOTOR_CONTINUOUS;
            }
            else if (event_type == KEYPAD_EVENT_KEY_RELEASED) {
                           return MOTOR_STOP;
            }
            break;

        case KEY_MINUS:
        	if (event_type == KEYPAD_EVENT_KEY_PRESSED)
        	         return MOTOR_SPEED_INCREASE;
        	break;


        default:
            break;
    }

    return EVT_NONE;
}

void KeypadHandler_Init(void)
{
    Keypad_Init(&HardwareKeypad);

    KeypadData.CurrentEvent = EVT_NONE;
    KeypadData.PreviousEvent = EVT_NONE;
    KeypadData.NewEventReceived = false;
    KeypadData.CurrentKey = KEY_NONE;
    KeypadData.CurrentChar = '\0';
}

void UpdateKeypad(void)
{
    Keypad_Scan(&HardwareKeypad);

    Keypad_Key_t key;
    Keypad_Event_t event_type = Keypad_GetEvent(&HardwareKeypad, &key);

    if (event_type == KEYPAD_EVENT_NONE) {
        return;
    }

    KeypadData.CurrentKey = key;
    KeypadData.CurrentChar = Keypad_GetChar(key);

    EVENT_CODES_ENUM app_event = MapKeyToEvent(key, event_type);

    if (app_event != EVT_NONE)
    {
        KeypadData.PreviousEvent = KeypadData.CurrentEvent;
        KeypadData.CurrentEvent = app_event;
        KeypadData.NewEventReceived = true;
    }
}
