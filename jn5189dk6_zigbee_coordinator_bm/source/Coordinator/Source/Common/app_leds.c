/*APP_vSetLedBrightness
* Copyright 2019 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


/****************************************************************************/
/***        Include files                                                 ***/
/****************************************************************************/
#include <EmbeddedTypes.h>
#include "dbg.h"
#include "app.h"
#include "fsl_gpio.h"
#include "fsl_debug_console.h"
#include "fsl_iocon.h"
#include "fsl_inputmux.h"

#include "iot_led_pwm.h"


static uint8_t value  = 250;

uint16_t g_hue 		= 240;
uint8_t g_sat		= 100;
uint8_t g_val 		= 100;


/****************************************************************************
 *
 * NAME: APP_vLedInitialise
 *
 * DESCRIPTION:
 * initialise the application leds
 *
 * PARAMETER: void
 *
 * RETURNS: bool_t
 *
 ****************************************************************************/
void APP_vLedInitialise(void)
{

    /* Define the init structure for the output LED pin*/
    gpio_pin_config_t led_config = {
            kGPIO_DigitalOutput, OFF,
    };

    GPIO_PinInit(GPIO, APP_BOARD_GPIO_PORT, APP_BASE_BOARD_LED1_PIN, &led_config);
    GPIO_PinInit(GPIO, APP_BOARD_GPIO_PORT, APP_BASE_BOARD_LED2_PIN, &led_config);

}

/****************************************************************************
 *
 * NAME: APP_vSetLed
 *
 * DESCRIPTION:
 * set the state ofthe given application led
 *
 * PARAMETER:
 *
 * RETURNS:
 *
 ****************************************************************************/
void APP_vSetLed(uint8_t u8Led, bool_t bState)
{
    uint32_t u32LedPin = 0;
    switch (u8Led)
    {
    case LED1:
    	u32LedPin = APP_BASE_BOARD_LED1_PIN;
    	bState = (bState == OFF);
        break;
    case LED2:
    	u32LedPin = APP_BASE_BOARD_LED2_PIN;
    	bState = (bState == OFF);
        break;
    default:
        u32LedPin -= 1;
        break;
    }

    if (u32LedPin != 0xffffffff)
    {
        GPIO_PinWrite(GPIO, APP_BOARD_GPIO_PORT, u32LedPin, bState);
    }
}


/****************************************************************************
 *
 * NAME: APP_vSetLedBrightness
 *
 * DESCRIPTION:
 * set the brightness of LED
 *
 * PARAMETER:
 *
 * RETURNS:
 *
 ****************************************************************************/
void APP_vSetLedBrightness(uint8_t brightness)
{
	value = (brightness / 100.0) * 250;
}

void APP_vSetLedOn(void)
{
	changeHSV(g_hue, g_sat, g_val);
}

void APP_vSetLedOff(void)
{
	changeHSV(g_hue, g_sat, 0);
}

void APP_vSetLedColor(uint8_t red, uint8_t green, uint8_t blue)
{
//	RGB[RED] 	= red;
//	RGB[GREEN]	= green;
//	RGB[BLUE]	= blue;

//	changeRGB(RGB, value);
}
/****************************************************************************/
/***        Local Functions                                               ***/
/****************************************************************************/


/****************************************************************************/
/***        END OF FILE                                                   ***/
/****************************************************************************/
