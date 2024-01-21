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

#include "debug.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"
#include "led.h"

/* Global define */
#define TASK1_TASK_PRIO     5
#define TASK1_STK_SIZE      (256*4)
#define TASK2_TASK_PRIO     5
#define TASK2_STK_SIZE      (256*4)

/* Global Variable */
TaskHandle_t Task1Task_Handler;
TaskHandle_t Task2Task_Handler;

_Noreturn void task1_task(void *pvParameters)
{
    while(1)
    {
        printf("task1 entry\r\n");
//        GPIO_SetBits(GPIOB, GPIO_Pin_3);
//        vTaskDelay(100);
//        GPIO_ResetBits(GPIOB, GPIO_Pin_3);
        vTaskDelay(100);
    }
}

_Noreturn void task2_task(void *pvParameters)
{
    while(1)
    {
        printf("task2 entry\r\n");
//        GPIO_ResetBits(GPIOA, GPIO_Pin_15);
//        vTaskDelay(100);
//        GPIO_SetBits(GPIOA, GPIO_Pin_15);
        vTaskDelay(100);
    }
}

int main(void)
{
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
    SystemCoreClockUpdate();
    Delay_Init();
    USART_Printf_Init(115200);
    printf("SystemClk:%ld\r\n", SystemCoreClock);
    printf("ChipID:%08lx\r\n", DBGMCU_GetCHIPID());
    printf("FreeRTOS Kernel Version:%s\r\n", tskKERNEL_VERSION_NUMBER);
    Power_Ctrl_Init();
    LCD_Init();

    LCD_Clear(LCD_COLOR_BLACK);

    LED_Init();
    BEEP_Init();

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
