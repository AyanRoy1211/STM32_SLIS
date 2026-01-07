#include "TMC2208_Core.h"

// ========================================================================
// GCONF REGISTER FUNCTIONS
// ========================================================================

uint32_t TMC2208_Core_SetGCONFBit(uint32_t reg_value, uint32_t bit_mask) {
    return reg_value | bit_mask;
}

uint32_t TMC2208_Core_ClearGCONFBit(uint32_t reg_value, uint32_t bit_mask) {
    return reg_value & ~bit_mask;
}

uint32_t TMC2208_Core_ToggleGCONFBit(uint32_t reg_value, uint32_t bit_mask) {
    return reg_value ^ bit_mask;
}

uint32_t TMC2208_Core_WriteGCONFBit(uint32_t reg_value, uint32_t bit_mask, uint8_t value) {
    if (value) {
        return reg_value | bit_mask;
    } else {
        return reg_value & ~bit_mask;
    }
}

uint8_t TMC2208_Core_ReadGCONFBit(uint32_t reg_value, uint32_t bit_mask) {
    return (reg_value & bit_mask) ? 1 : 0;
}

// ========================================================================
// IOIN REGISTER FUNCTIONS
// ========================================================================

uint8_t TMC2208_Core_ReadPin(uint32_t ioin_value, TMC2208_Pin_t pin) {
    // Right shift method - most reliable
    return (uint8_t)((ioin_value >> pin) & 0x01);
}

uint8_t TMC2208_Core_GetVersion(uint32_t ioin_value) {
    return (uint8_t)((ioin_value & IOIN_VERSION_MASK) >> IOIN_VERSION_SHIFT);
}

uint8_t TMC2208_Core_GetMicrostepSetting(uint32_t ioin_value) {
    uint8_t ms1 = (ioin_value & IOIN_MS1) ? 1 : 0;
    uint8_t ms2 = (ioin_value & IOIN_MS2) ? 1 : 0;

    // Decode according to TMC2208 datasheet
    // MS2  MS1  Result
    //  0    0   8 microsteps
    //  0    1   2 microsteps
    //  1    0   4 microsteps
    //  1    1   16 microsteps

    if (!ms2 && !ms1) return 8;
    if (!ms2 &&  ms1) return 2;
    if ( ms2 && !ms1) return 4;
    if ( ms2 &&  ms1) return 16;

    return 0;  // Should never reach here
}

void TMC2208_Core_GetMicrostepPins(uint32_t ioin_value, uint8_t *ms1, uint8_t *ms2) {
    *ms1 = (ioin_value & IOIN_MS1) ? 1 : 0;
    *ms2 = (ioin_value & IOIN_MS2) ? 1 : 0;
}

// ========================================================================
// IHOLD_IRUN REGISTER FUNCTIONS
// ========================================================================

uint32_t TMC2208_Core_SetCurrentField(uint32_t reg_value, TMC2208_CurrentField_t field, uint8_t value) {
    switch (field) {
        case TMC2208_IHOLD:
            // Validate range (0-31)
            if (value > 31) {
                return reg_value;  // Invalid, return unchanged
            }
            // Clear old value and set new
            reg_value &= ~IHOLD_MASK;
            reg_value |= ((uint32_t)value << IHOLD_SHIFT);
            break;

        case TMC2208_IRUN:
            // Validate range (0-31)
            if (value > 31) {
                return reg_value;  // Invalid, return unchanged
            }
            // Clear old value and set new
            reg_value &= ~IRUN_MASK;
            reg_value |= ((uint32_t)value << IRUN_SHIFT);
            break;

        case TMC2208_IHOLDDELAY:
            // Validate range (0-15)
            if (value > 15) {
                return reg_value;  // Invalid, return unchanged
            }
            // Clear old value and set new
            reg_value &= ~IHOLDDELAY_MASK;
            reg_value |= ((uint32_t)value << IHOLDDELAY_SHIFT);
            break;

        default:
            return reg_value;  // Invalid field, return unchanged
    }

    return reg_value;
}

uint8_t TMC2208_Core_GetCurrentField(uint32_t reg_value, TMC2208_CurrentField_t field) {
    switch (field) {
        case TMC2208_IHOLD:
            return (uint8_t)((reg_value & IHOLD_MASK) >> IHOLD_SHIFT);

        case TMC2208_IRUN:
            return (uint8_t)((reg_value & IRUN_MASK) >> IRUN_SHIFT);

        case TMC2208_IHOLDDELAY:
            return (uint8_t)((reg_value & IHOLDDELAY_MASK) >> IHOLDDELAY_SHIFT);

        default:
            return 0;
    }
}

uint32_t TMC2208_Core_BuildCurrents(uint8_t irun, uint8_t ihold, uint8_t iholddelay) {
    // Clamp values to valid ranges
    if (irun > 31) irun = 31;
    if (ihold > 31) ihold = 31;
    if (iholddelay > 15) iholddelay = 15;

    return ((uint32_t)ihold << IHOLD_SHIFT) |
           ((uint32_t)irun << IRUN_SHIFT) |
           ((uint32_t)iholddelay << IHOLDDELAY_SHIFT);
}

void TMC2208_Core_ParseCurrents(uint32_t reg_value, uint8_t *irun, uint8_t *ihold, uint8_t *iholddelay) {
    *ihold = (uint8_t)((reg_value & IHOLD_MASK) >> IHOLD_SHIFT);
    *irun = (uint8_t)((reg_value & IRUN_MASK) >> IRUN_SHIFT);
    *iholddelay = (uint8_t)((reg_value & IHOLDDELAY_MASK) >> IHOLDDELAY_SHIFT);
}
