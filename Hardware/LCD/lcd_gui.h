//
// Created by FairyTale on 2024/1/21.
//

#ifndef CH32V203_FREERTOS_HARDWARE_LCD_LCD_GUI_H_
#define CH32V203_FREERTOS_HARDWARE_LCD_LCD_GUI_H_

#include "stdio.h"
#include "stdlib.h"

void SwapNumber(uint8_t *number1, uint8_t *number2);
void LCD_BUF_Fill(uint8_t xStar, uint8_t yStar, uint8_t xEnd, uint8_t yEnd, uint16_t Color);
void LCD_GUI_Refresh(void);

#endif //CH32V203_FREERTOS_HARDWARE_LCD_LCD_GUI_H_
