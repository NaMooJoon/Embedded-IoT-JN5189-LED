#include "iot_crc8.h"


uint8_t calc_checksum(uint8_t *data, int size){
    uint8_t result = 0;
    for(int i=0; i < size; i++){
        result = crc_table[result ^ data[i]];
    }
    return result;
}
