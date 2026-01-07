#include "TMC2208_Test.h"
#include "TMC2208_Core.h"
#include "TMC2208_UART_Driver.h"
#include "main.h"

volatile uint8_t test1_expected_1;
volatile uint8_t test1_expected_0;
volatile uint8_t test2_expected_1;
volatile uint8_t test3_expected_1;
uint8_t enn;
uint8_t ms1;
uint8_t ms2;
uint8_t enn_manual;
uint8_t ms1_manual;
uint8_t ms2_manual;
volatile uint8_t test_set;
volatile uint8_t test_clear;
volatile uint8_t parse_irun;
volatile uint8_t parse_ihold;
volatile uint8_t parse_delay;
volatile uint8_t test_parse;
volatile uint8_t test_build;

void Test_BasicPinReading(void) {
    uint32_t test_value;
    uint8_t result;

    test_value = 0x00000001;
    result = TMC2208_Core_ReadPin(test_value, TMC2208_PIN_ENN);
    test1_expected_1 = result;  // Should be 1

    test_value = 0x00000000;
    result = TMC2208_Core_ReadPin(test_value, TMC2208_PIN_ENN);
    test1_expected_0 = result;  // Should be 0

    test_value = 0x00000004;
    result = TMC2208_Core_ReadPin(test_value, TMC2208_PIN_MS1);
    test2_expected_1 = result;  // Should be 1

    test_value = 0x00000010;
    result = TMC2208_Core_ReadPin(test_value, TMC2208_PIN_DIAG);
    test3_expected_1 = result;  // Should be 1

    asm("NOP");
}

void Test_ActualIOIN(void) {
    uint32_t ioin = 0x00060505;

    enn = TMC2208_Core_ReadPin(ioin, TMC2208_PIN_ENN);
    ms1 = TMC2208_Core_ReadPin(ioin, TMC2208_PIN_MS1);
    ms2 = TMC2208_Core_ReadPin(ioin, TMC2208_PIN_MS2);

    enn_manual = (ioin >> 0) & 0x01;
    ms1_manual = (ioin >> 2) & 0x01;
    ms2_manual = (ioin >> 3) & 0x01;

//    volatile uint8_t enn_match = (enn == enn_manual);
//    volatile uint8_t ms1_match = (ms1 == ms1_manual);
//    volatile uint8_t ms2_match = (ms2 == ms2_manual);

    asm("NOP");
}

void Test_GCONF_Functions(void) {
    uint32_t gconf;

    gconf = 0x00000000;
    gconf = TMC2208_Core_SetGCONFBit(gconf, GCONF_EN_SPREADCYCLE);
    volatile uint32_t set_result = gconf;  // Should be 0x00000004

    gconf = 0xFFFFFFFF;
    gconf = TMC2208_Core_ClearGCONFBit(gconf, GCONF_EN_SPREADCYCLE);
    volatile uint32_t clear_result = gconf;  // Should be 0xFFFFFFFB

    test_set = (set_result == 0x00000004);
    test_clear = (clear_result == 0xFFFFFFFB);

    asm("NOP");
}

void Test_IHOLD_IRUN_Functions(void) {
    uint32_t reg_value;

    reg_value = TMC2208_Core_BuildCurrents(20, 10, 6);
    volatile uint32_t build_result = reg_value;  // Should be 0x0006140A

    uint8_t irun, ihold, delay;
    TMC2208_Core_ParseCurrents(0x0006140A, &irun, &ihold, &delay);
    parse_irun = irun;   // Should be 20
    parse_ihold = ihold; // Should be 10
    parse_delay = delay; // Should be 6

    test_build = (build_result == 0x0006140A);
    test_parse = (irun == 20) && (ihold == 10) && (delay == 6);

    asm("NOP");
}

//void Test_RealHardware(void) {
//    uint32_t ioin;
//
//    if (TMC2208_ReadReg(TMC2208_REG_IOIN, &ioin)) {
//        uint8_t version = TMC2208_Core_GetVersion(ioin);
//        volatile uint8_t pin_enn = TMC2208_Core_ReadPin(ioin, TMC2208_PIN_ENN);
//        volatile uint8_t pin_ms1 = TMC2208_Core_ReadPin(ioin, TMC2208_PIN_MS1);
//        volatile uint8_t pin_ms2 = TMC2208_Core_ReadPin(ioin, TMC2208_PIN_MS2);
//
//        asm("NOP");
//    }
//}

void RunAllTests(void) {
    Test_BasicPinReading();
    Test_ActualIOIN();
    Test_GCONF_Functions();
    Test_IHOLD_IRUN_Functions();
//    Test_RealHardware();
}
