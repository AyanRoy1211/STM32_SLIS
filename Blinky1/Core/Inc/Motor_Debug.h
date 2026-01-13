#ifndef MOTOR_DEBUG_H
#define MOTOR_DEBUG_H

#include <stdint.h>

// Motor Debug Structure (pack all key variables)
typedef struct {
    // Communication
    uint32_t uart_tx_count;
    uint32_t uart_rx_count;
    uint8_t  uart_errors;
    uint8_t  comm_ok;         // 1 = communication working

    // Motor State
    uint8_t  motor_state;     // 0=IDLE, 1=MOVING, 2=RUNNING, 3=FAULT
    uint8_t  driver_enabled;  // 1 = TMC2208 enabled

    // Position/Speed
    int32_t  current_pos;     // Current position (microsteps)
    int32_t  target_pos;      // Target position
    uint32_t speed;           // Current speed (Hz)
    uint8_t  direction;       // 0=forward, 1=reverse

    // Status/Faults
    uint32_t fault_flags;     // TMC2208 fault bits
    uint8_t  overtemperature; // 1 = too hot
    uint8_t  short_circuit;   // 1 = short circuit detected

    // Last Commands
    uint32_t last_command;    // Last command sent
    uint32_t last_response;   // Last response received
    uint32_t last_error;      // Last error code

} MotorDebug_t;

extern MotorDebug_t motor_debug;

// Quick status check functions
uint8_t IsMotorCommunicationOK(void);
uint8_t IsMotorMoving(void);
uint8_t HasMotorFault(void);
void PrintMotorDebug(void);

#endif
