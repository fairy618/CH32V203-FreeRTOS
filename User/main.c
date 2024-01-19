/********************************** (C) COPYRIGHT *******************************
 * File Name          : main.c
 * Author             : WCH
 * Version            : V1.0.0
 * Date               : 2021/06/06
 * Description        : Main program body.
 *********************************************************************************
 * Copyright (c) 2021 Nanjing Qinheng Microelectronics Co., Ltd.
 * Attention: This software (modified or not) and binary are used for 
 * microcontroller manufactured by Nanjing Qinheng Microelectronics.
 *******************************************************************************/

/*
 *@Note
 *task1 and task2 alternate printing
 */

#include "debug.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"
#include "lcd_img.h"

/* Global define */
#define TASK1_TASK_PRIO     5
#define TASK1_STK_SIZE      (256*4)
#define TASK2_TASK_PRIO     5
#define TASK2_STK_SIZE      (256*4)

/* Global Variable */
TaskHandle_t Task1Task_Handler;
TaskHandle_t Task2Task_Handler;

extern const uint8_t gImage_1[];

/*********************************************************************
 * @fn      GPIO_Toggle_INIT
 *
 * @brief   Initializes GPIOA.0/1
 *
 * @return  none
 */
void GPIO_Toggle_INIT(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_15;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

}

/*********************************************************************
 * @fn      task1_task
 *
 * @brief   task1 program.
 *
 * @param  *pvParameters - Parameters point of task1
 *
 * @return  none
 */
_Noreturn void task1_task(void *pvParameters)
{
    while(1)
    {
        printf("task1 entry\r\n");
        GPIO_SetBits(GPIOB, GPIO_Pin_3);
        vTaskDelay(100);
        GPIO_ResetBits(GPIOB, GPIO_Pin_3);
        vTaskDelay(100);
    }
}

/*********************************************************************
 * @fn      task2_task
 *
 * @brief   task2 program.
 *
 * @param  *pvParameters - Parameters point of task2
 *
 * @return  none
 */
_Noreturn void task2_task(void *pvParameters)
{
    while(1)
    {
        printf("task2 entry\r\n");
        GPIO_ResetBits(GPIOA, GPIO_Pin_15);
        vTaskDelay(100);
        GPIO_SetBits(GPIOA, GPIO_Pin_15);
        vTaskDelay(100);
    }
}

/*********************************************************************
 * @fn      main
 *
 * @brief   Main program.
 *
 * @return  none
 */
int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%ld\r\n", SystemCoreClock);
    printf("ChipID:%08lx\r\n", DBGMCU_GetCHIPID());
    printf("FreeRTOS Kernel Version:%s\r\n", tskKERNEL_VERSION_NUMBER);

    LCD_Init();

    LCD_Clear(LCD_GRAY);

    LCD_ShowPicture(0, 0, 100, 100, gImage_1);

    LCD_DrawPoint(170, 2, LCD_WHITE);

//    LCD_DrawLine(140, 5, 140, 50, LCD_WHITE);
//    LCD_DrawLine(140, 5, 200, 5, LCD_BLACK);
//    LCD_DrawLine(140, 50, 200, 50, LCD_BLUE);
//    LCD_DrawLine(140, 5, 200, 50, LCD_BRED);

    LCD_Fill(200, 5, 240, 25, LCD_GRED);
    LCD_Fill(200, 25, 240, 50, LCD_GBLUE);

    LCD_DrawRectangle(150, 10, 190, 40, LCD_RED);
    LCD_DrawCircle(170, 28, 27, LCD_MAGENTA);

    LCD_ShowChar(140, 60, 'A', LCD_GREEN, 24);

    LCD_ShowString(0, 100, "FairyTale", LCD_CYAN, 32);

    LCD_ShowIntNum(180, 60, 256, 3, LCD_YELLOW, 24);

    GPIO_Toggle_INIT();
    /* create two task */
    xTaskCreate((TaskFunction_t) task2_task,
                (const char *) "task2",
                (uint16_t) TASK2_STK_SIZE,
                (void *) NULL,
                (UBaseType_t) TASK2_TASK_PRIO,
                (TaskHandle_t *) &Task2Task_Handler);

    xTaskCreate((TaskFunction_t) task1_task,
                (const char *) "task1",
                (uint16_t) TASK1_STK_SIZE,
                (void *) NULL,
                (UBaseType_t) TASK1_TASK_PRIO,
                (TaskHandle_t *) &Task1Task_Handler);
    vTaskStartScheduler();

    while(1)
    {
        printf("shouldn't run at here!!\n");
    }
}
