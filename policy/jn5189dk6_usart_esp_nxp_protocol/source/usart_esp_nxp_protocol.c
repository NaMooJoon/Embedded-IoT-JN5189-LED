/*
 * Copyright (c) 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

/*System includes.*/
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "timers.h"

/* Freescale includes. */
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"
#include "fsl_usart.h"
#include "board.h"

#include "pin_mux.h"
#include "fsl_flash.h"
#include <stdbool.h>

/* IoT System includes */
#include "iot_usart.h"



/*******************************************************************************
 * Code
 ******************************************************************************/

/* Initializer */
void initializeSystem(void);

/* Application API */
void data_process_task (void *pvParameters); 	/* Data parsing process from the request of ESP */
void blink_led_task (void *pvParameters); 		/* Blink the led light */

/*!
 * @brief Main function
 */
int main (void)
{
    initializeSystem();
	initializeUsart();

    if (xTaskCreate(data_process_task, "DATA_PROCESS_TASK", configMINIMAL_STACK_SIZE + 166, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while(1);
    }
	if (xTaskCreate(blink_led_task, "BLINK_LED_TASK", configMINIMAL_STACK_SIZE + 50, NULL, tskIDLE_PRIORITY + 1, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while(1);
    }
    vTaskStartScheduler();
    for (;;);
}




/*******************************************************************************
 * Initializer
 ******************************************************************************/
void initializeSystem(void)
{
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
}


/*******************************************************************************
 * Application functions
 ******************************************************************************/

void data_process_task (void *pvParameters)
{
    RxDataPacket packet;
    PacketData ack_char;

    PRINTF("DATA_PROCESS_TASK \n\r");
#if 1
    while (1)
    {
    	/* Data parsing process */
    	if (get_rest_buf_size() >= MIN_RX_PACKET_SIZE)
    	{
    		ack_char = read_packet_from_buf(&packet);
    		write_response(ack_char);

    		if (ack_char == ack) {
				// TODO: cast the command to corresponding task thread.
    			print_packet_data(&packet);
				taskYIELD();
    		}
    	}

    }
#elif
    /* This code is for debugging to print raw data. */
    while(1) {
		while (back_idx != front_idx) {
			PRINTF("%d: %X \n\r", get_rest_buf_size(), pop_buf());
			if (back_idx == front_idx)
				PRINTF("\n\r");
		}
    }
#endif

}

void blink_led_task (void *pvParameters)
{
	while (1)
	{
		// TODO
	}
}
