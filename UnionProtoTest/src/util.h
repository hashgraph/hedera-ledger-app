#include <stdint.h>
#include <stdbool.h>

bool hex_to_bytes(char* hex, uint8_t *buffer);

void uint64_to_hex_proper_endian(uint64_t number, char hex[17]);