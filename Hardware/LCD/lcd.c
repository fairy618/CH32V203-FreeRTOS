/*********************************************************************
 * File Name          : lcd.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2023/8/15
 * Description        : file for lcd screen.
 *********************************************************************************
* Copyright (c) 2023 Nanjing Qinheng Microelectronics Co., Ltd.
* Attention: This software (modified or not) and binary are used for 
* microcontroller manufactured by Nanjing Qinheng Microelectronics.
*******************************************************************************/

#include "lcd.h"
#include "lcdfont.h"
#include "debug.h"

static uint16_t width;
static uint16_t height;

#define LCD_SOFT_RESET          (0x01)
#define LCD_READ_ID             (0x04)
#define LCD_READ_DISPLAY_STATUS (0x09)
#define LCD_ON                  (0x28)
#define LCD_OFF                 (0x29)

#define LCD_SET_X               (0x2a)
#define LCD_SET_Y               (0x2B)
#define LCD_MEM_WRITE           (0x2C)
#define LCD_MEM_READ            (0x2e)

//static uint16_t color_buffer[2][FILL_BUFFER_COUNT] = {0};

/*********************************************************************
 * @fn      LCD_WR_REG
 *
 * @brief   Write an 8-bit command to the LCD screen
 * 
 * @param   data - Command value to be written
 *
 * @return  none
 */
static void LCD_WR_REG(uint8_t data)
{
    LCD_CS_CLR;
    LCD_DC_CLR;
    SPI2_Write(data);
    LCD_CS_SET;
}

/*********************************************************************
 * @fn      LCD_WR_DATA
 *
 * @brief   Write an 8-bit data to the LCD screen
 * 
 * @param   data - the data to write
 * 
 */
static void LCD_WR_DATA(uint8_t data)
{
    LCD_CS_CLR;
    LCD_DC_SET;
    SPI2_Write(data);
    LCD_CS_SET;
}

/*********************************************************************
 * @fn      LCD_LCD_WR_REG
 * 
 * @brief   write a data to the register
 * 
 * @param   LCD_Reg - register address
 *          LCD_RegValue - the data to write
 * 
 * @return  none
 */
static void LCD_LCD_WR_REG(uint8_t LCD_Reg, uint8_t LCD_RegValue)
{
    LCD_WR_REG(LCD_Reg);
    LCD_WR_DATA(LCD_RegValue);
}

/*********************************************************************
 * @fn      Lcd_WriteData_16Bit
 * 
 * @brief   write two bytes to the lcd screen
 * 
 * @param   Data - the data to write
 * 
 * @return  none
 */
void Lcd_WriteData_16Bit(uint16_t Data)
{
    LCD_CS_CLR;
    LCD_DC_SET;
    SPI2_Write(Data >> 8);
    SPI2_Write(Data);
    LCD_CS_SET;
}

/*********************************************************************
 * @fn      LCD_GPIOInit
 * 
 * @brief   Configuring the control gpio for the lcd screen
 * 
 * @return  none
 */
