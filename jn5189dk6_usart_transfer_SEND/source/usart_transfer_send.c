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
    SYSCON->CODESECURITYPROT = 0x87654320; /* 하드웨어 초기화: SYSCON->CODESECURITYPROT = 0x87654320 */

    /* Initialize the UART*/
    USART_GetDefaultConfig(&g_config);
    g_config.baudRate_Bps = DEMO_USART_BAUD_RATE;
    g_config.enableTx	  = true;
    g_config.enableRx	  = false;

    USART_Init(DEMO_USART, &g_config, DEMO_USART_CLK_FREQ);


    /* attach clock for USART(debug console) */
    CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH);
    /* USART(debug console) clock 연결:
     * CLOCK_AttachClk(BOARD_DEBUG_UART_CLK_ATTACH); 은 USART(시리얼 통신을 위한 인터페이스)에
     * 클럭을 연결한다. 이 코드는 보드 내부의 시스템 클럭을 USART 모듈에 연결하는 것을 의미
     * USART는 이 클럭을 사용해 데이터를 동기화하고, 정확한 타이밍에 데이터를 전송하거나 수신함.
     * 모든 데이터 통신은 이 클럭에 동기화가 됨.
     */

    /* reset FLEXCOMM for USART */
    RESET_PeripheralReset(kFC0_RST_SHIFT_RSTn);
    RESET_PeripheralReset(kGPIO0_RST_SHIFT_RSTn);
    /* USART와 GPIO(General Purpose Input/Output, 임베디드 시스템에서 주로 사용되는 입출력 인터페이스)를 리셋하는 코드
     * 이를 통해 이전에 설정된 값들이 초기화 되고, 통신을 위한 준비가 완료된다.
     * kFC0_RST_SHIFT_RSTn: 이 식별자는 "Flexible Communications Interface 0, FC0)로
     * 						여러 통신 프로토콜(USART, SPI, I2C 등)을 지원함.
     * kGPIO0_RST_SHIFT_RSTn: 이 식별자는 "GPIO0"을 나타냄.
     */

#if (BOARD_BOOTCLOCKRUN_CORE_CLOCK > 32000000U)
    /* When the CPU clock frequency is increased,
     * the Set Read command shall be called before the frequency change. */
    FLASH_SetReadMode(FLASH, true);
#endif
    BOARD_BootClockRUN();
    BOARD_InitDebugConsole();
    BOARD_InitPins();
    /* 디버그 콘솔과 핀(임베디드 보드에서 신호를 주고받는데 사용되는 연결점)을 초기화함. */

    PRINTF("USART SEND.\r\n\n");

    while (1)
    {
        ch = GETCHAR();
        if (ch == '\r') {
        	PUTCHAR('\n');
        	USART_WriteByte(DEMO_USART, '\n');
        }
        PUTCHAR(ch);
        USART_WriteByte(DEMO_USART, ch);
    }
}
