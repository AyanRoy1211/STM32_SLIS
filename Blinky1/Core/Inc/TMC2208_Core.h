#ifndef TMC2208_CORE_H_
#define TMC2208_CORE_H_

#include <stdint.h>
#include <stdbool.h>

// ========== Register Addresses ==========
#define TMC2208_REG_GCONF        0x00
#define TMC2208_REG_IOIN         0x06
#define TMC2208_REG_IHOLD_IRUN   0x10

// ========== GCONF Register Bit Definitions ==========
#define GCONF_I_SCALE_ANALOG     (1UL << 0)   // 0=Internal ref, 1=VREF pin
#define GCONF_INTERNAL_RSENSE    (1UL << 1)   // 0=External sense, 1=Internal
#define GCONF_EN_SPREADCYCLE     (1UL << 2)   // 0=StealthChop, 1=SpreadCycle
#define GCONF_SHAFT              (1UL << 3)   // 1=Inverse motor direction
#define GCONF_INDEX_OTPW         (1UL << 4)   // INDEX shows OTPW flag
#define GCONF_INDEX_STEP         (1UL << 5)   // INDEX shows step pulses
#define GCONF_PDN_DISABLE        (1UL << 6)   // Disable PDN_UART function
#define GCONF_MSTEP_REG_SELECT   (1UL << 7)   // Select MRES by register
#define GCONF_MULTISTEP_FILT     (1UL << 8)   // Step pulse filtering
#define GCONF_TEST_MODE          (1UL << 9)   // Analog test mode

// ========== IOIN Register Bit Definitions ==========
#define IOIN_ENN                 (1UL << 0)   // Enable pin state
#define IOIN_MS1                 (1UL << 2)   // Microstep select 1
#define IOIN_MS2                 (1UL << 3)   // Microstep select 2
#define IOIN_DIAG                (1UL << 4)   // Diagnostic output
#define IOIN_PDN_UART            (1UL << 6)   // Power down/UART pin
#define IOIN_STEP                (1UL << 7)   // Step input
#define IOIN_SEL_A               (1UL << 8)   // Driver type
#define IOIN_DIR                 (1UL << 9)   // Direction input
#define IOIN_VERSION_SHIFT       24
#define IOIN_VERSION_MASK        (0xFFUL << IOIN_VERSION_SHIFT)

// ========== IHOLD_IRUN Register Field Definitions ==========
#define IHOLD_MASK               0x0000001FUL  // Bits 4:0
#define IRUN_MASK                0x00001F00UL  // Bits 12:8
#define IHOLDDELAY_MASK          0x000F0000UL  // Bits 19:16
#define IHOLD_SHIFT              0
#define IRUN_SHIFT               8
#define IHOLDDELAY_SHIFT         16

// ========== Enumerations ==========

/**
 * @brief IOIN Pin enumeration
 * Pin positions in IOIN register
 */
typedef enum {
    TMC2208_PIN_ENN = 0,        // Enable pin (bit 0)
    TMC2208_PIN_MS1 = 2,        // Microstep select 1 (bit 2)
    TMC2208_PIN_MS2 = 3,        // Microstep select 2 (bit 3)
    TMC2208_PIN_DIAG = 4,       // Diagnostic output (bit 4)
    TMC2208_PIN_PDN_UART = 6,   // Power down/UART (bit 6)
    TMC2208_PIN_STEP = 7,       // Step input (bit 7)
    TMC2208_PIN_SEL_A = 8,      // Driver type select (bit 8)
    TMC2208_PIN_DIR = 9         // Direction input (bit 9)
} TMC2208_Pin_t;

/**
 * @brief IHOLD_IRUN Field enumeration
 * Current control fields
 */
typedef enum {
    TMC2208_IHOLD = 0,          // Standstill current (0-31)
    TMC2208_IRUN = 1,           // Run current (0-31)
    TMC2208_IHOLDDELAY = 2      // Power down delay (0-15)
} TMC2208_CurrentField_t;

// ========================================================================
// GCONF REGISTER FUNCTIONS
// ========================================================================

/**
 * @brief Set bit(s) in GCONF register value
 * @param reg_value Current GCONF register value
 * @param bit_mask Bit mask to set (use GCONF_xxx defines)
 * @return Modified register value
 */
uint32_t TMC2208_Core_SetGCONFBit(uint32_t reg_value, uint32_t bit_mask);

