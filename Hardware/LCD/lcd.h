/*********************************************************************
 * File Name          : lcd.h
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/8/15
 * Description        : file for lcd screen.
 *********************************************************************************
* Copyright (c) 2023 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#ifndef __LCD_H
#define __LCD_H
#include "spi.h"
#include "stdio.h"
#include "stdlib.h"
#include "flash.h"
#include "config.h"

// [15:11] 5bit red
//  [10:5] 6bit green
//   [4:0] 5bit blue
#define LCD_COLOR_WHITE    0xFFFF
#define LCD_COLOR_BLACK    0x0000
#define LCD_COLOR_BLUE     0x001F
#define LCD_COLOR_BRED     0XF81F
#define LCD_COLOR_GRED     0XFFE0
#define LCD_COLOR_GBLUE    0X07FF
#define LCD_COLOR_RED      0xF800
#define LCD_COLOR_MAGENTA  0xF81F
#define LCD_COLOR_GREEN    0x07E0
#define LCD_COLOR_CYAN     0x7FFF
#define LCD_COLOR_YELLOW   0xFFE0
#define LCD_COLOR_BROWN    0XBC40
#define LCD_COLOR_BRRED    0XFC07
#define LCD_COLOR_GRAY     0X8430

#define LCD_COLOR_FONT          LCD_COLOR_WHITE
#define LCD_COLOR_BACKGROUND    LCD_COLOR_BLACK


#define LCD_LED_SET  (LCD_LED_PORT->BSHR = LCD_LED_PIN)
#define LCD_CS_SET   (LCD_CS_PORT->BSHR = LCD_CS_PIN)
#define LCD_DC_SET   (LCD_DC_PORT->BSHR = LCD_DC_PIN)
#define LCD_RST_SET  (LCD_RST_PORT->BSHR = LCD_RST_PIN)

#define LCD_LED_CLR  (LCD_LED_PORT->BCR = LCD_LED_PIN)
#define LCD_CS_CLR   (LCD_CS_PORT->BCR = LCD_CS_PIN)
#define LCD_DC_CLR   (LCD_DC_PORT->BCR = LCD_DC_PIN)
#define LCD_RST_CLR  (LCD_RST_PORT->BCR = LCD_RST_PIN)

void LCD_Init(void);
void LCD_Clear(uint16_t Color);

void Lcd_WriteData_16Bit(uint16_t Data);
void LCD_direction(uint8_t direction);
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd);

void LCD_Fill(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd, uint16_t Color);
void LCD_DrawPoint(uint16_t PointX, uint16_t PointY, uint16_t Color);
void LCD_DrawLine(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd, uint16_t Color);
void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void LCD_DrawCircle(uint16_t CircleCenterX, uint16_t CircleCenterY, uint8_t CircleRadius, uint16_t Color);

void LCD_ShowChar(uint16_t CharX, uint16_t CharY, uint8_t Character, uint16_t FontColor, uint8_t FontSizeY);
void LCD_ShowString(uint16_t CharX, uint16_t CharY, const char *str, uint16_t FontColor, uint8_t FontSizeY);
uint32_t UserPow(uint8_t base, uint8_t power);
void LCD_ShowIntNum(uint16_t NumX, uint16_t NumY, uint16_t Num, uint8_t NumLength, uint16_t FontColor, uint8_t FontSizeY);
void LCD_ShowFloatNum(uint16_t NumX, uint16_t NumY, float num, uint8_t NumLength, uint16_t FontColor, uint8_t FontSizeY);

void LCD_ShowPicture(uint16_t PictureX, uint16_t PictureY, uint16_t PictureLength, uint16_t PictureWidth, const uint8_t Picture[]);

#endif
