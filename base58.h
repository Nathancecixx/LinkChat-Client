#ifndef BASE58_H
#define BASE58_H

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

char* base58_encode(const uint8_t* data, size_t length);
uint8_t* base58_decode(const char* data, size_t* out_length);

#endif // BASE58_H
