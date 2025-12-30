#ifndef INC_LCDCORE_H_
#define INC_LCDCORE_H_

#include "LCD.h"

void LCD_SetCursor(uint8_t row, uint8_t col);
void LCD_Print(char *str);
void LCD_Init(void);

#endif /* INC_LCDCORE_H_ */
