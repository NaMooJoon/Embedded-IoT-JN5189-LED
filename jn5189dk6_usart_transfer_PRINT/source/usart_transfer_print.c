/*
 * Copyright (c) 2013 - 2015, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_usart.h"
#include "fsl_device_registers.h"
#include "fsl_debug_console.h"

#include "pin_mux.h"
#include "fsl_flash.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_USART USART1
#define DEMO_USART_BAUD_RATE 		9600U
#define DEMO_USART_CLK_SRC 			kCLOCK_Fro32M  // 32MHz
#define DEMO_USART_CLK_FREQ 		CLOCK_GetFreq(DEMO_USART_CLK_SRC)

usart_config_t g_config;
/*******************************************************************************
 * Code
 ******************************************************************************/
/*!
 * @brief Main function
 */
int main(void)
{
    char ch;

    /* Init board hardware. */
    /* Security code to allow debug access */
    SYSCON->CODESECURITYPROT = 0x87654320;

    /* Initialize the UART*/
    USART_GetDefaultConfig(&g_config);
    g_config.baudRate_Bps = DEMO_USART_BAUD_RATE;
    g_config.enableTx	  = false;
    g_config.enableRx	  = true;

    USART_Init(DEMO_USART, &g_config, DEMO_USART_CLK_FREQ);


    /* attach clock for USART(debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);

    /* reset FLEXCOMM for USART */
    RESET_PeripheralReset(kFC0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO0_RST_SHIFT_RSTn);

#if (BOARD_BOOTCLOCKRUN_CORE_CLOCK > 32000000U)
    FLASH_SetReadMode(FLASH, true);
#endif
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitPins();

    PRINTF("USART PRINT.\r\n");

    while (1)
	{
		/* Check if there is a new data. */
		if (USART_GetStatusFlags(DEMO_USART) & kUSART_RxFifoNotEmptyFlag)
		{
			/* UART1 */
			ch = USART_ReadByte(DEMO_USART);

			/* Echo the received data. */
			PUTCHAR(ch);
		}
	}
}
