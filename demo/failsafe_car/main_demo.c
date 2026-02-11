#include "aes_gcm.h"
#include "/Users/raabiyakomur/PQClean/crypto_kem/hqc-128/clean/api.h"
#include <stdio.h>
#include <string.h>
#include "packet_format.h"
#include "failsafe_ecu.h"

int main(void) {

    printf("=== Fail-Safe PQC Connected Car Demo ===\n");

    // HQC değişkenleri

    uint8_t pk[PQCLEAN_HQC128_CLEAN_CRYPTO_PUBLICKEYBYTES];
    uint8_t sk[PQCLEAN_HQC128_CLEAN_CRYPTO_SECRETKEYBYTES];

    uint8_t ss[PQCLEAN_HQC128_CLEAN_CRYPTO_BYTES];
    uint8_t ss_dec[PQCLEAN_HQC128_CLEAN_CRYPTO_BYTES];

    uint8_t ct[PQCLEAN_HQC128_CLEAN_CRYPTO_CIPHERTEXTBYTES];

    // 1) Keypair Üretimi

    printf("[PQC] Generating HQC keypair...\n");
    PQCLEAN_HQC128_CLEAN_crypto_kem_keypair(pk, sk);
    printf("[PQC] Keypair generated.\n");

    // ECU başlatma
    ecu_context_t ecu;
    uint32_t expected_token = 0xDEADBEEF;
    ecu_init(&ecu, expected_token);

    const char *dummy_data = "sensor: speed=42;";
    secure_packet_t pkt;

    packet_fill(&pkt, expected_token,
                (const uint8_t *)dummy_data,
                (uint16_t)strlen(dummy_data));

    // 2) ENCRYPT

    printf("[PQC] Encrypting shared secret with HQC...\n");
    PQCLEAN_HQC128_CLEAN_crypto_kem_enc(ct, ss, pk);
    printf("[PQC] Ciphertext generated (%d bytes).\n",
           PQCLEAN_HQC128_CLEAN_CRYPTO_CIPHERTEXTBYTES);

    // 3) ECU den  DECRYPT

    printf("[PQC] Decrypting on ECU...\n");
    PQCLEAN_HQC128_CLEAN_crypto_kem_dec(ss_dec, ct, sk);

    if (memcmp(ss, ss_dec, PQCLEAN_HQC128_CLEAN_CRYPTO_BYTES) != 0) {
        printf("[PQC] Shared secret mismatch! FAILSAFE TRIGGERED.\n");
    } else {
        printf("[PQC] Shared secret verified OK.\n");
    }


    // === AES-GCM payload encryption ===
   uint8_t aes_key[32]; 
   uint8_t aes_iv[12];
   memcpy(aes_key, ss, 32);       // First 32 bytes → AES-256 key
   memcpy(aes_iv, ss + 32, 12);   // Next 12 bytes → IV
   uint8_t cipher[256];
   uint8_t tag[16];
   const uint8_t *plain = (const uint8_t *)dummy_data;
   size_t plain_len = strlen(dummy_data);

   int enc_ok = aes_gcm_encrypt(aes_key, aes_iv,
                             plain, plain_len,
                             cipher, tag);

   if (!enc_ok) {
        printf("[AES] Encryption failed! FAILSAFE.\n");
   } else {
        printf("[AES] AES-GCM encrypted sensor payload (%zu bytes).\n", plain_len);
   }


    // ECU state machine
    ecu_handle_packet(&ecu, &pkt);

    if (ecu_is_failsafe(&ecu)) {
        printf("[ECU] State: FAILSAFE\n");
    } else {
        printf("[ECU] State: NORMAL\n");
    }

    return 0;
}

