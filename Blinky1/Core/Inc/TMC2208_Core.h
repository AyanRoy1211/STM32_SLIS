#ifndef TMC2208_CORE_H_
#define TMC2208_CORE_H_

#include <stdint.h>
#include <stdbool.h>
#include "stddef.h"

// ========================================================================
// REGISTER MAP
// ========================================================================

typedef enum {
    TMC2208_REG_GCONF = 0x00,
    TMC2208_REG_GSTAT = 0x01,
    TMC2208_REG_IFCNT = 0x02,
    TMC2208_REG_IOIN = 0x06,
    TMC2208_REG_IHOLD_IRUN = 0x10,
    TMC2208_REG_TPOWERDOWN = 0x11,
    TMC2208_REG_TSTEP = 0x12,
    TMC2208_REG_TPWMTHRS = 0x13,
    TMC2208_REG_VACTUAL = 0x22,
    TMC2208_REG_MSCNT = 0x6A,
    TMC2208_REG_MSCURACT = 0x6B,
    TMC2208_REG_CHOPCONF = 0x6C,
    TMC2208_REG_DRVSTATUS = 0x6F,
    TMC2208_REG_PWMCONF = 0x70,
    TMC2208_REG_PWMSCALE = 0x71,
    TMC2208_REG_PWMAUTO = 0x72
} TMC2208_Register_t;

// ========================================================================
// GCONF REGISTER (0x00)
// ========================================================================

typedef enum {
    TMC2208_GCONF_I_SCALE_ANALOG = 0,   // Bit 0: Use VREF for current scaling
    TMC2208_GCONF_INTERNAL_RSENSE = 1,  // Bit 1: Use internal sense resistors
    TMC2208_GCONF_EN_SPREADCYCLE = 2,   // Bit 2: Enable SpreadCycle mode
    TMC2208_GCONF_SHAFT = 3,            // Bit 3: Inverse motor direction
    TMC2208_GCONF_INDEX_OTPW = 4,       // Bit 4: INDEX shows overtemp warning
    TMC2208_GCONF_INDEX_STEP = 5,       // Bit 5: INDEX shows step pulses
    TMC2208_GCONF_PDN_DISABLE = 6,      // Bit 6: Disable PDN_UART function
    TMC2208_GCONF_MSTEP_REG_SELECT = 7, // Bit 7: Microstep from register
    TMC2208_GCONF_MULTISTEP_FILT = 8,   // Bit 8: Enable step pulse filtering
    TMC2208_GCONF_TEST_MODE = 9         // Bit 9: Enable test mode
} TMC2208_GCONF_Bit_t;

// ========================================================================
// IOIN REGISTER (0x06) - Read Only
// ========================================================================

typedef enum {
    TMC2208_IOIN_PIN_ENN = 0,       // Bit 0: Enable pin state
    TMC2208_IOIN_PIN_MS1 = 2,       // Bit 2: Microstep select 1
    TMC2208_IOIN_PIN_MS2 = 3,       // Bit 3: Microstep select 2
    TMC2208_IOIN_PIN_DIAG = 4,      // Bit 4: Diagnostic output
    TMC2208_IOIN_PIN_PDN_UART = 6,  // Bit 6: Power down/UART pin
    TMC2208_IOIN_PIN_STEP = 7,      // Bit 7: Step input
    TMC2208_IOIN_PIN_SEL_A = 8,     // Bit 8: Driver type select
    TMC2208_IOIN_PIN_DIR = 9        // Bit 9: Direction input
} TMC2208_IOIN_Pin_t;

typedef struct {
    uint8_t enn;          // Enable pin state (0=enabled, 1=disabled)
    uint8_t ms1;          // Microstep select 1
    uint8_t ms2;          // Microstep select 2
    uint8_t diag;         // Diagnostic flag
    uint8_t pdn_uart;     // PDN_UART pin state
    uint8_t step;         // Step input state
    uint8_t dir;          // Direction input state
} TMC2208_IOIN_Pins_t;

// ========================================================================
// IHOLD_IRUN REGISTER (0x10)
// ========================================================================

typedef enum {
    TMC2208_CURRENT_IHOLD = 0,      // Standstill current (bits 4:0)
    TMC2208_CURRENT_IRUN = 1,       // Run current (bits 12:8)
    TMC2208_CURRENT_IHOLDDELAY = 2  // Power down delay (bits 19:16)
} TMC2208_CurrentField_t;

typedef struct {
    uint8_t ihold;          // Standstill current (0-31)
    uint8_t irun;           // Run current (0-31)
    uint8_t iholddelay;     // Power down delay (0-15)
} TMC2208_CurrentConfig_t;

// ========================================================================
// REGISTER BIT MASKS
// ========================================================================

// GCONF bit masks
#define TMC2208_GCONF_MASK(bit) (1UL << (bit))