void LCD_GPIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    RCC_APB2PeriphClockCmd(LCD_LED_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(LCD_DC_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(LCD_RST_RCC, ENABLE);
    RCC_APB2PeriphClockCmd(LCD_CS_RCC, ENABLE);

    GPIO_InitStructure.GPIO_Pin = LCD_LED_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_LED_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_DC_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_DC_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_RST_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_RST_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = LCD_CS_PIN;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(LCD_CS_PORT, &GPIO_InitStructure);
}

/*********************************************************************
 * @fn      LCD_RESET
 * 
 * @brief   reset the lcd screen by the rst pin
 * 
 * @return  none
 */
void LCD_RESET(void)
{
    LCD_RST_CLR;
    Delay_Ms(100);
    LCD_RST_SET;
    Delay_Ms(50);
}

/*********************************************************************
 * @fn      LCD_Init
 * 
 * @brief   Initialization LCD screen
 * 
 * @return  none
 */
void LCD_Init(void)
{
    SPI2_Init();
    SPI2_DMA_Init();
    LCD_GPIOInit();
    LCD_RESET();
    LCD_LED_SET;
    Delay_Ms(100);

    // Software Reset
    LCD_WR_REG(0x01);
    Delay_Ms(120);

//    // Sleep in
//    LCD_WR_REG(0x10);
    // Sleep out
    LCD_WR_REG(0x11);
    Delay_Ms(120);

    // Memory Data Access Control
    LCD_direction(USE_HORIZONTAL);

    // Interface Pixel Format
    LCD_WR_REG(0x3A);
//    LCD_WR_DATA(0x06);  // 262K of RGB interface & 18bit/pixel RGB666
//    LCD_WR_DATA(0x55);  // 65K of RGB interface & 16bit/pixel RGB565
    LCD_WR_DATA(0x05);

    // Porch Setting
    LCD_WR_REG(0xB2);
    LCD_WR_DATA(0x0C);
    LCD_WR_DATA(0x0C);
    LCD_WR_DATA(0x00);
    LCD_WR_DATA(0x33);
    LCD_WR_DATA(0x33);

    // Gate Control
    LCD_WR_REG(0xB7);
    LCD_WR_DATA(0x35);

    // VCOMS Setting
    LCD_WR_REG(0xBB);
    LCD_WR_DATA(0x19);

    // LCM Control
    LCD_WR_REG(0xC0);
    LCD_WR_DATA(0x2C);

    // VDV and VRH Command Enable
    LCD_WR_REG(0xC2);
    LCD_WR_DATA(0x01);
//    LCD_WR_DATA(0xFF);

    // VRH Set
    LCD_WR_REG(0xC3);
    LCD_WR_DATA(0x12);

    // VDV Set
    LCD_WR_REG(0xC4);
    LCD_WR_DATA(0x20);

    // Frame Rate Control in Normal Mode
    LCD_WR_REG(0xC6);
    LCD_WR_DATA(0x0F);

    //  CABC Control
    LCD_WR_REG(0xD0);
    LCD_WR_DATA(0xA4);
    LCD_WR_DATA(0xA1);

    // Positive Voltage Gamma Control
    LCD_WR_REG(0xE0);
    LCD_WR_DATA(0xD0);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x0D);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x2B);
    LCD_WR_DATA(0x3F);
    LCD_WR_DATA(0x54);
    LCD_WR_DATA(0x4C);
    LCD_WR_DATA(0x18);
    LCD_WR_DATA(0x0D);
    LCD_WR_DATA(0x0B);
    LCD_WR_DATA(0x1F);
    LCD_WR_DATA(0x23);

    // Negative Voltage Gamma Control
    LCD_WR_REG(0xE1);
    LCD_WR_DATA(0xD0);
    LCD_WR_DATA(0x04);
    LCD_WR_DATA(0x0C);
    LCD_WR_DATA(0x11);
    LCD_WR_DATA(0x13);
    LCD_WR_DATA(0x2C);
    LCD_WR_DATA(0x3F);
    LCD_WR_DATA(0x44);
    LCD_WR_DATA(0x51);
    LCD_WR_DATA(0x2F);
    LCD_WR_DATA(0x1F);
    LCD_WR_DATA(0x1F);
    LCD_WR_DATA(0x20);
    LCD_WR_DATA(0x23);

