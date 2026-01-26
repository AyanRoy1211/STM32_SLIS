#include "Motor_Debug.h"
#include <stdio.h>
#include "usart.h"
#include "string.h"

MotorDebug_t motor_debug = {0};

uint8_t IsMotorCommunicationOK(void) {
    return (motor_debug.comm_ok && (motor_debug.uart_errors == 0));
}

uint8_t IsMotorMoving(void) {
    return (motor_debug.motor_state == 1 || motor_debug.motor_state == 2);
}

uint8_t HasMotorFault(void) {
    return (motor_debug.motor_state == 3 ||
            motor_debug.fault_flags != 0 ||
            motor_debug.overtemperature ||
            motor_debug.short_circuit);
}

void PrintMotorDebug(void) {
    char buffer[256];

    sprintf(buffer, "\r\n" \
                   "╔══════════════════════════════════╗\r\n" \
                   "║      MOTOR DEBUG STATUS         ║\r\n" \
                   "╠══════════════════════════════════╣\r\n");
   HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    // Communication Status
    sprintf(buffer, "║ UART: TX=%lu RX=%lu ERR=%d %s║\r\n",
            motor_debug.uart_tx_count,
            motor_debug.uart_rx_count,
            motor_debug.uart_errors,
            motor_debug.comm_ok ? "[OK] " : "[FAIL]");
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    // Motor State
    const char* states[] = {"IDLE", "MOVING", "RUNNING", "FAULT"};
    sprintf(buffer, "║ State: %-8s Pos: %8ld/%8ld║\r\n",
            states[motor_debug.motor_state],
            motor_debug.current_pos,
            motor_debug.target_pos);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    // Speed/Direction
    sprintf(buffer, "║ Speed: %4lu Hz Dir: %s En: %d║\r\n",
            motor_debug.speed,
            motor_debug.direction ? "REV" : "FWD",
            motor_debug.driver_enabled);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    // Fault Status
    sprintf(buffer, "║ Faults: %s %s %s║\r\n",
            motor_debug.overtemperature ? "OT " : "   ",
            motor_debug.short_circuit ? "SC " : "   ",
            motor_debug.fault_flags ? "REG " : "   ");
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    sprintf(buffer, "║ Last Cmd: 0x%08lX  Resp: 0x%08lX║\r\n",
            motor_debug.last_command,
            motor_debug.last_response);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    sprintf(buffer, "╚══════════════════════════════════╝\r\n\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
}