// IOIN version field
#define TMC2208_IOIN_VERSION_SHIFT  24
#define TMC2208_IOIN_VERSION_MASK   (0xFFUL << TMC2208_IOIN_VERSION_SHIFT)

// IHOLD_IRUN field masks
#define TMC2208_IHOLD_MASK          0x0000001FUL
#define TMC2208_IRUN_MASK           0x00001F00UL
#define TMC2208_IHOLDDELAY_MASK     0x000F0000UL
#define TMC2208_IHOLD_SHIFT         0
#define TMC2208_IRUN_SHIFT          8
#define TMC2208_IHOLDDELAY_SHIFT    16

// ========================================================================
// GCONF REGISTER FUNCTIONS
// ========================================================================

/**
 * @brief Set bit in GCONF register
 * @param reg_value Current register value
 * @param bit Bit position (use TMC2208_GCONF_xxx enum)
 * @return Modified register value
 */
uint32_t TMC2208_GCONF_SetBit(uint32_t reg_value, TMC2208_GCONF_Bit_t bit);

/**
 * @brief Clear bit in GCONF register
 * @param reg_value Current register value
 * @param bit Bit position (use TMC2208_GCONF_xxx enum)
 * @return Modified register value
 */
uint32_t TMC2208_GCONF_ClearBit(uint32_t reg_value, TMC2208_GCONF_Bit_t bit);

/**
 * @brief Write bit in GCONF register
 * @param reg_value Current register value
 * @param bit Bit position (use TMC2208_GCONF_xxx enum)
 * @param value 1 to set, 0 to clear
 * @return Modified register value
 */
uint32_t TMC2208_GCONF_WriteBit(uint32_t reg_value, TMC2208_GCONF_Bit_t bit, bool value);

/**
 * @brief Read bit from GCONF register
 * @param reg_value Register value
 * @param bit Bit position (use TMC2208_GCONF_xxx enum)
 * @return Bit value (0 or 1)
 */
uint8_t TMC2208_GCONF_ReadBit(uint32_t reg_value, TMC2208_GCONF_Bit_t bit);

// ========================================================================
// IOIN REGISTER FUNCTIONS
// ========================================================================

/**
 * @brief Read single pin from IOIN register
 * @param ioin_value IOIN register value
 * @param pin Pin to read (use TMC2208_IOIN_PIN_xxx enum)
 * @return Pin state (0 or 1)
 */
uint8_t TMC2208_IOIN_ReadPin(uint32_t ioin_value, TMC2208_IOIN_Pin_t pin);

/**
 * @brief Parse IOIN register into pin structure
 * @param ioin_value IOIN register value
 * @param pins Pointer to store parsed pin states
 */
void TMC2208_IOIN_ParsePins(uint32_t ioin_value, TMC2208_IOIN_Pins_t *pins);

/**
 * @brief Get microstep setting from MS1/MS2 pins
 * @param ioin_value IOIN register value
 * @return Microstep value (2, 4, 8, or 16)
 */
uint8_t TMC2208_IOIN_GetMicrostepSetting(uint32_t ioin_value);

// ========================================================================
// IHOLD_IRUN REGISTER FUNCTIONS
// ========================================================================

/**
 * @brief Build IHOLD_IRUN register from structure
 * @param config Pointer to current configuration
 * @return Complete register value
 */
uint32_t TMC2208_Current_Build(const TMC2208_CurrentConfig_t *config);

/**
 * @brief Parse IHOLD_IRUN register into structure
 * @param reg_value Register value
 * @param config Pointer to store parsed configuration
 */
void TMC2208_Current_Parse(uint32_t reg_value, TMC2208_CurrentConfig_t *config);

/**
 * @brief Set individual current field
 * @param reg_value Current register value
 * @param field Field to modify (use TMC2208_CURRENT_xxx enum)
 * @param value Value to set (0-31 for currents, 0-15 for delay)
 * @return Modified register value
 */
uint32_t TMC2208_Current_SetField(uint32_t reg_value, TMC2208_CurrentField_t field, uint8_t value);

/**
 * @brief Get individual current field
 * @param reg_value Register value
 * @param field Field to read (use TMC2208_CURRENT_xxx enum)
 * @return Field value
 */
uint8_t TMC2208_Current_GetField(uint32_t reg_value, TMC2208_CurrentField_t field);

// ========================================================================
// HELPER FUNCTIONS
// ========================================================================

/**
 * @brief Convert microstep enum to actual step count
 * @param ms1 MS1 pin state
 * @param ms2 MS2 pin state
 * @return Microstep count (2, 4, 8, or 16)
 */
uint8_t TMC2208_Helper_PinsToMicrosteps(uint8_t ms1, uint8_t ms2);

#endif /* TMC2208_CORE_H_ */
