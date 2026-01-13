#include "TMC2208_UART_Application.h"
#include "TMC2208_UART_Driver.h"
#include "TMC2208_Core.h"
#include "string.h"
#include <stdio.h>

// UART motor control structure
static TMC2208_Motor_t motor;

// Debug variables for UART commands
volatile uint32_t debug_uart_tx_count = 0;
volatile uint32_t debug_uart_rx_count = 0;
volatile uint32_t debug_last_command = 0;
volatile uint32_t debug_last_response = 0;
volatile uint8_t debug_uart_errors = 0;
volatile uint32_t debug_reg_write_count = 0;
volatile uint32_t debug_reg_read_count = 0;

void TMC2208_ApplicationInit(void) {
    // Initialize UART communication
    TMC2208_UART_Init();

    // Synchronize with TMC2208
    TMC2208_SyncUART(&huart2);

    // Configure TMC2208 via UART
    // Set GCONF register
    uint32_t gconf = 0;
    gconf = TMC2208_GCONF_SetBit(gconf, TMC2208_GCONF_PDN_DISABLE);  // Enable UART
    gconf = TMC2208_GCONF_SetBit(gconf, TMC2208_GCONF_I_SCALE_ANALOG);
    TMC2208_WriteReg(TMC2208_UART_REG_GCONF, gconf);

    // Set microstepping to 8 (via CHOPCONF)
    uint32_t chopconf = 0x000100C3;  // MRES=0 (256 microsteps), other defaults
    TMC2208_WriteReg(TMC2208_UART_REG_CHOPCONF, chopconf);

    // Set current (IHOLD_IRUN)
    uint32_t ihold_irun = 0x0006140A;  // IRUN=20, IHOLD=10, IHOLDDELAY=6
    TMC2208_WriteReg(TMC2208_UART_REG_IHOLD_IRUN, ihold_irun);

    // Set PWM configuration
    uint32_t pwmconf = 0x000401C8;  // Default PWM settings
    TMC2208_WriteReg(TMC2208_UART_REG_PWMCONF, pwmconf);

    motor.enabled = 1;
    motor.current_position = 0;
    motor.target_position = 0;
    motor.moving = 0;

    debug_uart_tx_count = 0;
    debug_uart_errors = 0;
}

void TMC2208_MoveSteps(uint32_t steps, uint32_t speed, uint8_t direction) {
    if (!motor.enabled) return;

    // Calculate target position (relative to current)
    int32_t target;
    if (direction == 0) {
        target = motor.current_position + steps;
    } else {
        target = motor.current_position - steps;
    }

    // Set VACTUAL register for speed control
    uint32_t vactual = speed;  // Speed in microsteps/sec
    if (direction == 1) {
        vactual = -vactual;  // Negative for reverse
    }

    TMC2208_WriteReg(TMC2208_UART_REG_VACTUAL, vactual);

    // Set XTARGET register
    TMC2208_WriteReg(TMC2208_UART_REG_XTARGET, target);

    motor.target_position = target;
    motor.moving = 1;

    // Update debug
    debug_last_command = (steps << 16) | (speed << 8) | direction;
    debug_reg_write_count++;
}

void TMC2208_StartJog(uint32_t speed, uint8_t direction) {
    if (!motor.enabled) return;

    // Set continuous movement via VACTUAL
    uint32_t vactual = speed;
    if (direction == 1) {
        vactual = -vactual;
    }

    TMC2208_WriteReg(TMC2208_UART_REG_VACTUAL, vactual);
    motor.moving = 1;

    debug_last_command = 0xFFFF0000 | (speed << 8) | direction;
    debug_reg_write_count++;
}

void TMC2208_Stop(void) {
    // Stop movement by setting VACTUAL to 0
    TMC2208_WriteReg(TMC2208_UART_REG_VACTUAL, 0);
    motor.moving = 0;

    // Read current position to update our tracking
    uint32_t xactual;
    if (TMC2208_ReadReg(TMC2208_UART_REG_XACTUAL, &xactual)) {
        motor.current_position = xactual;
    }

    debug_last_command = 0x00000000;
    debug_reg_write_count++;
}

