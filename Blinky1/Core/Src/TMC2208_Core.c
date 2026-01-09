#include "TMC2208_Core.h"

uint32_t TMC2208_GCONF_SetBit(uint32_t reg_value, TMC2208_GCONF_Bit_t bit) {
    return reg_value | TMC2208_GCONF_MASK(bit);
}

uint32_t TMC2208_GCONF_ClearBit(uint32_t reg_value, TMC2208_GCONF_Bit_t bit) {
    return reg_value & ~TMC2208_GCONF_MASK(bit);
}

uint32_t TMC2208_GCONF_WriteBit(uint32_t reg_value, TMC2208_GCONF_Bit_t bit, bool value) {
    if (value) {
        return TMC2208_GCONF_SetBit(reg_value, bit);
    } else {
        return TMC2208_GCONF_ClearBit(reg_value, bit);
    }
}

uint8_t TMC2208_GCONF_ReadBit(uint32_t reg_value, TMC2208_GCONF_Bit_t bit) {
    return (reg_value & TMC2208_GCONF_MASK(bit)) ? 1 : 0;
}

uint8_t TMC2208_IOIN_ReadPin(TMC2208_IOIN_Pin_t pin) {
	uint32_t ioin_value = 0;
	TMC2208_ReadReg(0x06, &ioin_value);
    return (uint8_t)((ioin_value >> pin) & 0x01);
}

void TMC2208_IOIN_ParsePins(uint32_t ioin_value, TMC2208_IOIN_Pins_t *pins) {
    if (pins == NULL) {
        return;
    }

    pins->enn = TMC2208_IOIN_ReadPin(TMC2208_IOIN_PIN_ENN);
    pins->ms1 = TMC2208_IOIN_ReadPin(TMC2208_IOIN_PIN_MS1);
    pins->ms2 = TMC2208_IOIN_ReadPin(TMC2208_IOIN_PIN_MS2);
    pins->diag = TMC2208_IOIN_ReadPin(TMC2208_IOIN_PIN_DIAG);
    pins->pdn_uart = TMC2208_IOIN_ReadPin(TMC2208_IOIN_PIN_PDN_UART);
    pins->step = TMC2208_IOIN_ReadPin(TMC2208_IOIN_PIN_STEP);
    pins->dir = TMC2208_IOIN_ReadPin(TMC2208_IOIN_PIN_DIR);
}

uint8_t TMC2208_IOIN_GetMicrostepSetting(uint32_t ioin_value) {
    uint8_t ms1 = TMC2208_IOIN_ReadPin(TMC2208_IOIN_PIN_MS1);
    uint8_t ms2 = TMC2208_IOIN_ReadPin(TMC2208_IOIN_PIN_MS2);

    return TMC2208_Helper_PinsToMicrosteps(ms1, ms2);
}

uint32_t TMC2208_Current_Build(const TMC2208_CurrentConfig_t *config) {
    if (config == NULL) {
        return 0;
    }

    // Clamp values to valid ranges
    uint8_t irun = (config->irun > 31) ? 31 : config->irun;
    uint8_t ihold = (config->ihold > 31) ? 31 : config->ihold;
    uint8_t delay = (config->iholddelay > 15) ? 15 : config->iholddelay;

    return ((uint32_t)ihold << TMC2208_IHOLD_SHIFT) |
           ((uint32_t)irun << TMC2208_IRUN_SHIFT) |
           ((uint32_t)delay << TMC2208_IHOLDDELAY_SHIFT);
}

void TMC2208_Current_Parse(uint32_t reg_value, TMC2208_CurrentConfig_t *config) {
    if (config == NULL) {
        return;
    }

    config->ihold = (uint8_t)((reg_value & TMC2208_IHOLD_MASK) >> TMC2208_IHOLD_SHIFT);
    config->irun = (uint8_t)((reg_value & TMC2208_IRUN_MASK) >> TMC2208_IRUN_SHIFT);
    config->iholddelay = (uint8_t)((reg_value & TMC2208_IHOLDDELAY_MASK) >> TMC2208_IHOLDDELAY_SHIFT);
}

uint32_t TMC2208_Current_SetField(uint32_t reg_value, TMC2208_CurrentField_t field, uint8_t value) {
    switch (field) {
        case TMC2208_CURRENT_IHOLD:
            if (value > 31) return reg_value;
            reg_value &= ~TMC2208_IHOLD_MASK;
            reg_value |= ((uint32_t)value << TMC2208_IHOLD_SHIFT);
            break;

        case TMC2208_CURRENT_IRUN:
            if (value > 31) return reg_value;
            reg_value &= ~TMC2208_IRUN_MASK;
            reg_value |= ((uint32_t)value << TMC2208_IRUN_SHIFT);
            break;

        case TMC2208_CURRENT_IHOLDDELAY:
            if (value > 15) return reg_value;
            reg_value &= ~TMC2208_IHOLDDELAY_MASK;
            reg_value |= ((uint32_t)value << TMC2208_IHOLDDELAY_SHIFT);
            break;

        default:
            break;
    }

    return reg_value;
}

uint8_t TMC2208_Current_GetField(uint32_t reg_value, TMC2208_CurrentField_t field) {
    switch (field) {
        case TMC2208_CURRENT_IHOLD:
            return (uint8_t)((reg_value & TMC2208_IHOLD_MASK) >> TMC2208_IHOLD_SHIFT);

        case TMC2208_CURRENT_IRUN:
            return (uint8_t)((reg_value & TMC2208_IRUN_MASK) >> TMC2208_IRUN_SHIFT);

        case TMC2208_CURRENT_IHOLDDELAY:
            return (uint8_t)((reg_value & TMC2208_IHOLDDELAY_MASK) >> TMC2208_IHOLDDELAY_SHIFT);

        default:
            return 0;
    }
}

// ========================================================================
// HELPER FUNCTIONS
// ========================================================================

uint8_t TMC2208_Helper_PinsToMicrosteps(uint8_t ms1, uint8_t ms2) {
    // TMC2208 microstep decoding table
    // MS2  MS1  Result
    //  0    0   8 microsteps
    //  0    1   2 microsteps
    //  1    0   4 microsteps
    //  1    1   16 microsteps

    if (!ms2 && !ms1) return 8;
    if (!ms2 &&  ms1) return 2;
    if ( ms2 && !ms1) return 4;
    if ( ms2 &&  ms1) return 16;

    return 0;
}
