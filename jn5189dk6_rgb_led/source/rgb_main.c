/*
 * Copyright 2017 NXP
 * All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "board.h"
#include "fsl_gpio.h"

#include "pin_mux.h"
#include "fsl_flash.h"
#include <stdbool.h>

#include "fsl_debug_console.h"
#include "iot_led_pwm.h"

#define NUM_COROL

/*******************************************************************************
 * Definitions
 ******************************************************************************/
#define DEMO_USART USART0
#define DEMO_USART_CLK_SRC kCLOCK_Fro32M

/*******************************************************************************
 * Prototypes
 ******************************************************************************/

/*******************************************************************************
 * Variables
 ******************************************************************************/
volatile uint32_t g_systickCounter;
uint8_t RGB[NUM_COROL][3] = { // for test
		{0, 0, 0},
		{255, 255, 255},
		{0, 100, 100},
		{0, 200, 200},
		{0, 0, 205},
		{127, 100, 200}
};

/*******************************************************************************
 * Code
 ******************************************************************************/
void SysTick_Handler(void);
void SysTick_DelayTicks(uint32_t n);
void initializeSystem(void);

int main(void)
{
    int i = 0;
    int brightness = 200;

	initializeSystem();
	initializeRgbLed();

    PRINTF("RGB LED Tutorial\r\n");

    /* Set systick reload value to generate 1ms interrupt */
    if (SysTick_Config(SystemCoreClock / 1000U)) {
        while (1)
        {
        }
    }
    while (1)
    {
		changeRGB(RGB[i], brightness);
        /* Delay 2200 ms */
        SysTick_DelayTicks(2200U);
        i = (i+1) % 6;
    }
}


/*******************************************************************************/

void SysTick_Handler(void)
{
    if (g_systickCounter != 0U)
    {
        g_systickCounter--;
    }
}

void SysTick_DelayTicks(uint32_t n)
{
    g_systickCounter = n;
    while (g_systickCounter != 0U)
    {
    }
}

void initializeSystem(void)
{
    /* Board pin init */
    /* Security code to allow debug access */
    SYSCON->CODESECURITYPROT = 0x87654320;

    /* attach clock for USART(debug console) */

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
