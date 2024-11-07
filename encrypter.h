#ifndef ENCRYPTER_H
#define ENCRYPTER_H

#include <stddef.h>

// Initialize the encrypter with a given key and IV
void Encrypter_Init(const unsigned char* key, size_t key_length, const unsigned char* iv, size_t iv_length);

// Encrypt a message and return a base64-encoded string
char* EncryptMessage(const char* plaintext);

// Decrypt a base64-encoded message
char* DecryptMessage(const char* ciphertext);

// Encrypt an IP string into a short code
char* EncryptIP(const char* ip_string);

// Decrypt the short code back into an IP string
char* DecryptIP(const char* encrypted_ip);

#endif // ENCRYPTER_H
