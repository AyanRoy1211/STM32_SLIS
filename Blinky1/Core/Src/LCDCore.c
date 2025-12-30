#include "LCDCore.h"

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

	LCD_SetCursor(0, 0);
	LCD_Print("Mode: IDLE      ");
	LCD_SetCursor(1, 0);
	LCD_Print("LED: OFF        ");
}
