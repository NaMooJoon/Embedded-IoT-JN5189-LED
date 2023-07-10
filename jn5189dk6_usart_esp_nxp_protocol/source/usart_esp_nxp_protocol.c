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
#include "board.h"

#include "pin_mux.h"
#include "fsl_flash.h"
#include <stdbool.h>
/*******************************************************************************
 * Definitions
 ******************************************************************************/

/*******************************************************************************
 * Globals
 ******************************************************************************/

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main(void)
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
    for (;;)
        ;
}

/*******************************************************************************
 * Application functions
 ******************************************************************************/
