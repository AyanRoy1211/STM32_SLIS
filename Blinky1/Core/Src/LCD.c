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

/* Public functions */

void LCD_Init(void)
{
    HAL_Delay(50);

    LCD_Command(0x33);
    HAL_Delay(5);
    LCD_Command(0x32);
    HAL_Delay(5);
    LCD_Command(0x28);   /* 4-bit, 2 line, 5x8 dots */
    LCD_Command(0x0C);   /* Display ON, Cursor OFF */
    LCD_Command(0x06);   /* Entry mode: increment cursor */
    LCD_Command(0x01);   /* Clear display */
    HAL_Delay(5);
}

//void LCD_Clear(void)
//{
//    LCD_Command(0x01);
//    HAL_Delay(2);
//}

void LCD_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr;
    if (row == 0)
        addr = 0x80 + col;
    else
        addr = 0xC0 + col;

    LCD_Command(addr);
}

void LCD_Print(char *str)
{
    while (*str)
        LCD_Data(*str++);
}

//void LCD_PrintChar(char c)
//{
//    LCD_Data(c);
//}
