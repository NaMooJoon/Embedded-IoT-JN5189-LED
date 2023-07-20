
#ifndef __IOT_USART_H__
#define __IOT_USART_H__

/* USART */
#define ESP_USART USART1
#define ESP_USART_CLK_SRC kCLOCK_Fro32M
#define ESP_USART_CLK_FREQ CLOCK_GetFreq(ESP_USART_CLK_SRC)
#define ESP_USART_IRQn USART1_IRQn
#define ESP_USART_IRQHandler FLEXCOMM1_IRQHandler
#define ESP_USART_BAUDRATE 115200U

/* Ring buffer */
#define RING_BUFFER_SIZE 128
#define ACK_SIZE 7

#define MIN_RX_PACKET_SIZE 10
#define MAX_RX_PACKET_SIZE 16

extern uint8_t ring_buffer[];
extern volatile uint16_t front_idx; 				/* Index of the data to send out. */
extern volatile uint16_t back_idx; 					/* Index of the memory to save new arrived data. */

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
	form,
	steer,
	find,
	hardreset,
	softreset,
	N_command
} PacketData;

typedef enum packet_data_type {
	size = 0x00,
	command,
	target,
	data,
} PacketDataType;


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


void initializeUsart(void);

/* Interrupt Handler for ESP-NXP USART RX port*/
void ESP_USART_IRQHandler (void);

/* data_process */
PacketData read_packet_from_buf (uint8_t *packet, uint8_t next_seq);
void write_response (uint8_t seq);
void deliver_command(uint8_t *packet);

/* Circular buffer (queue) API*/
uint8_t pop_buf ();
uint8_t peek_buf ();
void push_buf (uint8_t byte);
uint8_t get_rest_buf_size ();				/* Get the size number of remain data in buffer */
void flush_buf_upto_dle ();

void data_process_task();

#endif

