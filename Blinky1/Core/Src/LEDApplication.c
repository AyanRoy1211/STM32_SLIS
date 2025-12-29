#include "LEDApplication.h"

LED_Controller_t local_ctrl  = {LED_MODE_IDLE, 0, 0};  // LD3
LED_Controller_t remote_ctrl = {LED_MODE_IDLE, 0, 0};  // LD4

extern uint8_t current_mode;

void UpdateLED(void) {
    // Single sync line - NO timer interference
    local_ctrl.mode = (LED_Mode_t)current_mode;

    // Update both controllers
    LED_Controller_Update(&local_ctrl,  LD3_GPIO_Port, LD3_Pin);
    LED_Controller_Update(&remote_ctrl, LD4_GPIO_Port, LD4_Pin);
}

void SetRemoteMode(uint8_t mode) {
    remote_ctrl.mode = (LED_Mode_t)mode;
}
