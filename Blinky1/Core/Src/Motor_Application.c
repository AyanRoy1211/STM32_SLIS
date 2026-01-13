#include "Motor_Application.h"
#include "TMC2208_UART_Application.h"
#include "gpio.h"
#include <stdio.h>
#include "string.h"

// Motor control structure for UART-based control
MotorController_t motor_ctrl = {0, 0, 0, 0, 0, MOTOR_STATE_IDLE};

// Debug variables (make them extern for monitoring)
volatile uint32_t debug_motor_steps_requested = 0;
volatile uint32_t debug_motor_steps_completed = 0;
volatile MotorState_t debug_motor_current_state = MOTOR_STATE_IDLE;
volatile uint32_t debug_motor_target_position = 0;
volatile uint32_t debug_motor_current_position = 0;
volatile uint8_t debug_motor_speed = 0;
volatile uint32_t debug_motor_fault_flags = 0;

void MotorApplicationInit(void) {
    // Initialize TMC2208 via UART
    TMC2208_ApplicationInit();

    // Set up motor parameters via UART
    TMC2208_SetMicrostepping(8);  // 8 microsteps per full step
    TMC2208_SetCurrent(1500, 1000);  // 1.5A run, 1.0A hold
    TMC2208_EnableDriver();  // Enable via UART command

    motor_ctrl.state = MOTOR_STATE_IDLE;

    // Initialize debug variables
    debug_motor_steps_requested = 0;
    debug_motor_steps_completed = 0;
    debug_motor_current_state = MOTOR_STATE_IDLE;
}

void SetMotorMode(EVENT_CODES_ENUM event) {
    switch (event) {
        case EVT_SINGLE_CLICK:
            // Move 200 steps forward at medium speed
            debug_motor_steps_requested += 200;
            motor_ctrl.target_steps = 200;
            motor_ctrl.speed = 500;  // Medium speed
            motor_ctrl.direction = 0;  // Forward
            motor_ctrl.state = MOTOR_STATE_MOVING;
            TMC2208_MoveSteps(200, 500, 0);  // Steps, speed, direction
            break;

        case EVT_DOUBLE_CLICK:
            // Move 200 steps backward at slow speed
            debug_motor_steps_requested += 200;
            motor_ctrl.target_steps = 200;
            motor_ctrl.speed = 1000;  // Slow speed
            motor_ctrl.direction = 1;  // Backward
            motor_ctrl.state = MOTOR_STATE_MOVING;
            TMC2208_MoveSteps(200, 1000, 1);
            break;

        case EVT_HOLD_START:
            // Continuous movement at medium speed
            debug_motor_steps_requested = 0xFFFFFFFF;  // Continuous
            motor_ctrl.target_steps = 0;  // 0 = continuous
            motor_ctrl.speed = 500;
            motor_ctrl.direction = 0;
            motor_ctrl.state = MOTOR_STATE_RUNNING;
            TMC2208_StartJog(500, 0);  // Start continuous movement
            break;

        case EVT_HOLD_END:
            // Stop all movement
            motor_ctrl.state = MOTOR_STATE_STOPPING;
            TMC2208_Stop();
            motor_ctrl.state = MOTOR_STATE_IDLE;
            debug_motor_current_state = MOTOR_STATE_IDLE;
            break;

        default:
            break;
    }

    // Update debug state
    debug_motor_current_state = motor_ctrl.state;
}

void MotorApplication_Process(void) {
    static uint32_t last_update = 0;
    uint32_t current_time = HAL_GetTick();

    // Update motor status every 100ms
    if (current_time - last_update > 100) {
        last_update = current_time;

        // Read motor status via UART
        TMC2208_ReadStatus();

        // Update debug variables with actual driver status
        if (motor_ctrl.state == MOTOR_STATE_MOVING ||
            motor_ctrl.state == MOTOR_STATE_RUNNING) {

            // Read actual position (simulated - you'd implement actual UART read)
            debug_motor_current_position = TMC2208_GetCurrentPosition();

            // Check if move is complete
            if (motor_ctrl.target_steps > 0) {
                if (debug_motor_current_position >= debug_motor_steps_completed + motor_ctrl.target_steps) {
                    motor_ctrl.state = MOTOR_STATE_IDLE;
                    debug_motor_current_state = MOTOR_STATE_IDLE;
                    debug_motor_steps_completed += motor_ctrl.target_steps;
                }
            }
        }

        // Read fault flags
        debug_motor_fault_flags = TMC2208_GetFaultFlags();
    }
}

// Debug function to print motor status via UART
void MotorApplication_DebugPrint(void) {
    char buffer[128];

    sprintf(buffer, "\r\n=== MOTOR DEBUG ===\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    sprintf(buffer, "State: %d (0=Idle,1=Moving,2=Running,3=Stopping)\r\n",
            debug_motor_current_state);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    sprintf(buffer, "Steps Req: %lu, Steps Done: %lu\r\n",
            debug_motor_steps_requested, debug_motor_steps_completed);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    sprintf(buffer, "Curr Pos: %lu, Target: %lu\r\n",
            debug_motor_current_position, debug_motor_target_position);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    sprintf(buffer, "Speed: %d, Dir: %s\r\n",
            debug_motor_speed, motor_ctrl.direction ? "REV" : "FWD");
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    sprintf(buffer, "Fault Flags: 0x%08lX\r\n", debug_motor_fault_flags);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
}
