#include <stdio.h>
#include <stdint.h>

#define PQCLEAN_HQC128_CLEAN
#include "api.h"

int main() {
    uint8_t pk[PQCLEAN_HQC128_CLEAN_CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[PQCLEAN_HQC128_CLEAN_CRYPTO_SECRETKEYBYTES];

    PQCLEAN_HQC128_CLEAN_crypto_kem_keypair(pk, sk);
    printf("✅ Anahtar üretildi!\n");

    return 0;
}








 
