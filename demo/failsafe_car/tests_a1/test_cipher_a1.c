#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "/Users/raabiyakomur/PQClean/crypto_kem/hqc-128/clean/api.h"

#define CRYPTO_PUBLICKEYBYTES PQCLEAN_HQC128_CLEAN_CRYPTO_PUBLICKEYBYTES
#define CRYPTO_SECRETKEYBYTES PQCLEAN_HQC128_CLEAN_CRYPTO_SECRETKEYBYTES
#define CRYPTO_BYTES PQCLEAN_HQC128_CLEAN_CRYPTO_BYTES
#define CRYPTO_CIPHERTEXTBYTES PQCLEAN_HQC128_CLEAN_CRYPTO_CIPHERTEXTBYTES

#define crypto_kem_keypair PQCLEAN_HQC128_CLEAN_crypto_kem_keypair
#define crypto_kem_enc PQCLEAN_HQC128_CLEAN_crypto_kem_enc
#define crypto_kem_dec PQCLEAN_HQC128_CLEAN_crypto_kem_dec

int main() {
    printf("A2 TEST :HQC ENCRYPTION AND SHARED SECRET \n");
    uint8_t pk[CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[CRYPTO_SECRETKEYBYTES];
    uint8_t ss[CRYPTO_BYTES];
    uint8_t ss_dec[CRYPTO_BYTES];
    uint8_t ct[CRYPTO_CIPHERTEXTBYTES];

    crypto_kem_keypair(pk, sk);
    crypto_kem_enc(ct, ss, pk);
    crypto_kem_dec(ss_dec, ct, sk);


    printf("[TEST] Ciphertext (first 32 bytes): \n");
    for(int i =0; i<32; i++) printf("%02X", ct[i]);
    printf("\n");
   
    printf("\n[TEST] Shared Secret (Encrypt):\n");
    for(int i=0; i<32; i++) printf("%02X", ss[i]);
    printf("\n");

    printf("\n[TEST] Shared Secret (Decrypt): \n");
    for(int i=0; i<32; i++) printf("%02X", ss_dec[i]);
    printf("\n");

    if (memcmp(ss, ss_dec, CRYPTO_BYTES) == 0) {
       printf("[PASS]Shared Secret verified. Encryption/Decryption OK.\n");
    }else {
       printf("[FAIL] Shared Secret mismatchh! HQC decapsulation FAILED.\n");
    }

    printf("\n A2 HQC CIPHER TEST COMPLETE \n");
   

    return 0;

}

