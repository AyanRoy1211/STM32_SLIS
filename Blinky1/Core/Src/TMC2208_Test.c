#include "TMC2208_Test.h"

// Test 1: Basic Pin Reading
volatile uint8_t test1_bit0_set = 0;         // Expected: 1
volatile uint8_t test1_bit0_clear = 0;       // Expected: 0
volatile uint8_t test1_bit2_set = 0;         // Expected: 1
volatile uint8_t test1_bit4_set = 0;         // Expected: 1
volatile uint8_t test1_pass = 0;             // Expected: 1

// Test 2: Actual IOIN Value
volatile uint8_t test2_enn = 0;              // Expected: 1
volatile uint8_t test2_ms1 = 0;              // Expected: 1
volatile uint8_t test2_ms2 = 0;              // Expected: 0
volatile uint8_t test2_microsteps = 0;       // Expected: 2
volatile uint8_t test2_pass = 0;             // Expected: 1

// Test 3: GCONF Functions
volatile uint32_t test3_set_result = 0;      // Expected: 0x00000004
volatile uint32_t test3_clear_result = 0;    // Expected: 0xFFFFFFFB
volatile uint8_t test3_read1 = 0;            // Expected: 1
volatile uint8_t test3_read2 = 0;            // Expected: 0
volatile uint8_t test3_pass = 0;             // Expected: 1

// Test 4: IHOLD_IRUN Individual Fields
volatile uint32_t test4_set_irun = 0;        // Expected: 0x00001400
volatile uint32_t test4_set_ihold = 0;       // Expected: 0x0000140A
volatile uint8_t test4_get_irun = 0;         // Expected: 20
volatile uint8_t test4_get_ihold = 0;        // Expected: 10
volatile uint8_t test4_pass = 0;             // Expected: 1

// Test 5: IOIN Parse Struct
volatile uint8_t test5_pins_enn = 0;         // Expected: 1
volatile uint8_t test5_pins_ms1 = 0;         // Expected: 1
volatile uint8_t test5_pins_ms2 = 0;         // Expected: 0
volatile uint8_t test5_pins_diag = 0;        // Expected: 0
volatile uint8_t test5_pass = 0;             // Expected: 1

// Test 6: Current Build/Parse with Struct
volatile uint32_t test6_build_result = 0;    // Expected: 0x0006140A
volatile uint8_t test6_parsed_irun = 0;      // Expected: 20
volatile uint8_t test6_parsed_ihold = 0;     // Expected: 10
volatile uint8_t test6_parsed_delay = 0;     // Expected: 6
volatile uint8_t test6_pass = 0;             // Expected: 1

// Test 7: Real Hardware
volatile uint32_t test7_ioin_value = 0;      // Actual IOIN
volatile uint8_t test7_microsteps = 0;       // Expected: 2, 4, 8, or 16
volatile uint8_t test7_comm_ok = 0;          // Expected: 1

// ========================================================================
// TEST FUNCTIONS
// ========================================================================

/**
 * @brief Test 1: Basic pin reading with known values
 * Tests TMC2208_IOIN_ReadPin() function
 */
//void Test_BasicPinReading(void) {
//    uint32_t test_value;
//
//    // Test bit 0 (ENN) - set
//    test_value = 0x00000001;
//    test1_bit0_set = TMC2208_IOIN_ReadPin(test_value, TMC2208_IOIN_PIN_ENN);
//
//    // Test bit 0 (ENN) - clear
//    test_value = 0x00000000;
//    test1_bit0_clear = TMC2208_IOIN_ReadPin(test_value, TMC2208_IOIN_PIN_ENN);
//
//    // Test bit 2 (MS1) - set
//    test_value = 0x00000004;
//    test1_bit2_set = TMC2208_IOIN_ReadPin(test_value, TMC2208_IOIN_PIN_MS1);
//
//    // Test bit 4 (DIAG) - set
//    test_value = 0x00000010;
//    test1_bit4_set = TMC2208_IOIN_ReadPin(test_value, TMC2208_IOIN_PIN_DIAG);
//
//    // Overall pass check
//    test1_pass = (test1_bit0_set == 1) &&
//                 (test1_bit0_clear == 0) &&
//                 (test1_bit2_set == 1) &&
//                 (test1_bit4_set == 1);
//
//    asm("NOP");  // Breakpoint: check test1_pass = 1
//}


