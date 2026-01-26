#ifndef MOTOR_APPLICATION_H
#define MOTOR_APPLICATION_H

#include "main.h"

//// Motor states
//typedef enum {
//    MOTOR_STATE_IDLE,
//    MOTOR_STATE_MOVING,
//    MOTOR_STATE_RUNNING,  // Continuous movement
//    MOTOR_STATE_STOPPING,
//    MOTOR_STATE_FAULT
//} MotorState_t;
//
//// Motor control structure
//typedef struct {
//    uint32_t target_steps;
//    uint32_t completed_steps;
//    uint32_t speed;        // In Hz or RPM
//    uint8_t direction;     // 0 = forward, 1 = reverse
//    MotorState_t state;
//    uint32_t fault_flags;
//} MotorController_t;
//
//// Debug variables (extern for monitoring)
//extern volatile uint32_t debug_motor_steps_requested;
//extern volatile uint32_t debug_motor_steps_completed;
//extern volatile MotorState_t debug_motor_current_state;
//extern volatile uint32_t debug_motor_target_position;
//extern volatile uint32_t debug_motor_current_position;
//extern volatile uint8_t debug_motor_speed;
//extern volatile uint32_t debug_motor_fault_flags;
//
//extern MotorController_t motor_ctrl;
////extern UART_HandleTypeDef huart2;

void MotorApplication_Init(void);
void Motor_Application_SetMotorMode(EVENT_CODES_ENUM event);
void MotorApplication_Update(void);

#endif
