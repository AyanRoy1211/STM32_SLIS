#ifndef TMC2208_UART_APPLICATION_H
#define TMC2208_UART_APPLICATION_H

#include "main.h"

// TMC2208 Register addresses

#define TMC2208_UART_REG_GCONF      0x00
#define TMC2208_UART_REG_IOIN       0x06
#define TMC2208_UART_REG_XACTUAL    0x21
#define TMC2208_UART_REG_XTARGET    0x2D
#define TMC2208_UART_REG_VACTUAL    0x22
#define TMC2208_UART_REG_IHOLD_IRUN 0x10
#define TMC2208_UART_REG_CHOPCONF   0x6C
#define TMC2208_UART_REG_PWMCONF    0x70
#define TMC2208_UART_REG_DRV_STATUS 0x6F

// Motor structure
typedef struct {
    uint8_t enabled;
    uint32_t current_position;
    uint32_t target_position;
    uint8_t moving;
} TMC2208_Motor_t;

// Debug variables
extern volatile uint32_t debug_uart_tx_count;
extern volatile uint32_t debug_uart_rx_count;
extern volatile uint32_t debug_last_command;
extern volatile uint32_t debug_last_response;
extern volatile uint8_t debug_uart_errors;
extern volatile uint32_t debug_reg_write_count;
extern volatile uint32_t debug_reg_read_count;

void TMC2208_ApplicationInit(void);
void TMC2208_MoveSteps(uint32_t steps, uint32_t speed, uint8_t direction);
void TMC2208_StartJog(uint32_t speed, uint8_t direction);
void TMC2208_Stop(void);
void TMC2208_SetMicrostepping(uint16_t microsteps);
void TMC2208_SetCurrent(uint16_t run_current_ma, uint16_t hold_current_ma);
void TMC2208_EnableDriver(void);
void TMC2208_DisableDriver(void);
uint32_t TMC2208_GetCurrentPosition(void);
uint32_t TMC2208_GetFaultFlags(void);
void TMC2208_ReadStatus(void);
void TMC2208_DebugPrint(void);

#endif