void Test_ActualIOIN(void) {
//    uint32_t ioin = 0x00060505;  // Your actual IOIN value

    // Read individual pins
    test2_enn = TMC2208_IOIN_ReadPin(TMC2208_IOIN_PIN_ENN);
    test2_ms1 = TMC2208_IOIN_ReadPin(TMC2208_IOIN_PIN_MS1);
    test2_ms2 = TMC2208_IOIN_ReadPin(TMC2208_IOIN_PIN_MS2);

    // Get microstep setting
    //test2_microsteps = TMC2208_IOIN_GetMicrostepSetting(ioin);

    // Pass check
    test2_pass = (test2_enn == 1) &&
                 (test2_ms1 == 1) &&
                 (test2_ms2 == 0) &&
                 (test2_microsteps == 2);

    asm("NOP");  // Breakpoint: check test2_pass = 1
}

/**
 * @brief Test 3: GCONF register bit manipulation
 * Tests TMC2208_GCONF_SetBit(), ClearBit(), ReadBit()
 */
void Test_GCONF_Functions(void) {
    uint32_t gconf;

    // Test SetBit
    gconf = 0x00000000;
    gconf = TMC2208_GCONF_SetBit(gconf, TMC2208_GCONF_EN_SPREADCYCLE);
    test3_set_result = gconf;  // Should be 0x00000004

    // Test ClearBit
    gconf = 0xFFFFFFFF;
    gconf = TMC2208_GCONF_ClearBit(gconf, TMC2208_GCONF_EN_SPREADCYCLE);
    test3_clear_result = gconf;  // Should be 0xFFFFFFFB

    // Test ReadBit
    gconf = 0x00000004;  // Only SPREADCYCLE bit set
    test3_read1 = TMC2208_GCONF_ReadBit(gconf, TMC2208_GCONF_EN_SPREADCYCLE);
    test3_read2 = TMC2208_GCONF_ReadBit(gconf, TMC2208_GCONF_SHAFT);

    // Pass check
    test3_pass = (test3_set_result == 0x00000004) &&
                 (test3_clear_result == 0xFFFFFFFB) &&
                 (test3_read1 == 1) &&
                 (test3_read2 == 0);

    asm("NOP");  // Breakpoint: check test3_pass = 1
}

/**
 * @brief Test 4: IHOLD_IRUN individual field operations
 * Tests TMC2208_Current_SetField() and GetField()
 */
