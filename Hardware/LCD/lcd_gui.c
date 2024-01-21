//
// Created by FairyTale on 2024/1/21.
//

#include "lcd_gui.h"
#include "lcd.h"
#include "debug.h"

uint8_t LCDReverse = 0;
uint8_t LCDOverlap = 1;

uint8_t DisplayBuf[LCD_H * LCD_W / 30];
/*
 *  1 2 3 4 5 6 7 8 9 ... 240
 *  2
 *  3
 *  4
 *  5
 *  6
 *  7
 *  8
 *  9
 *  .
 *  .
 *  .
 *  135
 * */

void SwapNumber(uint8_t *number1, uint8_t *number2)
{
    uint8_t t = *number1;
    *number1 = *number2;
    *number2 = t;
}

void LCD_BUF_Fill(uint8_t xStar, uint8_t yStar, uint8_t xEnd, uint8_t yEnd, uint16_t Color)
{
    uint16_t x, y;

    if(xStar > xEnd)
    {
        SwapNumber(&xEnd, &xStar);
    }
    if(yStar > yEnd)
    {
        SwapNumber(&yEnd, &yStar);
    }
    for(y = yStar; y < yEnd; y++)
    {
        for(x = xStar; x < xEnd; x++)
        {
            DisplayBuf[x + y * LCD_W] = Color;
        }
    }
}

void LCD_GUI_Refresh(void)
{
    SPI2_Write_DMA(DisplayBuf, LCD_H * LCD_W);
}










