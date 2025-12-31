#ifndef INC_LCDCORE_H_
#define INC_LCDCORE_H_

#include "LCD.h"
#include <stdint.h>

void LCD_Init(void);
void LCD_Driver_Commad(uint8_t cmd);
void LCD_Driver_Data(uint8_t data);
void LCD_Driver_SetCursor(uint8_t row, uint8_t col);
void LCD_Driver_Print(const char *str);
void LCD_Driver_Clear(void);

#endif /* INC_LCDCORE_H_ */
