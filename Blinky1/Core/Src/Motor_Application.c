#include "Motor_Application.h"
#include "Motor_Controller.h"
#include "gpio.h"
#include <stdio.h>
#include "string.h"

extern MotorController_t motor_control;

#define STEPS_PER_MOVE 200
#define PERIOD_MEDIUM_US 2000
#define PERIOD_SLOW_US 5000

void MotorApplication_Init(void) {
//    // Initialize TMC2208 via UART
//    TMC2208_ApplicationInit();
//
//    // Set up motor parameters via UART
//    TMC2208_SetMicrostepping(8);  // 8 microsteps per full step
//    TMC2208_SetCurrent(1500, 1000);  // 1.5A run, 1.0A hold
//    TMC2208_EnableDriver();  // Enable via UART command
//
//    motor_ctrl.state = MOTOR_STATE_IDLE;
//
//    // Initialize debug variables
//    debug_motor_steps_requested = 0;
//    debug_motor_steps_completed = 0;
//    debug_motor_current_state = MOTOR_STATE_IDLE;
	Motor_Init();
}

void MotorApplication_SetMotorMode(EVENT_CODES_ENUM event) {
    switch (event) {
        case EVT_SINGLE_CLICK:
        	motor_control.direction = 0;
        	motor_control.period_us = PERIOD_MEDIUM_US;
        	motor_control.steps_remaining = STEPS_PER_MOVE;
        	motor_control.running = 1;
            break;

        case EVT_DOUBLE_CLICK:
        	motor_control.direction = 1;
        	motor_control.period_us = PERIOD_SLOW_US;
        	motor_control.steps_remaining = STEPS_PER_MOVE;
        	motor_control.running = 1;
            break;

        case EVT_HOLD_START:
        	motor_control.direction = -1;
        	motor_control.period_us = PERIOD_MEDIUM_US;
        	motor_control.steps_remaining = 0;
        	motor_control.running = 1;
            break;

        case EVT_HOLD_END:
        	motor_control.running = 0;
        	motor_control.accumulator_us = 0;
            break;

        default:
            break;
    }

}

void MotorApplication_Update(void) {

	Motor_Update();
}


