#include "LCD.h"

void LCD_EnablePulse(void)
{
    HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_SET);
    HAL_Delay(1);
    HAL_GPIO_WritePin(E_GPIO_Port, E_Pin, GPIO_PIN_RESET);
    HAL_Delay(1);
}

void LCD_Send4Bit(uint8_t data)
{
    HAL_GPIO_WritePin(DB4_GPIO_Port, DB4_Pin, (data & 0x01) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DB5_GPIO_Port, DB5_Pin, (data & 0x02) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DB6_GPIO_Port, DB6_Pin, (data & 0x04) ? GPIO_PIN_SET : GPIO_PIN_RESET);
    HAL_GPIO_WritePin(DB7_GPIO_Port, DB7_Pin, (data & 0x08) ? GPIO_PIN_SET : GPIO_PIN_RESET);

    LCD_EnablePulse();
}

void LCD_Command(uint8_t cmd)
{
    HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_RESET);

    LCD_Send4Bit(cmd >> 4);
    LCD_Send4Bit(cmd & 0x0F);

    HAL_Delay(2);
}

void LCD_Data(uint8_t data)
{
    HAL_GPIO_WritePin(RS_GPIO_Port, RS_Pin, GPIO_PIN_SET);

    LCD_Send4Bit(data >> 4);
    LCD_Send4Bit(data & 0x0F);

    HAL_Delay(2);
}
