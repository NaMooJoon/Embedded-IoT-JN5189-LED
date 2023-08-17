/*
* Copyright 2019 NXP
* All rights reserved.
*
* SPDX-License-Identifier: BSD-3-Clause
*/


#ifndef APP_LEDS_H
#define APP_LEDS_H

#include "app_common.h"

extern uint16_t g_hue;
extern uint8_t g_sat;
extern uint8_t g_val;

/****************************************************************************/
/***        Exported Functions                                            ***/
/****************************************************************************/
void APP_vLedInitialise(void);
void APP_vSetLed(uint8_t u8Led, bool_t bState);
void APP_vSetLedBrightness(uint8_t brightness);

void APP_vSetLedOn(void);
void APP_vSetLedOff(void);



#endif /*APP_LEDS_H*/
