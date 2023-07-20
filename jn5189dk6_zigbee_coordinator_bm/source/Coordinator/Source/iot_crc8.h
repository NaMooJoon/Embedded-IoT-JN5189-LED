#ifndef __IOT_CRC8_h__
#define __IOT_CRC8_h__

#include <stdio.h>

#define CRC8 0b100000111
#define ACK_CHECKSUM1	0x6c
#define ACK_CHECKSUM0	0x6b

/* calculate the CRC-8 checksum */
uint8_t calc_checksum(uint8_t *data, int size);

#endif
