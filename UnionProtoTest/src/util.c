#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "util.h"

bool hex_to_bytes(char* hex, uint8_t *buffer) {

    if(hex == NULL) 
       return false;

    size_t hex_length = strlen(hex);
    if((hex_length % 2) != 0) // must be even
       return false;

    size_t buffer_length = hex_length / 2;

    memset(buffer, 0, buffer_length);

    size_t index = 0;
    while (index < hex_length) {
        char c = hex[index];
        int value = 0;
        if(c >= '0' && c <= '9')
          value = (c - '0');
        else if (c >= 'A' && c <= 'F') 
          value = (10 + (c - 'A'));
        else if (c >= 'a' && c <= 'f')
          value = (10 + (c - 'a'));
        else {
          return false;
        }

        buffer[(index/2)] += value << (((index + 1) % 2) * 4);
        index++;
    }
    return true;
}

void int64_to_hex_proper_endian(int64_t number, char hex[17]){
    uint8_t *p = (uint8_t *)&number;
    unsigned char hex_str[]= "0123456789abcdef";
    memset(hex, 0, 17);
    for(int i = 0; i < 8; i++) {
        //to maintain proper endianness read buffer 'p' from the other end 
        hex[i * 2 + 0] = hex_str[(p[7-i] >> 4) & 0x0F];
        hex[i * 2 + 1] = hex_str[(p[7-i]) & 0x0F];
    }
}

void uint64_to_hex_proper_endian(uint64_t number, char hex[17]){
    uint8_t *p = (uint8_t *)&number;
    unsigned char hex_str[]= "0123456789abcdef";
    memset(hex, 0, 17);
    for(int i = 0; i < 8; i++) {
        //to maintain proper endianness read buffer 'p' from the other end 
        hex[i * 2 + 0] = hex_str[(p[7-i] >> 4) & 0x0F];
        hex[i * 2 + 1] = hex_str[(p[7-i]) & 0x0F];
    }
}
