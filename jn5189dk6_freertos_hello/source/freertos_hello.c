/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/* FreeRTOS kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_gpio.h"
#include "board.h"

#include "pin_mux.h"
#include "fsl_flash.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define BOARD_LED_PORT BOARD_LED_RED1_GPIO_PORT
#define BOARD_LED_PIN BOARD_LED_RED1_GPIO_PIN

/* Task priorities. */
#define hello_task_PRIORITY (configMAX_PRIORITIES - 1)
#define led_toggle_task_PRIORITY (configMAX_PRIORITIES - 2)
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
static void hello_task(void *pvParameters);
static void led_toggle_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/


/*!
 * @brief Application entry point.
 */
int main(void)
{
	gpio_pin_config_t led_config = {
			kGPIO_DigitalOutput,
			0,
	};

    /* Init board hardware. */
    /* Security code to allow debug access */
    SYSCON->CODESECURITYPROT = 0x87654320;

    /* attach clock for USART(debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* reset FLEXCOMM for USART */
    RESET_PeripheralReset(kFC0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO0_RST_SHIFT_RSTn);

#if (BOARD_BOOTCLOCKRUN_CORE_CLOCK > 32000000U)
    /* When the CPU clock frequency is increased,
     * the Set Read command shall be called before the frequency change. */
    FLASH_SetReadMode(FLASH, true);
#endif
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitPins();


    if (xTaskCreate(hello_task, "Hello_task", configMINIMAL_STACK_SIZE + 10, NULL, hello_task_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
    if (xTaskCreate(led_toggle_task, "Led_toggle_task", configMINIMAL_STACK_SIZE + 10, &led_config, led_toggle_task_PRIORITY, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
    vTaskStartScheduler();
    for (;;)
        ;
}

/*!
 * @brief Task responsible for printing of "Hello world." message.
 */
static void hello_task(void *pvParameters)
{
    for (;;)
    {
        PRINTF("Hello world.\r\n");
        vTaskSuspend(NULL);
    }
}

static void led_toggle_task(void *pvParameters)
{
	gpio_pin_config_t *led_config = (gpio_pin_config_t *)pvParameters;

    GPIO_PortInit(GPIO, BOARD_LED_PORT);
    GPIO_PinInit(GPIO, BOARD_LED_PORT, BOARD_LED_PIN, led_config);

    /* Set systick reload value to generate 1ms interrupt */
    if (SysTick_Config(SystemCoreClock / 1000U))
    {
        while (1)
        {
        }
    }

    while (1)
    {
        /* Delay 1000ms */
    	vTaskDelay(1000);
        GPIO_PortToggle(GPIO, BOARD_LED_PORT, 1u << BOARD_LED_PIN);
    }
}