////    // Partial Display Mode On
////    LCD_WR_REG(0x12);
//    // Normal Display Mode On
//    LCD_WR_REG(0x13);
//    // Gamma Set
//    LCD_WR_REG(0x26);
//    LCD_WR_DATA(0x01);
//    // Idle Mode Off
//    LCD_WR_REG(0x38);
////    // Idle Mode on
////    LCD_WR_REG(0x39);
//    // Write Display Brightness
//    LCD_WR_REG(0x51);
//    LCD_WR_DATA(0xFF);  // 00h value means the lowest brightness and FFh value means the highest brightness
//    // Write CTRL Display
//    LCD_WR_REG(0x53);
//    LCD_WR_DATA(0x2C);
//    // Write Content Adaptive Brightness Control and Color Enhancement
//    LCD_WR_REG(0x55);
////    LCD_WR_DATA(0x92);
//    LCD_WR_DATA(0x00);
//    // Write CABC Minimum Brightness
//    LCD_WR_REG(0x5E);
//    LCD_WR_DATA(0x00);
//    // RAM Control
//    LCD_WR_REG(0xB0);
//    LCD_WR_DATA(0x00);
//    LCD_WR_DATA(0xF0);
//    // Frame Rate Control 1 (In partial mode/ idle colors)
//    LCD_WR_REG(0xB3);
//    LCD_WR_DATA(0x00);
//    LCD_WR_DATA(0x0F);
//    LCD_WR_DATA(0x0F);
//    // Partial mode Control
//    LCD_WR_REG(0xB5);
//    LCD_WR_DATA(0x00);
//    // Gate On Timing Adjustment
//    LCD_WR_REG(0xB8);
//    LCD_WR_DATA(0x2A);
//    LCD_WR_DATA(0x2B);
//    LCD_WR_DATA(0x22);
//    LCD_WR_DATA(0xF5);
//    // Digital Gamma Enable
//    LCD_WR_REG(0xBA);
//    LCD_WR_DATA(0x00);
//    // VCOMS Offset Set
//    LCD_WR_REG(0xC5);
//    LCD_WR_DATA(0x20);
//    // CABC Control
//    LCD_WR_REG(0xC7);
//    LCD_WR_DATA(0x00);
//    // Register Value Selection 1
//    LCD_WR_REG(0xC8);
//    LCD_WR_DATA(0x08);
//    // Register Value Selection 2
//    LCD_WR_REG(0xCA);
//    LCD_WR_DATA(0x0F);
//    // PWM Frequency Selection
//    LCD_WR_REG(0xCC);
//    LCD_WR_DATA(0x02);

//    // Display inversion off
//    LCD_WR_REG(0x20);
    // Display inversion on
    LCD_WR_REG(0x21);

    //    // Display Off
//    LCD_WR_REG(0x28);
    // Display On
    LCD_WR_REG(0x29);
}

/****************************************************** ***************
 * @fn      LCD_SetWindows
 * 
 * @brief   Setting LCD display window
 * 
 * @param   xStar -  the bebinning x coordinate of the LCD display window
 *          yStar -  the bebinning y coordinate of the LCD display window
 *          xEnd -  the endning x coordinate of the LCD display window
 *          yEnd -  the endning y coordinate of the LCD display window
 * 
 * @return  none
 */
void LCD_SetWindows(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd)
{
    xStar += LCD_X_OFFSET;
    xEnd += LCD_X_OFFSET;
    yStar += LCD_Y_OFFSET;
    yEnd += LCD_Y_OFFSET;

    LCD_WR_REG(LCD_SET_X);
    LCD_WR_DATA(xStar >> 8);
    LCD_WR_DATA(0x00FF & xStar);
    LCD_WR_DATA(xEnd >> 8);
    LCD_WR_DATA(0x00FF & xEnd);

    LCD_WR_REG(LCD_SET_Y);
    LCD_WR_DATA(yStar >> 8);
    LCD_WR_DATA(0x00FF & yStar);
    LCD_WR_DATA(yEnd >> 8);
    LCD_WR_DATA(0x00FF & yEnd);
    LCD_WR_REG(LCD_MEM_WRITE);
}

/*********************************************************************
 * @fn      LCD_direction
 * 
 * @brief   Setting the display direction of LCD screen
 * 
 * @param   direction -   direction:0-0 degree  1-90 degree  2-180 degree  3-270 degree
 * 
 * @return  none
 */
