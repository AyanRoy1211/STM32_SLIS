#ifndef TMC2208_TEST_H_
#define TMC2208_TEST_H_

#include <stdint.h>

// Test function declarations
void Test_BasicPinReading(void);
void Test_ActualIOIN(void);
void Test_GCONF_Functions(void);
void Test_IHOLD_IRUN_Functions(void);
void Test_RealHardware(void);
void RunAllTests(void);

#endif /* TMC2208_TEST_H_ */
