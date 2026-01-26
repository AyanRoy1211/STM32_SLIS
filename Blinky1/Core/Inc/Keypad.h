#ifndef KEYPAD_H
#define KEYPAD_H

#include "stm32f0xx_hal.h"
#include <stdint.h>
#include <stdbool.h>


#define KEYPAD_ROWS 5
#define KEYPAD_COLS 4
#define KEYPAD_DEBOUNCE_MS 20
#define KEYPAD_HOLD_TIME_MS 30

#define ROW1_PIN GPIO_PIN_0
#define ROW1_PORT GPIOC
#define ROW2_PIN GPIO_PIN_1
#define ROW2_PORT GPIOC
#define ROW3_PIN GPIO_PIN_2
#define ROW3_PORT GPIOC
#define ROW4_PIN GPIO_PIN_3
#define ROW4_PORT GPIOC
#define ROW5_PIN GPIO_PIN_10
#define ROW5_PORT GPIOC

#define COL1_PIN GPIO_PIN_4
#define COL1_PORT GPIOC
#define COL2_PIN GPIO_PIN_5
#define COL2_PORT GPIOC
#define COL3_PIN GPIO_PIN_6
#define COL3_PORT GPIOC
#define COL4_PIN GPIO_PIN_7
#define COL4_PORT GPIOC

typedef enum {
    KEY_NONE = 0,

    KEY_F1,
    KEY_F2,
    KEY_CLR,
    KEY_PLUS,
    KEY_ENT,
    KEY_UP,
    KEY_LEFT,
    KEY_RIGHT,
    KEY_ZEUT,
    KEY_DN,
    KEY_MINUS,

    KEY_WASS,
    KEY_AUTO,
    KEY_LIFT,

    KEY_FACET,
    KEY_START,
    KEY_GRAIN
} Keypad_Key_t;

typedef enum {
    KEYPAD_EVENT_NONE = 0,
    KEYPAD_EVENT_KEY_PRESSED,
    KEYPAD_EVENT_KEY_RELEASED,
    KEYPAD_EVENT_KEY_HOLD
} Keypad_Event_t;

typedef struct {
    uint8_t current_key;
    uint8_t previous_key;
    uint32_t press_start_time;
    uint32_t debounce_timer;
    bool key_hold_triggered;
    Keypad_Event_t pending_event;
    Keypad_Key_t pressed_key;
} Keypad_t;


void Keypad_Init(Keypad_t *keypad);
void Keypad_Scan(Keypad_t *keypad);
Keypad_Event_t Keypad_GetEvent(Keypad_t *keypad, Keypad_Key_t *key);
char Keypad_GetChar(Keypad_Key_t key);
const char* Keypad_GetKeyName(Keypad_Key_t key);
#endif
