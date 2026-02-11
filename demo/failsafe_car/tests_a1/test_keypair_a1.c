#include "/Users/raabiyakomur/PQClean/crypto_kem/hqc-128/clean/api.h"
#include "/Users/raabiyakomur/PQClean/crypto_kem/hqc-128/clean/parameters.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define CRYPTO_PUBLICKEYBYTES       PQCLEAN_HQC128_CLEAN_CRYPTO_PUBLICKEYBYTES
#define CRYPTO_SECRETKEYBYTES       PQCLEAN_HQC128_CLEAN_CRYPTO_SECRETKEYBYTES
#define CRYPTO_BYTES                PQCLEAN_HQC128_CLEAN_CRYPTO_BYTES
#define CRYPTO_CIPHERTEXTBYTES      PQCLEAN_HQC128_CLEAN_CRYPTO_CIPHERTEXTBYTES
#define crypto_kem_keypair          PQCLEAN_HQC128_CLEAN_crypto_kem_keypair
#define crypto_kem_enc              PQCLEAN_HQC128_CLEAN_crypto_kem_enc
#define crypto_kem_dec              PQCLEAN_HQC128_CLEAN_crypto_kem_dec

int main() {

    printf(" A1 TEST: HQC Keypair Generation \n");

    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];

    printf("[TEST] Generating HQC keypair...\n");

    if (crypto_kem_keypair(pk, sk) != 0) {
        printf("[FAIL] crypto_kem_keypair returned error.\n");
        return 1;
    }
    printf("[PASS] Keypair generated successfullly.\n");
    printf("\n[TEST] Public Key (first 32 bytes):\n");
    for (int i = 0; i < 32; i++) printf("%02X ", pk[i]);
    printf("\n");

    printf("\n[TEST] Secret Key (first 32 bytes):\n");
    for (int i = 0; i < 32; i++) printf("%02X ", sk[i]);
    printf("\n");

    // Check if keys are all zeros — should NEVER be
    int all_zero_pk = 1;
    for (int i = 0; i < CRYPTO_PUBLICKEYBYTES; i++) {
        if (pk[i] != 0) { all_zero_pk = 0; break; }
    }

    int all_zero_sk = 1;
    for (int i = 0; i < CRYPTO_SECRETKEYBYTES; i++) {
        if (sk[i] != 0) { all_zero_sk = 0; break; }
    }

    if (all_zero_pk) {
        printf("[FAIL] Public key is all zeros! Not random.\n");
    } else {
        printf("[PASS] Public key appears random.\n");
    }

    if (all_zero_sk) {
        printf("[FAIL] Secret key is all zeros! Not random.\n");
    } else {
        printf("[PASS] Secret key appears random.\n");
    }

    printf("\n A1 HQC Keypair Test Complete \n");

   // Basic entropy estimation (count number of zero bytes)
    int zero_count = 0;
    for (int i = 0; i < CRYPTO_PUBLICKEYBYTES; i++) {
         if (pk[i] == 0) zero_count++;
    }
 
    printf("\n[TEST] Public key zero-byte ratio: %.2f%%\n",
       (100.0 * zero_count) / CRYPTO_PUBLICKEYBYTES);

    if (zero_count > CRYPTO_PUBLICKEYBYTES * 0.30)
        printf("[WARN] High zero-byte ratio (lower entropy suspected)\n");
    else
        printf("[PASS] Entropy looks acceptable.\n");

    return 0;

}