void LCD_direction(uint8_t direction)
{
    switch(direction)
    {
        case 0:width = LCD_W;
            height = LCD_H;
            LCD_LCD_WR_REG(0x36, (0 << 6) | (0 << 7));  //,MY==0,MX==0,MV==0,ML==0
            break;
        case 1:width = LCD_W;
            height = LCD_H;
            LCD_LCD_WR_REG(0x36, (1 << 6) | (1 << 7));  // MY==1,MX==1,MV==0,ML==0
            break;
        case 2:width = LCD_H;
            height = LCD_W;
            LCD_LCD_WR_REG(0x36, (1 << 6) | (1 << 5) | (1 << 4));  //MY==0,MX==1,MV==1,ML==1
            break;
        case 3:width = LCD_H;
            height = LCD_W;
            LCD_LCD_WR_REG(0x36, (1 << 7) | (1 << 5));  // MY==1,MX==0,MV==1,ML==0
            break;
        default:break;
    }
}

/*********************************************************************
 * @fn      LCD_Clear
 * 
 * @brief   Full screen filled LCD screen
 * 
 * @param   Color -   Filled color
 * 
 * @return  none
 */
void LCD_Clear(uint16_t Color)
{
    unsigned int i, m;
    LCD_SetWindows(0, 0, width - 1, height - 1);
    for(i = 0; i < height; i++)
    {
        for(m = 0; m < width; m++)
        {
            Lcd_WriteData_16Bit(Color);
        }
    }
}

/*********************************************************************
 * @fn      LCD_Fill
 *
 * @brief   Fill the specified area of the LCD with color
 *
 * @param   xStar -  the bebinning x coordinate of the LCD display window
 *          yStar -  the bebinning y coordinate of the LCD display window
 *          xEnd -  the endning x coordinate of the LCD display window
 *          yEnd -  the endning y coordinate of the LCD display window
 *          Color -   Filled color
 *
 * @return  none
 */
void LCD_Fill(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd, uint16_t Color)
{
    unsigned int i, j;
    LCD_SetWindows(xStar, yStar, xEnd - 1, yEnd - 1);
    for(i = yStar; i < yEnd; i++)
    {
        for(j = xStar; j < xEnd; j++)
        {
            Lcd_WriteData_16Bit(Color);
        }
    }
}

/*********************************************************************
 * @fn      LCD_DrawPoint
 *
 * @brief   Draws a point at the coordinates specified by the LCD.
 *
 * @param   x -  The horizontal (x) coordinates of the point
 *          y -  The vertical (y) coordinates of the point
 *          Color -   Color of the point
 *
 * @return  none
 */
void LCD_DrawPoint(uint16_t PointX, uint16_t PointY, uint16_t Color)
{
    LCD_SetWindows(PointX, PointY, PointX, PointY);
    Lcd_WriteData_16Bit(Color);
}

/*********************************************************************
 * @fn      LCD_DrawLine
 *
 * @brief   Draws a line at the coordinates specified by the LCD.
 *
 * @param   xStar -  the bebinning x coordinate of the LCD display window
 *          yStar -  the bebinning y coordinate of the LCD display window
 *          xEnd -  the endning x coordinate of the LCD display window
 *          yEnd -  the endning y coordinate of the LCD display window
 *          Color -   Color of the line
 *
 * @return  none
 */
void LCD_DrawLine(uint16_t xStar, uint16_t yStar, uint16_t xEnd, uint16_t yEnd, uint16_t Color)
{
    uint16_t i;
    int distance, IncrementX, IncrementY;
    int DeltaX = xEnd - xStar;
    int DeltaY = yEnd - yStar;
    int PointX = xStar;
    int PointY = yStar;
    int CorrectionX = 0;
    int CorrectionY = 0;

    if(DeltaX > 0)
    {
        IncrementX = 1; // 设置单步方向
    }
    else if(DeltaX == 0)
    {
        IncrementX = 0; // 垂直线
    }
    else
    {
        IncrementX = -1;
        DeltaX = -DeltaX;
    }

    if(DeltaY > 0)
    {
        IncrementY = 1;
    }
    else if(DeltaY == 0)
    {
        IncrementY = 0; // 水平线
    }
    else
    {
        IncrementY = -1;
        DeltaY = -DeltaY;
    }

    if(DeltaX > DeltaY)
    {
        distance = DeltaX; // 选取基本增量坐标轴
    }
    else
    {
        distance = DeltaY;
    }

    for(i = 0; i < distance + 1; i++)
    {
        LCD_DrawPoint(PointX, PointY, Color);
        CorrectionX += DeltaX;
        CorrectionY += DeltaY;
        if(CorrectionX > distance)
        {
            CorrectionX -= distance;
            PointX += IncrementX;
        }
        if(CorrectionY > distance)
        {
            CorrectionY -= distance;
            PointY += IncrementY;
        }
    }
}

