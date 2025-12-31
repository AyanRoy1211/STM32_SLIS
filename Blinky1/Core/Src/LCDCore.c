#include "LCDCore.h"
#include "LCD.h"
void LCD_Init(void)
{
	HAL_Delay(50);
	LCD_Command(0x33);
	HAL_Delay(5);
	LCD_Command(0x32);
	HAL_Delay(5);
	LCD_Command(0x28);
	LCD_Command(0x0C);
	LCD_Command(0x06);
	LCD_Command(0x01);
	HAL_Delay(5);
}

void LCD_Driver_Command(uint8_t cmd)
{
	LCD_Command(cmd);
}
void LCD_Driver_Data(uint8_t data){
	  LCD_Data(data);
}
void LCD_Driver_SetCursor(uint8_t row, uint8_t col)
{
    uint8_t addr;
    if (row == 0)
        addr = 0x80 + col;
    else
        addr = 0xC0 + col;

    LCD_Command(addr);
}

void LCD_Driver_Print(const char *str)
{
    while (*str)
        LCD_Data(*str++);
}

void LCD_Driver_Clear(void)
{
    LCD_Command(0x01);
    HAL_Delay(2);
}
