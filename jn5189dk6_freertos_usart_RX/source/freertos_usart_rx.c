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
#include "board.h"

#include "fsl_usart_freertos.h"
#include "fsl_usart.h"

#include "pin_mux.h"
#include "fsl_flash.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_USART USART0
#define RX_USART   USART1
#define DEMO_USART_BAUD_RATE 	9600U
#define DEMO_USART_IRQHandler FLEXCOMM0_IRQHandler
#define DEMO_USART_IRQn FLEXCOMM0_IRQn
/* Task priorities. */
#define uart_task_PRIORITY (configMAX_PRIORITIES - 1)
#define USART_NVIC_PRIO 5
/* Queue */
#define MAX_LOG_LENGTH 20

typedef enum command {
	start = 2,
	end,
	power,
	brightness,
	hue,
	ack,
	nak,
	join,
	leave,
	dle,
	N_command
} Command;

/*******************************************************************************
 * Globals
 ******************************************************************************/
/* Logger queue handle */
static QueueHandle_t log_queue = NULL;
/*******************************************************************************
 * Prototypes
 ******************************************************************************/
/* Application API */
static void read_task(void *pvParameters);
//static void uart_task(void *pvParameters);

/* Logger API */
void log_add(char *log);
void log_init(uint32_t queue_length, uint32_t max_log_length);
static void log_task(void *pvParameters);

/*******************************************************************************
 * Code
 ******************************************************************************/
const char *to_send             = "FreeRTOS USART driver example!\r\n";
const char *send_buffer_overrun = "\r\nRing buffer overrun!\r\n";
//uint8_t background_buffer0[32];
uint8_t background_buffer1[32];
uint8_t recv_buffer[32];

//usart_rtos_handle_t handle0;
//struct _usart_handle t_handle0;
//struct rtos_usart_config usart_config0 = {
//    .baudrate    = DEMO_USART_BAUD_RATE,
//    .parity      = kUSART_ParityDisabled,
//    .stopbits    = kUSART_OneStopBit,
//    .buffer      = background_buffer0,
//    .buffer_size = sizeof(background_buffer0),
//};

usart_rtos_handle_t handle1;
struct _usart_handle t_handle1;
struct rtos_usart_config usart_config1 = {
    .baudrate    = DEMO_USART_BAUD_RATE,
    .parity      = kUSART_ParityDisabled,
    .stopbits    = kUSART_OneStopBit,
    .buffer      = background_buffer1,
    .buffer_size = sizeof(background_buffer1),
};

/*!
 * @brief Application entry point.
 */
int main(void)
{
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
    log_init(10, MAX_LOG_LENGTH);
    if (xTaskCreate(read_task, "Read_task", configMINIMAL_STACK_SIZE + 166, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    {
    	PRINTF("Task creation failed!\r\n");
    	while(1)
    		;
    }
//    if (xTaskCreate(uart_task, "Uart_task", configMINIMAL_STACK_SIZE + 10, NULL, uart_task_PRIORITY, NULL) != pdPASS)
//    {
//        PRINTF("Task creation failed!.\r\n");
//        while (1)
//            ;
//    }
    vTaskStartScheduler();
    for (;;)
        ;
}

/*******************************************************************************
 * Application functions
 ******************************************************************************/
static void read_task(void *pvParameters)
{
//	char log[MAX_LOG_LENGTH + 1];
	int error;
	size_t n 			 = 0;
	usart_config1.srcclk = BOARD_DEBUG_UART_CLK_FREQ;
	usart_config1.base	 = RX_USART;

	NVIC_SetPriority(DEMO_USART_IRQn, USART_NVIC_PRIO);

	if (0 > USART_RTOS_Init(&handle1, &t_handle1, &usart_config1))
	{
		vTaskSuspend(NULL);
	}

	do
	{
		error = USART_RTOS_Receive(&handle1, recv_buffer, sizeof(recv_buffer), &n);
		if (error == kStatus_USART_RxRingBufferOverrun)
		{
			/* Notify about hardware buffer overrun */
			if (kStatus_Success != USART_RTOS_Send(&handle1, (uint8_t *)send_buffer_overrun, strlen(send_buffer_overrun)))
			{
				vTaskSuspend(NULL);
			}
		}
		if (n > 0)
		{
			/* send back the received data */
//			USART_RTOS_Send(&handle1, (uint8_t *)recv_buffer, n);
			log_add((char *)recv_buffer);
			taskYIELD();
		}
	} while (kStatus_Success == error);

	USART_RTOS_Deinit(&handle1);
	vTaskSuspend(NULL);
}




/*******************************************************************************
 * Logger functions
 ******************************************************************************/
void log_add(char *log)
{
	xQueueSend(log_queue, log, 0);
}
void log_init(uint32_t queue_length, uint32_t max_log_length)
{
	log_queue = xQueueCreate(queue_length, max_log_length);
	/* Enable queue view in MCUX IDE FreeRTOS TAD plugin */
	if (log_queue != NULL)
	{
		vQueueAddToRegistry(log_queue, "LogQ");
	}
	if (xTaskCreate(log_task, "log_task", configMINIMAL_STACK_SIZE + 166, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS)
	{
		PRINTF("Task creation failed!\r\n");
		while (1)
			;
	}
}
static void log_task(void *pvParameters)
{
	uint32_t counter = 0;
	char log[MAX_LOG_LENGTH + 1];
	while (1)
	{
		if (xQueueReceive(log_queue, log, portMAX_DELAY) != pdTRUE)
		{
			PRINTF("Failed to receive queue.\r\n");
		}
		PRINTF("Log %d: %s\r\n", counter, log);
		counter++;
	}
}
