#include "base58.h"
#include <stdlib.h>
#include <string.h>

static const char* base58_alphabet = "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz";

char* base58_encode(const uint8_t* data, size_t length) {
    size_t size = length * 138 / 100 + 1;
    char* buffer = calloc(size + 1, 1); // +1 for null terminator
    if (!buffer) return NULL;

    uint8_t* tmp = malloc(length);
    if (!tmp) {
        free(buffer);
        return NULL;
    }
    memcpy(tmp, data, length);

    size_t zcount = 0;
    while (zcount < length && tmp[zcount] == 0) {
        zcount++;
    }

    ssize_t idx = size - 1;
    size_t high = length;
    while (zcount < high) {
        uint32_t carry = 0;
        for (size_t i = zcount; i < high; i++) {
            carry = (carry << 8) + tmp[i];
            tmp[i] = carry / 58;
            carry %= 58;
        }
        while (zcount < high && tmp[zcount] == 0) {
            zcount++;
        }
        if (idx < 0) {
            fprintf(stderr, "Error: Buffer overflow in base58_encode.\n");
            free(tmp);
            free(buffer);
            return NULL;
        }
        buffer[idx--] = base58_alphabet[carry];
    }

    // Leading zeros
    while (idx >= 0 && zcount--) {
        buffer[idx--] = base58_alphabet[0];
    }

    char* result = strdup(&buffer[idx + 1]);
    free(tmp);
    free(buffer);
    return result;
}

uint8_t* base58_decode(const char* data, size_t* out_length) {
    size_t len = strlen(data);
    size_t size = len * 733 / 1000 + 1;
    uint8_t* buffer = calloc(size, 1);
    if (!buffer) return NULL;

    size_t zcount = 0;
    while (zcount < len && data[zcount] == base58_alphabet[0]) {
        zcount++;
    }

    for (size_t i = zcount; i < len; i++) {
        const char* ch = strchr(base58_alphabet, data[i]);
        if (!ch) {
            free(buffer);
            return NULL;
        }
        int c = ch - base58_alphabet;

        uint32_t carry = c;
        for (ssize_t j = size - 1; j >= 0; j--) {
            carry += 58 * buffer[j];
            buffer[j] = carry & 0xFF;
            carry >>= 8;
        }
        if (carry != 0) {
            fprintf(stderr, "Error: Buffer overflow in base58_decode.\n");
            free(buffer);
            return NULL;
        }
    }

    // Skip leading zeros in buffer
    size_t i = 0;
    while (i < size && buffer[i] == 0) {
        i++;
    }

    *out_length = size - i + zcount;
    uint8_t* decoded = malloc(*out_length);
    if (!decoded) {
        free(buffer);
        return NULL;
    }
    memset(decoded, 0, zcount);
    memcpy(decoded + zcount, buffer + i, size - i);

    free(buffer);
    return decoded;
}
