/*System includes.*/
#include <stdio.h>

#include "fsl_usart.h"

/* IoT System includes */
#include "iot_usart.h"
#include "iot_crc8.h"

#include "dbg.h"
#include "ZQueue.h"
#include "app_coordinator.h"
#include "app_common.h"
#include "app_main.h"
#include "fsl_reset.h"
#include "MicroSpecific.h"

/* ESP-NXP receive buffer */
uint8_t seq = 0;
uint8_t ring_buffer[RING_BUFFER_SIZE];
volatile uint16_t front_idx = 0; 				/* Index of the data to send out. */
volatile uint16_t back_idx = 0; 				/* Index of the memory to save new arrived data. */



/*******************************************************************************
 * Initializer
 ******************************************************************************/

void initializeUsart(void)
{
	usart_config_t config;

    /* Set usart config */
	USART_GetDefaultConfig(&config);
	config.baudRate_Bps = ESP_USART_BAUDRATE;
	config.enableTx     = true;
	config.enableRx     = true;

	USART_Init(ESP_USART, &config, ESP_USART_CLK_FREQ);

	/* Enable RX interrupt. */
	USART_EnableInterrupts(ESP_USART, kUSART_RxLevelInterruptEnable | kUSART_RxErrorInterruptEnable);
	EnableIRQ(ESP_USART_IRQn);
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
 * Data process task
 ******************************************************************************/
PacketData read_packet_from_buf (uint8_t *packet)
{
	uint8_t checksum;
	uint8_t size;
	uint8_t i;

	/* Find start byte */
	if (pop_buf() != dle) 		goto __flush_packet_upto_dle__;
	if (pop_buf() != start) 	goto __flush_packet_upto_dle__;

	/* Read SEQ */
	seq = pop_buf();

	/* Read command */
	size = peek_buf();
	for (i = 0; i < size; i++) {
		packet[i] = pop_buf();
	}

	/* checksum */
	checksum = calc_checksum(packet, size);
	if (checksum != pop_buf())	goto __flush_packet_upto_dle__;


	while (get_rest_buf_size() < 2) ; /* Prevent buffer overflow */
	if (pop_buf() != dle)		goto __flush_packet_upto_dle__;
	if (pop_buf() != end)  		goto __flush_packet_upto_dle__;

	return ack;

__flush_packet_upto_dle__:
	while (front_idx != back_idx) {
		if (peek_buf() == dle) 	break;
		else 					pop_buf();
	}
	return nak;
}

void write_response (PacketData data)
{
	PacketData ACK[ACK_SIZE] = { dle, start, ack, seq, dle, end };
	PacketData NAK[ACK_SIZE] = { dle, start, nak, seq, dle, end };

	switch (data)
	{
		case ack:
			USART_WriteBlocking(ESP_USART, ACK, ACK_SIZE);
			break;

		case nak:
			USART_WriteBlocking(ESP_USART, NAK, ACK_SIZE);
			break;

		default:
			break;
	}
}

/*******************************************************************************
 * Data process task
 ******************************************************************************/
void data_process_task ()
{
	uint8_t packet[MAX_RX_PACKET_SIZE] = {0};
    PacketData ack_stat; 				/* ack or nak */

	APP_tsEvent packetEvent;
	packetEvent.eType = APP_E_EVENT_NONE;

	/* Data parsing process */
	if (get_rest_buf_size() >= MIN_RX_PACKET_SIZE)
	{
		ack_stat = read_packet_from_buf(packet);
		write_response(ack_stat);

		if (ack_stat == ack) {
			// TODO: cast the command to corresponding task thread.
			switch (packet[command])
			{
				case power:
					DBG_vPrintf(TRACE_SERIAL, "Power\r\n");
					packetEvent.eType = (packet[data] == 1)?  APP_E_EVENT_SERIAL_LED_ON
															: APP_E_EVENT_SERIAL_LED_OFF ;
					break;

				case brightness:
					DBG_vPrintf(TRACE_SERIAL, "Brightness\r\n");
					packetEvent.eType = APP_E_EVENT_SERIAL_BRIGHTNESS;
					packetEvent.data  = packet[data];
					break;
			
				case hue:
					DBG_vPrintf(TRACE_SERIAL, "Hue \r\n");
					DBG_vPrintf(TRACE_SERIAL, " ㄴThis LED does not off the Hue.\r\n");
					break; 

				case form:
					DBG_vPrintf(TRACE_SERIAL, "Form\r\n");
					packetEvent.eType = APP_E_EVENT_SERIAL_FORM_NETWORK;
					break;
				
				case steer:
					DBG_vPrintf(TRACE_SERIAL, "Steer\r\n");
					packetEvent.eType = APP_E_EVENT_SERIAL_NWK_STEER;
					break;
				
				case find:
					DBG_vPrintf(TRACE_SERIAL, "Find\r\n");
					packetEvent.eType = APP_E_EVENT_SERIAL_FIND_BIND_START;
					break;

				case hardreset:
					DBG_vPrintf(TRACE_SERIAL, "Factory reset\r\n");
					APP_vFactoryResetRecords();
					MICRO_DISABLE_INTERRUPTS();
					RESET_SystemReset();
					break;
				
				case softreset:
					DBG_vPrintf(TRACE_SERIAL, "Soft reset\r\n");
					MICRO_DISABLE_INTERRUPTS();
					RESET_SystemReset();
					break;
				
				default:
					break;
			}
		}
		if (packetEvent.eType != APP_E_EVENT_NONE)
		{
			if( ZQ_bQueueSend(&APP_msgAppEvents, &packetEvent)  == FALSE)
       		{
            	DBG_vPrintf(1, "Queue Overflow has happened \n");
        	}
		}
	}
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




