#ifndef AES_GCM_H
#define AES_GCM_H
#include <stdint.h>
#include <stddef.h>
#define AES_GCM_KEYLEN 32   // 256-bit key
#define AES_GCM_IVLEN 12    // 96-bit IV
#define AES_GCM_TAGLEN 16   // 128-bit tag

int aes_gcm_encrypt(
    const uint8_t *key,
    const uint8_t *iv,
    const uint8_t *plaintext, size_t plaintext_len,
    uint8_t *ciphertext,
    uint8_t *tag
);

int aes_gcm_decrypt(
    const uint8_t *key,
    const uint8_t *iv,
    const uint8_t *ciphertext, size_t ciphertext_len,
    const uint8_t *tag,
    uint8_t *plaintext
);

#endif