//void Test_IHOLD_IRUN_Functions(void) {
//    uint32_t reg_value;
//
//    // Test SetField - IRUN
//    reg_value = 0x00000000;
//    reg_value = TMC2208_Current_SetField(reg_value, TMC2208_CURRENT_IRUN, 20);
//    test4_set_irun = reg_value;  // Should be 0x00001400
//
//    // Test SetField - IHOLD
//    reg_value = TMC2208_Current_SetField(reg_value, TMC2208_CURRENT_IHOLD, 10);
//    test4_set_ihold = reg_value;  // Should be 0x0000140A
//
//    // Test GetField
//    reg_value = 0x0006140A;
//    test4_get_irun = TMC2208_Current_GetField(reg_value, TMC2208_CURRENT_IRUN);
//    test4_get_ihold = TMC2208_Current_GetField(reg_value, TMC2208_CURRENT_IHOLD);
//
//    // Pass check
//    test4_pass = (test4_set_irun == 0x00001400) &&
//                 (test4_set_ihold == 0x0000140A) &&
//                 (test4_get_irun == 20) &&
//                 (test4_get_ihold == 10);
//
//    asm("NOP");  // Breakpoint: check test4_pass = 1
//}
//
///**
// * @brief Test 5: IOIN parse into struct
// * Tests TMC2208_IOIN_ParsePins()
// */
//void Test_IOIN_ParseStruct(void) {
//    uint32_t ioin = 0x00060505;
//    TMC2208_IOIN_Pins_t pins;
//
//    // Parse IOIN into structure
//    //TMC2208_IOIN_ParsePins(ioin, &pins);
//
//    // Copy to volatile for debugging
//    test5_pins_enn = pins.enn;
//    test5_pins_ms1 = pins.ms1;
//    test5_pins_ms2 = pins.ms2;
//    test5_pins_diag = pins.diag;
//
//    // Pass check
//    test5_pass = (test5_pins_enn == 1) &&
//                 (test5_pins_ms1 == 1) &&
//                 (test5_pins_ms2 == 0) &&
//                 (test5_pins_diag == 0);
//
//    asm("NOP");  // Breakpoint: check test5_pass = 1
//}

/**
 * @brief Test 6: Current Build and Parse with structures
 * Tests TMC2208_Current_Build() and TMC2208_Current_Parse()
 */
//void Test_Current_BuildParse(void) {
//    // Build from struct
//    TMC2208_CurrentConfig_t config_build = {
//        .ihold = 10,
//        .irun = 20,
//        .iholddelay = 6
//    };
//
//    //test6_build_result = TMC2208_Current_Build(&config_build);
//
//    // Parse back to struct
//    TMC2208_CurrentConfig_t config_parse;
//    //TMC2208_Current_Parse(0x0006140A, &config_parse);
//
//    test6_parsed_irun = config_parse.irun;
//    test6_parsed_ihold = config_parse.ihold;
//    test6_parsed_delay = config_parse.iholddelay;
//
//    // Pass check
//    test6_pass = (test6_build_result == 0x0006140A) &&
//                 (test6_parsed_irun == 20) &&
//                 (test6_parsed_ihold == 10) &&
//                 (test6_parsed_delay == 6);
//
//    asm("NOP");  // Breakpoint: check test6_pass = 1
//}

/**
 * @brief Test 7: Real hardware communication
 * Tests with actual TMC2208 hardware
 * WARNING: Requires TMC2208 connected and UART working
 */
//void Test_RealHardware(void) {
//    uint32_t ioin;
//
//    // Try to read IOIN register
//    if (TMC2208_ReadReg(TMC2208_REG_IOIN, &ioin)) {
//        test7_comm_ok = 1;
//        test7_ioin_value = ioin;
//
//        // Get microstep setting
//        //test7_microsteps = TMC2208_IOIN_GetMicrostepSetting(ioin);
//
//        // Parse into struct for detailed info
//        TMC2208_IOIN_Pins_t pins;
//        //TMC2208_IOIN_ParsePins(ioin, &pins);
//
//        // Store in volatile variables for debugging
////        volatile uint8_t hw_enn = pins.enn;
////        volatile uint8_t hw_ms1 = pins.ms1;
////        volatile uint8_t hw_ms2 = pins.ms2;
////        volatile uint8_t hw_diag = pins.diag;
//
//        asm("NOP");  // Breakpoint: inspect hardware pin states
//
//    } else {
//        test7_comm_ok = 0;
//    }
//
//    asm("NOP");  // Breakpoint: check test7_comm_ok = 1
//}

/**
 * @brief Run all tests sequentially
 */
void RunAllTests(void) {
    //Test_BasicPinReading();
    Test_ActualIOIN();
    Test_GCONF_Functions();
    //Test_IHOLD_IRUN_Functions();
    //Test_IOIN_ParseStruct();
    //Test_Current_BuildParse();

    // Uncomment when hardware is connected:
    // Test_RealHardware();
}
