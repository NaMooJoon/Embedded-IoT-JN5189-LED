/*System includes.*/
#include <stdio.h>

#include "fsl_usart.h"
#include "fsl_debug_console.h"

/* IoT System includes */
#include "iot_usart.h"

/* ESP-NXP receive buffer */
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
PacketData read_packet_from_buf (RxDataPacket *packet)
{
	uint8_t checksum[2];

	/* Find start byte */
	if (pop_buf() != dle) 		goto __flush_packet_upto_dle__;
	if (pop_buf() != start) 	goto __flush_packet_upto_dle__;

	/* Read command */
	packet->target = pop_buf();
	packet->command = pop_buf();

	/* Read data*/
	packet->size = pop_buf();
	for (int i = 0; i < packet->size; i++) {
		packet->data[i] = pop_buf();
	}

	/* checksum */
	checksum[0] = pop_buf();
	checksum[1] = pop_buf();
	// TODO: check the checksum number (CRC 16)


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

void print_packet_data (RxDataPacket* packet)
{
	PRINTF("****** Packet request ******\n\r");
	PRINTF("  - Front_idx: %d, Back_idx: %d\n\r", front_idx, back_idx);

	PRINTF("  - Target: %d, command:%d \r\n", packet->target, packet->command);
	PRINTF("  - Size: %d\r\n", packet->size);
	PRINTF("****** Packet received *****\n\r\n\r");
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



