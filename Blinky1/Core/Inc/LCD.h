#ifndef __LCD_H
#define __LCD_H

#ifdef __cplusplus
extern "C" {
#endif

#include "main.h"
#include "stdio.h"
#include "strings.h"

void LCD_Command(uint8_t cmd);
void LCD_Data(uint8_t data);

#ifdef __cplusplus
}
#endif

#endif /* __LCD_H */
