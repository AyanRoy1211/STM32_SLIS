#ifndef INC_LED_CONTROLLER_H_
#define INC_LED_CONTROLLER_H_

#include "main.h"

typedef enum {
    LED_MODE_IDLE = 0,
    LED_MODE_FAST_BLINK,
    LED_MODE_SLOW_BLINK,
    LED_MODE_HOLD_ON
} LED_Mode_t;

typedef struct {
    LED_Mode_t mode;
    uint32_t blink_timer;
    uint32_t duration_timer;
} LED_Controller_t;

#define FAST_BLINK_PERIOD 	25
#define SLOW_BLINK_PERIOD  	100
#define BLINK_DURATION_MS  	1000

void LED_Controller_Init(LED_Controller_t *ctrl, LED_Mode_t initial_mode);
void LED_Controller_SetMode(LED_Controller_t *ctrl, LED_Mode_t mode);
void LED_Controller_Update(LED_Controller_t *ctrl, GPIO_TypeDef *port, uint16_t pin);

#endif /* INC_LED_CONTROLLER_H_ */