void LCD_DrawRectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    LCD_DrawLine(x1, y1, x2, y1, color);
    LCD_DrawLine(x1, y1, x1, y2, color);
    LCD_DrawLine(x1, y2, x2, y2, color);
    LCD_DrawLine(x2, y1, x2, y2, color);
}

void LCD_ShowPicture(uint16_t PictureX, uint16_t PictureY, uint16_t PictureLength, uint16_t PictureWidth, const uint8_t Picture[])
{
    uint16_t i, j;
    uint32_t k = 0;
    LCD_SetWindows(PictureX, PictureY, PictureX + PictureLength - 1, PictureY + PictureWidth - 1);
    for(i = 0; i < PictureLength; i++)
    {
        for(j = 0; j < PictureWidth; j++)
        {
            LCD_WR_DATA(Picture[k * 2]);
            LCD_WR_DATA(Picture[k * 2 + 1]);
            k++;
        }
    }
}

void LCD_DrawCircle(uint16_t CircleCenterX, uint16_t CircleCenterY, uint8_t CircleRadius, uint16_t Color)
{
    int a, b;
    a = 0;
    b = CircleRadius;
    while(a <= b)
    {
        LCD_DrawPoint(CircleCenterX - b, CircleCenterY - a, Color); // 3
        LCD_DrawPoint(CircleCenterX + b, CircleCenterY - a, Color); // 0
        LCD_DrawPoint(CircleCenterX - a, CircleCenterY + b, Color); // 1
        LCD_DrawPoint(CircleCenterX - a, CircleCenterY - b, Color); // 2
        LCD_DrawPoint(CircleCenterX + b, CircleCenterY + a, Color); // 4
        LCD_DrawPoint(CircleCenterX + a, CircleCenterY - b, Color); // 5
        LCD_DrawPoint(CircleCenterX + a, CircleCenterY + b, Color); // 6
        LCD_DrawPoint(CircleCenterX - b, CircleCenterY + a, Color); // 7
        a++;
        if((a * a + b * b) > (CircleRadius * CircleRadius)) // 判断要画的点是否过远
        {
            b--;
        }
    }
}

/*********************************************************************
 * @fn      LCD_ShowChar
 *
 * @brief   LCD display single character
 *
 * @param   CharX -  coordinates
 *          CharY -  coordinates
 *          Character -  the character
 *          FontColor -  Font clolr
 *          FontSizeY -   font size
 *
 * @return  none
 */
void LCD_ShowChar(uint16_t CharX, uint16_t CharY, uint8_t Character, uint16_t FontColor, uint8_t FontSizeY)
{
    uint8_t temp, FontSizeX, t;
    uint16_t i, TypefaceNum; // 一个字符所占字节大小
    uint16_t x0 = CharX;
    FontSizeX = FontSizeY / 2;
    TypefaceNum = (FontSizeX / 8 + ((FontSizeX % 8) ? 1 : 0)) * FontSizeY;
    Character = Character - ' ';                                     // 得到偏移后的值
    LCD_SetWindows(CharX, CharY, CharX + FontSizeX - 1, CharY + FontSizeY - 1); // 设置光标位置
    for(i = 0; i < TypefaceNum; i++)
    {
        if(FontSizeY == 12)
        {
            temp = ascii_1206[Character][i]; // 调用6x12字体
        }
        else if(FontSizeY == 16)
        {
            temp = ascii_1608[Character][i]; // 调用8x16字体
        }
        else if(FontSizeY == 24)
        {
            temp = ascii_2412[Character][i]; // 调用12x24字体
        }
        else if(FontSizeY == 32)
        {
            temp = ascii_3216[Character][i]; // 调用16x32字体
        }
        else
        {
            return;
        }

        for(t = 0; t < 8; t++)
        {
            if(temp & (0x01 << t))
            {
                LCD_DrawPoint(CharX, CharY, FontColor);
            }
            CharX++;
            if((CharX - x0) == FontSizeX)
            {
                CharX = x0;
                CharY++;
                break;
            }
        }
    }
}

