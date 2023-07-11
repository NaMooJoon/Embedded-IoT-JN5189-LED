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
/*******************************************************************************
 * Definitions
 ******************************************************************************/
/* USART */
#define ESP_USART USART1
#define ESP_USART_CLK_SRC kCLOCK_Fro32M
#define ESP_USART_CLK_FREQ CLOCK_GetFreq(ESP_USART_CLK_SRC)
#define ESP_USART_IRQn USART1_IRQn
#define ESP_USART_IRQHandler FLEXCOMM1_IRQHandler

#define RING_BUFFER_SIZE 32
#define ACK_SIZE 5

#define MIN_RX_PACKET_SIZE 10


/* ESP-NXP communication protocol packet data */
typedef enum packet_data {
	start = 0x02,
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
} PacketData;

/* data packet */
typedef struct rx_datapacket {
	uint8_t target;
	uint8_t command;
	uint8_t size;
	uint8_t data[2];
} RxDataPacket;

typedef struct tx_datapacket {
	uint8_t s_dle;
	uint8_t start;
	uint8_t command;
	uint8_t target;
	uint8_t checksum;
	uint8_t e_dle;
	uint8_t end;
} TxDataPacket;



/*******************************************************************************
 * Globals
 ******************************************************************************/
/* ESP-NXP receive buffer */
uint8_t ring_buffer[RING_BUFFER_SIZE];
volatile uint16_t front_idx = 0; 				/* Index of the data to send out. */
volatile uint16_t back_idx = 0; 				/* Index of the memory to save new arrived data. */


/*******************************************************************************
 * Prototypes
 ******************************************************************************/

void ESP_USART_IRQHandler (void);  				/* Interrupt Handler for ESP-NXP USART RX port*/

/* Application API */
void data_process_task (void *pvParameters); 	/* Data parsing process from the request of ESP */
void blink_led_task (void *pvParameters); 		/* Blink the led light */

/* data_process */
void send_ack (PacketData data);

/* Circular buffer (queue) API*/
static uint8_t pop_buf ();
static uint8_t peek_buf ();
static void push_buf (uint8_t byte);
static uint8_t get_rest_buf_size ();				/* Get the size number of remain data in buffer */


/*******************************************************************************
 * Code
 ******************************************************************************/

/*!
 * @brief Main function
 */
int main (void)
{
    usart_config_t config;

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


    /* Set usart config */
	USART_GetDefaultConfig(&config);
	config.baudRate_Bps = BOARD_DEBUG_UART_BAUDRATE;
	config.enableTx     = true;
	config.enableRx     = true;

	USART_Init(ESP_USART, &config, ESP_USART_CLK_FREQ);

	/* Enable RX interrupt. */
	USART_EnableInterrupts(ESP_USART, kUSART_RxLevelInterruptEnable | kUSART_RxErrorInterruptEnable);
	EnableIRQ(ESP_USART_IRQn);

    if (xTaskCreate(data_process_task, "DATA_PROCESS_TASK", configMINIMAL_STACK_SIZE + 166, NULL, tskIDLE_PRIORITY + 2, NULL) != pdPASS)
    {
        PRINTF("Task creation failed!.\r\n");
        while (1)
            ;
    }
    vTaskStartScheduler();
    for (;;)
        ;
}




/*******************************************************************************
 * Interrupt Handler
 ******************************************************************************/
void ESP_USART_IRQHandler (void)
{
    uint8_t data;

    /* If new data arrived. */
    if ((kUSART_RxFifoNotEmptyFlag | kUSART_RxError) & USART_GetStatusFlags(ESP_USART)) {

        data = USART_ReadByte(ESP_USART);

        /* If ring buffer is not full, add data to ring buffer. */
        if (((back_idx + 1) % RING_BUFFER_SIZE) != front_idx) {
            push_buf(data);
        }
    }
/* Add for ARM errata 838869, affects Cortex-M4, Cortex-M4F Store immediate overlapping
  exception return operation might vector to incorrect interrupt */
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}



/*******************************************************************************
 * Queue buffer functions
 ******************************************************************************/
uint8_t pop_buf ()
{
	uint8_t reval = ring_buffer[front_idx];
	front_idx = (front_idx+ 1) % RING_BUFFER_SIZE;
	return reval;
}

uint8_t peek_buf ()
{
	return ring_buffer[front_idx];
}

void push_buf (uint8_t byte)
{
	ring_buffer[back_idx] = byte;
	back_idx = (back_idx + 1) % RING_BUFFER_SIZE;
}

uint8_t get_rest_buf_size ()
{
	return (back_idx >= front_idx)? (back_idx - front_idx) :
								(back_idx + RING_BUFFER_SIZE) - front_idx;
}
/*******************************************************************************
 * Application functions
 ******************************************************************************/

void data_process_task (void *pvParameters)
{
    RxDataPacket packet;
    PacketData responce = ack;

    PRINTF("DATA_PROCESS_TASK \n\r");
#if 1
    while (1)
    {
    	/* Data parsing process */
__start_packet_parsing__:
    	if (get_rest_buf_size() >= MIN_RX_PACKET_SIZE)
    	{
    		responce = ack;

    		/* Find start byte */
    		if (pop_buf() != dle) 		goto __flush_packet_upto_dle__;
    		if (pop_buf() != start) 	goto __flush_packet_upto_dle__;
    		PRINTF("****** Packet request ******\n\r");
    		PRINTF("- Front_idx: %d, Back_idx: %d\n\r", front_idx, back_idx);

    		/* Read command */
    		packet.target = pop_buf();
    		packet.command = pop_buf();
    		PRINTF("- Target: %d, command:%d \r\n", packet.target, packet.command);

    		/* Read data*/
    		packet.size = pop_buf();
    		for (int i = 0; i < packet.size; i++) {
    			packet.data[i] = pop_buf();
    		}
    		PRINTF("- Size: %d\r\n", packet.size);

    		/* checksum */
    		PRINTF("- checksum1: %d,", pop_buf());
    		PRINTF("- checksum2: %d;", pop_buf());
    		// TODO: check the checksum number (CRC 16)

    		/* Check end byte */
    		if (pop_buf() != dle)	responce = nak;
    		if (pop_buf() != end)  	responce = nak;

    		/* Response ACK or NAK*/
    		send_ack(responce);
    		PRINTF("****** Packet received ******\n\r\n\r");
    		// TODO: cast the command to corresponding task thread.
			taskYIELD();

__flush_packet_upto_dle__:
			while (front_idx != back_idx) {
				if (peek_buf() == dle) 	goto __start_packet_parsing__;
				else 					pop_buf();
			}

    	}

    }
#endif


#if 0
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

void send_ack (PacketData data)
{
	static PacketData ACK[ACK_SIZE] = { dle, start, ack, dle, end };
	static PacketData NAK[ACK_SIZE] = { dle, start, nak, dle, end };

	switch (data)
	{
		case ack:
			USART_WriteBlocking(ESP_USART, ACK, ACK_SIZE);
			break;

		case nak:
			USART_WriteBlocking(ESP_USART, NAK, ACK_SIZE);
			break;
	}
}
