#include "TMC2208_UART_Application.h"
#include "gpio.h"

void TMC2208_ApplicationInit(void)
{
    TMC2208_UART_Init();
}

void TMC2208_RunTest(void)
{
    // Write test values to registers
    TMC2208_WriteReg(0x10, 0x00000505);  // IHOLD_IRUN
    TMC2208_ReadReg(0x10);               // Read it back

    TMC2208_WriteReg(0x00, 0x00000042);  // GCONF
    TMC2208_ReadReg(0x00);               // Read it back

    // Breakpoint here to inspect variables
    __asm("NOP");  // NOP for easy breakpoint
}

void TMC2208_ConfigureMotor(float irun_amps, float ihold_amps, uint8_t microsteps)
{
    uint8_t cs_irun = (uint8_t)(irun_amps / 0.325 / 1.414);
    uint8_t cs_ihold = (uint8_t)(ihold_amps / 0.325 / 1.414);

    if (cs_irun > 31) cs_irun = 31;
    if (cs_ihold > 31) cs_ihold = 31;

    uint16_t irun_ihold = (0 << 12) | (cs_ihold << 8) | cs_irun;

    TMC2208_ConfigureBasic(irun_ihold, microsteps);
}
