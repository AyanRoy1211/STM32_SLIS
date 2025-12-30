#include "LED_Controller.h"

void LED_Controller_Init(LED_Controller_t *ctrl, LED_Mode_t initial_mode)
{
    ctrl->mode = initial_mode;
    ctrl->blink_timer = 0;
    ctrl->duration_timer = 0;
}

void LED_Controller_SetMode(LED_Controller_t *ctrl, LED_Mode_t mode)
{
    ctrl->mode = mode;
    ctrl->blink_timer = 0;
    ctrl->duration_timer = 0;
}

void LED_Controller_Update(LED_Controller_t *ctrl, GPIO_TypeDef *port, uint16_t pin) {
    switch (ctrl->mode) {
        case LED_MODE_IDLE:
            HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
            ctrl->blink_timer = 0;
            ctrl->duration_timer = 0;
            break;

        case LED_MODE_FAST_BLINK:
        case LED_MODE_SLOW_BLINK:
            ctrl->duration_timer++;
            ctrl->blink_timer++;

            uint32_t period = (ctrl->mode == LED_MODE_FAST_BLINK) ?
                             FAST_BLINK_PERIOD : SLOW_BLINK_PERIOD;

            if (ctrl->blink_timer >= period) {
                ctrl->blink_timer = 0;
                HAL_GPIO_TogglePin(port, pin);
            }

            // Auto-expiry after 1s
            if (ctrl->duration_timer >= BLINK_DURATION_MS)
            {
                ctrl->mode = LED_MODE_IDLE;
                HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
                ctrl->duration_timer = 0;
                ctrl->blink_timer = 0;
            }
            break;

        case LED_MODE_HOLD_ON:
            HAL_GPIO_WritePin(port, pin, GPIO_PIN_SET);
            break;

        default:
            HAL_GPIO_WritePin(port, pin, GPIO_PIN_RESET);
            ctrl->mode = LED_MODE_IDLE;
            break;
    }
}
