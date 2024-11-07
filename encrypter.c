#include "encrypter.h"
#include "aes.h"
#include "base58.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

static uint8_t AESKey[16]; // 128-bit key
static uint8_t AESIV[16];  // Initialization vector
static struct AES_ctx aes_ctx;

void Encrypter_Init(const unsigned char* key, size_t key_length, const unsigned char* iv, size_t iv_length) {
    // For AES-128, both key and IV must be 16 bytes
    if (key_length != 16 || iv_length != 16) {
        fprintf(stderr, "Error: Key and IV must be 16 bytes long.\n");
        exit(EXIT_FAILURE);
    }
    memcpy(AESKey, key, 16);
    memcpy(AESIV, iv, 16);
    AES_init_ctx_iv(&aes_ctx, AESKey, AESIV);
}

static void PKCS7_Pad(uint8_t* buffer, size_t data_len, size_t buffer_size) {
    uint8_t padding_value = buffer_size - data_len;
    for (size_t i = data_len; i < buffer_size; ++i) {
        buffer[i] = padding_value;
    }
}

static size_t PKCS7_Unpad(uint8_t* buffer, size_t buffer_size) {
    uint8_t padding_value = buffer[buffer_size - 1];
    if (padding_value == 0 || padding_value > 16) {
        // Invalid padding
        fprintf(stderr, "Error: Invalid PKCS7 padding.\n");
        return buffer_size; // Return original size if padding is invalid
    }
    // Ensure all padding bytes are correct
    for (size_t i = 0; i < padding_value; i++) {
        if (buffer[buffer_size - 1 - i] != padding_value) {
            fprintf(stderr, "Error: Invalid PKCS7 padding.\n");
            return buffer_size;
        }
    }
    return buffer_size - padding_value;
}

char* EncryptMessage(const char* plaintext) {
    // Calculate padded length
    size_t len = strlen(plaintext);
    size_t padded_len = ((len + 15) / 16) * 16;
    uint8_t* padded_plaintext = calloc(1, padded_len);
    if (!padded_plaintext) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        return NULL;
    }
    memcpy(padded_plaintext, plaintext, len);
    PKCS7_Pad(padded_plaintext, len, padded_len);

    // Encrypt the plaintext
    AES_CBC_encrypt_buffer(&aes_ctx, padded_plaintext, padded_len);

    // Base58 encode
    char* base58_encoded = base58_encode(padded_plaintext, padded_len);
    free(padded_plaintext);

    if (!base58_encoded) {
        fprintf(stderr, "Error: Base58 encoding failed.\n");
        return NULL;
    }

    return base58_encoded;
}

char* DecryptMessage(const char* ciphertext) {
    // Base58 decode
    size_t decoded_len;
    uint8_t* decoded_data = base58_decode(ciphertext, &decoded_len);
    if (!decoded_data) {
        fprintf(stderr, "Error: Base58 decoding failed.\n");
        return NULL;
    }

    // Decrypt the data
    AES_CBC_decrypt_buffer(&aes_ctx, decoded_data, decoded_len);

    // Remove padding
    size_t actual_len = PKCS7_Unpad(decoded_data, decoded_len);

    // Null-terminate the plaintext
    char* plaintext = calloc(1, actual_len + 1);
    if (!plaintext) {
        fprintf(stderr, "Error: Memory allocation failed.\n");
        free(decoded_data);
        return NULL;
    }
    memcpy(plaintext, decoded_data, actual_len);
    plaintext[actual_len] = '\0';

    free(decoded_data);
    return plaintext;
}

char* EncryptIP(const char* ip_string) {
    return EncryptMessage(ip_string);
}

char* DecryptIP(const char* encrypted_ip) {
    return DecryptMessage(encrypted_ip);
}