void LCD_ShowString(uint16_t CharX, uint16_t CharY, const char *str, uint16_t FontColor, uint8_t FontSizeY)
{
    while(*str != '\0')
    {
        LCD_ShowChar(CharX, CharY, *str, FontColor, FontSizeY);
        CharX += FontSizeY / 2;
        str++;
    }
}

uint32_t UserPow(uint8_t base, uint8_t power)
{
    uint32_t result = 1;
    while(power--)
    {
        result *= base;
    }
    return result;
}

void LCD_ShowIntNum(uint16_t NumX, uint16_t NumY, uint16_t Num, uint8_t NumLength, uint16_t FontColor, uint8_t FontSizeY)
{
    uint8_t t, temp;
    uint8_t EnShow = 0;
    uint8_t FontSizeX = FontSizeY / 2;
    for(t = 0; t < NumLength; t++)
    {
        temp = (Num / UserPow(10, NumLength - t - 1)) % 10;
        if(EnShow == 0 && t < (NumLength - 1))
        {
            if(temp == 0)
            {
                LCD_ShowChar(NumX + t * FontSizeX, NumY, ' ', FontColor, FontSizeY);
                continue;
            }
            else
            {
                EnShow = 1;
            }
        }
        LCD_ShowChar(NumX + t * FontSizeX, NumY, temp + 48, FontColor, FontSizeY);
    }
}

void LCD_ShowFloatNum(uint16_t NumX, uint16_t NumY, float num, uint8_t NumLength, uint16_t FontColor, uint8_t FontSizeY)
{
    uint8_t t, temp, FontSizeX;
    uint16_t num1;
    FontSizeX = FontSizeY / 2;
    num1 = num * 100;
    for(t = 0; t < NumLength; t++)
    {
        temp = (num1 / UserPow(10, NumLength - t - 1)) % 10;
        if(t == (NumLength - 2))
        {
            LCD_ShowChar(NumX + (NumLength - 2) * FontSizeX, NumY, '.', FontColor, FontSizeY);
            t++;
            NumLength += 1;
        }
        LCD_ShowChar(NumX + t * FontSizeX, NumY, temp + 48, FontColor, FontSizeY);
    }
}

void LCD_DrawWave(int RawValue)
{
    static uint8_t Flag_FirstDraw = 1;
    static uint16_t LastX, LastY;
    uint16_t CurrentX, CurrentY;

    CurrentY = LCD_WAVE_CENTRE - RawValue;

    if(Flag_FirstDraw)
    {
        Flag_FirstDraw = 0;
        CurrentX = 0;
        LCD_DrawPoint(CurrentX, CurrentY, LCD_COLOR_FONT);
    }
    else
    {
        CurrentX = LastX + 1;
        if(CurrentX < (LCD_H - 1))
        {
            LCD_DrawLine(CurrentX + 1, 0, CurrentX + 1, 135, LCD_COLOR_BACKGROUND);
            LCD_DrawLine(LastX, LastY, CurrentX, CurrentY, LCD_COLOR_FONT);

        }
        else if(CurrentX == (LCD_H - 1))
        {
            LCD_DrawLine(0, 0, 0, 135, LCD_COLOR_BACKGROUND);
            LCD_DrawLine(LastX, LastY, CurrentX, CurrentY, LCD_COLOR_FONT);
        }
        else
        {
            CurrentX = 0;
            LCD_DrawLine(CurrentX + 1, 0, CurrentX + 1, 135, LCD_COLOR_BACKGROUND);
            LCD_DrawPoint(CurrentX, CurrentY, LCD_COLOR_FONT);
        }
    }
    LastX = CurrentX;
    LastY = CurrentY;
}