void TMC2208_SetMicrostepping(uint16_t microsteps) {
    uint32_t chopconf;

    // Read current CHOPCONF
    if (TMC2208_ReadReg(TMC2208_UART_REG_CHOPCONF, &chopconf)) {
        // Clear MRES bits (27:24)
        chopconf &= ~(0x0F << 24);

        // Set new microstepping
        switch(microsteps) {
            case 1:   chopconf |= (8 << 24); break;  // MRES=8
            case 2:   chopconf |= (7 << 24); break;  // MRES=7
            case 4:   chopconf |= (6 << 24); break;  // MRES=6
            case 8:   chopconf |= (5 << 24); break;  // MRES=5
            case 16:  chopconf |= (4 << 24); break;  // MRES=4
            case 32:  chopconf |= (3 << 24); break;  // MRES=3
            case 64:  chopconf |= (2 << 24); break;  // MRES=2
            case 128: chopconf |= (1 << 24); break;  // MRES=1
            case 256: chopconf |= (0 << 24); break;  // MRES=0
            default:  chopconf |= (5 << 24); break;  // Default to 8
        }

        TMC2208_WriteReg(TMC2208_UART_REG_CHOPCONF, chopconf);
        debug_reg_write_count++;
    }
}

void TMC2208_SetCurrent(uint16_t run_current_ma, uint16_t hold_current_ma) {
    // Convert mA to TMC2208 current setting (0-31)
    uint8_t irun = (run_current_ma * 31) / 2000;   // Max 2A
    uint8_t ihold = (hold_current_ma * 31) / 2000;

    if (irun > 31) irun = 31;
    if (ihold > 31) ihold = 31;

    uint32_t ihold_irun = (ihold << 0) | (irun << 8) | (6 << 16);  // 6 = iholddelay

    TMC2208_WriteReg(TMC2208_UART_REG_IHOLD_IRUN, ihold_irun);
    debug_reg_write_count++;
}

void TMC2208_EnableDriver(void) {
    // Enable driver via GCONF (clear pdn_disable bit)
    uint32_t gconf;
    if (TMC2208_ReadReg(TMC2208_UART_REG_GCONF, &gconf)) {
        gconf = TMC2208_GCONF_ClearBit(gconf, TMC2208_GCONF_PDN_DISABLE);
        TMC2208_WriteReg(TMC2208_UART_REG_GCONF, gconf);
        motor.enabled = 1;
        debug_reg_write_count++;
    }
}

void TMC2208_DisableDriver(void) {
    // Disable driver via GCONF (set pdn_disable bit)
    uint32_t gconf;
    if (TMC2208_ReadReg(TMC2208_UART_REG_GCONF, &gconf)) {
        gconf = TMC2208_GCONF_SetBit(gconf, TMC2208_GCONF_PDN_DISABLE);
        TMC2208_WriteReg(TMC2208_UART_REG_GCONF, gconf);
        motor.enabled = 0;
        debug_reg_write_count++;
    }
}

uint32_t TMC2208_GetCurrentPosition(void) {
    uint32_t position;
    if (TMC2208_ReadReg(TMC2208_UART_REG_XACTUAL, &position)) {
        motor.current_position = position;
        debug_reg_read_count++;
    }
    return motor.current_position;
}

uint32_t TMC2208_GetFaultFlags(void) {
    uint32_t drv_status;
    if (TMC2208_ReadReg(TMC2208_UART_REG_DRV_STATUS, &drv_status)) {
        debug_reg_read_count++;
        return drv_status;
    }
    return 0;
}

void TMC2208_ReadStatus(void) {
    // Read multiple status registers
    uint32_t io_status, drv_status;

    if (TMC2208_ReadReg(TMC2208_UART_REG_IOIN, &io_status)) {
        // Parse IO status
        debug_last_response = io_status;
        debug_reg_read_count++;
    }

    if (TMC2208_ReadReg(TMC2208_UART_REG_DRV_STATUS, &drv_status)) {
        // Check for faults
        debug_reg_read_count++;
    }
}

// Debug function to print UART status
void TMC2208_DebugPrint(void) {
    char buffer[128];

    sprintf(buffer, "\r\n=== TMC2208 UART DEBUG ===\r\n");
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    sprintf(buffer, "TX Count: %lu, RX Count: %lu\r\n",
            debug_uart_tx_count, debug_uart_rx_count);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    sprintf(buffer, "UART Errors: %d\r\n", debug_uart_errors);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    sprintf(buffer, "Reg Writes: %lu, Reg Reads: %lu\r\n",
            debug_reg_write_count, debug_reg_read_count);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);

    sprintf(buffer, "Last Cmd: 0x%08lX, Last Resp: 0x%08lX\r\n",
            debug_last_command, debug_last_response);
    HAL_UART_Transmit(&huart2, (uint8_t*)buffer, strlen(buffer), 100);
}
