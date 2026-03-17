#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "api.h"

#define FIRMWARE_SIZE 64

static void print_hex(const char *tag, const uint8_t *data, size_t len) {
    printf("%s: ", tag);
    for (size_t i = 0; i < len && i < 8; i++)
        printf("%02X ", data[i]);
    printf("...\n");
}

int main(void) {
    printf("=== PQC OTA Update Demo (ML-DSA-44 / Dilithium) ===\n\n");

    /* --- ANAHTAR ÜRETME (Porsche sunucu tarafı) --- */
    uint8_t pk[PQCLEAN_MLDSA44_CLEAN_CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[PQCLEAN_MLDSA44_CLEAN_CRYPTO_SECRETKEYBYTES];

    PQCLEAN_MLDSA44_CLEAN_crypto_sign_keypair(pk, sk);
    print_hex("[SERVER] Public key ", pk, PQCLEAN_MLDSA44_CLEAN_CRYPTO_PUBLICKEYBYTES);
    printf("  PK size : %d bytes\n", PQCLEAN_MLDSA44_CLEAN_CRYPTO_PUBLICKEYBYTES);
    printf("  SK size : %d bytes\n\n", PQCLEAN_MLDSA44_CLEAN_CRYPTO_SECRETKEYBYTES);

    /* --- FIRMWARE PAKETI (simüle) --- */
    uint8_t firmware[FIRMWARE_SIZE];
    for (int i = 0; i < FIRMWARE_SIZE; i++)
        firmware[i] = (uint8_t)(i + 1);
    print_hex("[SERVER] Firmware   ", firmware, FIRMWARE_SIZE);

    /* --- İMZALAMA (Porsche sunucu tarafı) --- */
    uint8_t sig[PQCLEAN_MLDSA44_CLEAN_CRYPTO_BYTES];
    size_t  sig_len = 0;

    PQCLEAN_MLDSA44_CLEAN_crypto_sign_signature(sig, &sig_len, firmware, FIRMWARE_SIZE, sk);
    print_hex("[SERVER] Signature  ", sig, sig_len);
    printf("  Sig size: %zu bytes\n\n", sig_len);

    /* --- DOĞRULAMA (Araç ECU tarafı) --- */
    printf("[ECU] Verifying firmware signature...\n");
    int result = PQCLEAN_MLDSA44_CLEAN_crypto_sign_verify(sig, sig_len, firmware, FIRMWARE_SIZE, pk);

    if (result == 0) {
        printf("[ECU] Signature VALID   -> Firmware accepted, installing...\n");
    } else {
        printf("[ECU] Signature INVALID -> Firmware REJECTED, failsafe triggered.\n");
        return 1;
    }

    /* --- SALDIRI SENARYOSU: Tampered Firmware --- */
    printf("\n[ATTACK] Tampering with firmware (bit flip)...\n");
    uint8_t firmware_tampered[FIRMWARE_SIZE];
    memcpy(firmware_tampered, firmware, FIRMWARE_SIZE);
    firmware_tampered[0] ^= 0x01;

    result = PQCLEAN_MLDSA44_CLEAN_crypto_sign_verify(sig, sig_len, firmware_tampered, FIRMWARE_SIZE, pk);

    if (result == 0) {
        printf("[ECU] Signature VALID   -> (BUG: tampered firmware accepted!)\n");
    } else {
        printf("[ECU] Signature INVALID -> Tampered firmware REJECTED.\n");
    }

    /* --- SALDIRI SENARYOSU: Sahte İmza --- */
    printf("\n[ATTACK] Sending forged signature...\n");
    uint8_t sig_forged[PQCLEAN_MLDSA44_CLEAN_CRYPTO_BYTES];
    memset(sig_forged, 0xAB, sizeof(sig_forged));

    result = PQCLEAN_MLDSA44_CLEAN_crypto_sign_verify(sig_forged, sig_len, firmware, FIRMWARE_SIZE, pk);

    if (result == 0) {
        printf("[ECU] Signature VALID   -> (BUG: forged signature accepted!)\n");
    } else {
        printf("[ECU] Signature INVALID -> Forged signature REJECTED.\n");
    }

    printf("\n=== Demo complete ===\n");
    return 0;
}
