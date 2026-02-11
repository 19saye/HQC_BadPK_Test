#include "aes_gcm.h"
#include <openssl/evp.h>
#include <string.h>

int aes_gcm_encrypt(
    const uint8_t *key,
    const uint8_t *iv,
    const uint8_t *plaintext, size_t plaintext_len,
    uint8_t *ciphertext,
    uint8_t *tag
) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return -1;

    int len = 0;
    int ciphertext_len = 0;

    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) return -1;
    if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, AES_GCM_IVLEN, NULL)) return -1;
    if (!EVP_EncryptInit_ex(ctx, NULL, NULL, key, iv)) return -1;

    if (!EVP_EncryptUpdate(ctx, ciphertext, &len, plaintext, plaintext_len)) return -1;
    ciphertext_len = len;

    if (!EVP_EncryptFinal_ex(ctx, ciphertext + len, &len)) return -1;
    ciphertext_len += len;

    if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_GET_TAG, AES_GCM_TAGLEN, tag)) return -1;

    EVP_CIPHER_CTX_free(ctx);
    return ciphertext_len;
}
int aes_gcm_decrypt(
    const uint8_t *key,
    const uint8_t *iv,
    const uint8_t *ciphertext, size_t ciphertext_len,
    const uint8_t *tag,
    uint8_t *plaintext
) {
    EVP_CIPHER_CTX *ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return -1;

    int len = 0;
    int plaintext_len = 0;

    if (!EVP_DecryptInit_ex(ctx, EVP_aes_256_gcm(), NULL, NULL, NULL)) return -1;
    if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_IVLEN, AES_GCM_IVLEN, NULL)) return -1;
    if (!EVP_DecryptInit_ex(ctx, NULL, NULL, key, iv)) return -1;

    if (!EVP_DecryptUpdate(ctx, plaintext, &len, ciphertext, ciphertext_len)) return -1;
    plaintext_len = len;

    if (!EVP_CIPHER_CTX_ctrl(ctx, EVP_CTRL_GCM_SET_TAG, AES_GCM_TAGLEN, (void *)tag)) return -1;

    if (EVP_DecryptFinal_ex(ctx, plaintext + len, &len) <= 0)
        return -1; // authentication failed

    plaintext_len += len;
    EVP_CIPHER_CTX_free(ctx);

    return plaintext_len;
}