/**
 * @brief Clear bit(s) in GCONF register value
 * @param reg_value Current GCONF register value
 * @param bit_mask Bit mask to clear (use GCONF_xxx defines)
 * @return Modified register value
 */
uint32_t TMC2208_Core_ClearGCONFBit(uint32_t reg_value, uint32_t bit_mask);

/**
 * @brief Toggle bit(s) in GCONF register value
 * @param reg_value Current GCONF register value
 * @param bit_mask Bit mask to toggle (use GCONF_xxx defines)
 * @return Modified register value
 */
uint32_t TMC2208_Core_ToggleGCONFBit(uint32_t reg_value, uint32_t bit_mask);

/**
 * @brief Write bit(s) in GCONF register value
 * @param reg_value Current GCONF register value
 * @param bit_mask Bit mask to modify (use GCONF_xxx defines)
 * @param value 1 to set, 0 to clear
 * @return Modified register value
 */
uint32_t TMC2208_Core_WriteGCONFBit(uint32_t reg_value, uint32_t bit_mask, uint8_t value);

/**
 * @brief Read bit(s) from GCONF register value
 * @param reg_value Current GCONF register value
 * @param bit_mask Bit mask to read (use GCONF_xxx defines)
 * @return 1 if any masked bit is set, 0 otherwise
 */
uint8_t TMC2208_Core_ReadGCONFBit(uint32_t reg_value, uint32_t bit_mask);

// ========================================================================
// IOIN REGISTER FUNCTIONS
// ========================================================================

/**
 * @brief Read individual pin state from IOIN register
 * @param ioin_value IOIN register value
 * @param pin Pin to read (use TMC2208_PIN_xxx enum)
 * @return 1 if pin is HIGH, 0 if pin is LOW
 */
uint8_t TMC2208_Core_ReadPin(uint32_t ioin_value, TMC2208_Pin_t pin);

/**
 * @brief Get IC version from IOIN register
 * @param ioin_value IOIN register value
 * @return Version byte (0x20 for TMC2208)
 */
uint8_t TMC2208_Core_GetVersion(uint32_t ioin_value);

/**
 * @brief Get microstep setting from MS1/MS2 pins
 * @param ioin_value IOIN register value
 * @return Microstep value: 2, 4, 8, or 16
 */
uint8_t TMC2208_Core_GetMicrostepSetting(uint32_t ioin_value);

/**
 * @brief Get both MS1 and MS2 pin states
 * @param ioin_value IOIN register value
 * @param ms1 Pointer to store MS1 state
 * @param ms2 Pointer to store MS2 state
 */
void TMC2208_Core_GetMicrostepPins(uint32_t ioin_value, uint8_t *ms1, uint8_t *ms2);

// ========================================================================
// IHOLD_IRUN REGISTER FUNCTIONS
// ========================================================================

/**
 * @brief Set individual field in IHOLD_IRUN register
 * @param reg_value Current IHOLD_IRUN register value
 * @param field Field to modify (use TMC2208_xxx enum)
 * @param value Value to set (0-31 for IRUN/IHOLD, 0-15 for IHOLDDELAY)
 * @return Modified register value (unchanged if value out of range)
 */
uint32_t TMC2208_Core_SetCurrentField(uint32_t reg_value, TMC2208_CurrentField_t field, uint8_t value);

/**
 * @brief Get individual field from IHOLD_IRUN register
 * @param reg_value Current IHOLD_IRUN register value
 * @param field Field to read (use TMC2208_xxx enum)
 * @return Field value
 */
uint8_t TMC2208_Core_GetCurrentField(uint32_t reg_value, TMC2208_CurrentField_t field);

/**
 * @brief Build IHOLD_IRUN register value from individual fields
 * @param irun Run current (0-31)
 * @param ihold Hold current (0-31)
 * @param iholddelay Power down delay (0-15)
 * @return Complete register value
 */
uint32_t TMC2208_Core_BuildCurrents(uint8_t irun, uint8_t ihold, uint8_t iholddelay);

/**
 * @brief Parse IHOLD_IRUN register into individual fields
 * @param reg_value IHOLD_IRUN register value
 * @param irun Pointer to store run current
 * @param ihold Pointer to store hold current
 * @param iholddelay Pointer to store power down delay
 */
void TMC2208_Core_ParseCurrents(uint32_t reg_value, uint8_t *irun, uint8_t *ihold, uint8_t *iholddelay);

#endif /* TMC2208_CORE_H_ */
