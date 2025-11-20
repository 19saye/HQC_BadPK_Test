#include "api.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

//We will not use self-check (pk_from_sk, ss_exp, etc.) in this file; it was mock-specific.

#define KEM_PUB_BYTES PQCLEAN_HQC128_CLEAN_CRYPTO_PUBLICKEYBYTES
#define KEM_SEC_BYTES PQCLEAN_HQC128_CLEAN_CRYPTO_SECRETKEYBYTES
#define KEM_CT_BYTES PQCLEAN_HQC128_CLEAN_CRYPTO_CIPHERTEXTBYTES
#define AEAD_KEY_BYTES PQCLEAN_HQC128_CLEAN_CRYPTO_BYTES
#define kem_keypair PQCLEAN_HQC128_CLEAN_crypto_kem_keypair
#define kem_encaps PQCLEAN_HQC128_CLEAN_crypto_kem_enc
#define kem_decaps PQCLEAN_HQC128_CLEAN_crypto_kem_dec

static void dump8(const char *tag, const uint8_t *p) {
    printf("%s:", tag);
    for (int i = 0; i < 8; i++) printf(" %02X", p[i]);
    printf("\n");
}

int main(void) {
    printf(" HQC-128 BadPK Test \n");
    printf("PK=%d SK=%d CT=%d SS=%d\n",
           KEM_PUB_BYTES,
           KEM_SEC_BYTES,
           KEM_CT_BYTES,
           AEAD_KEY_BYTES);

    uint8_t pk[KEM_PUB_BYTES];
    uint8_t sk[KEM_SEC_BYTES];
    uint8_t ct[KEM_CT_BYTES];
    uint8_t ss_enc[AEAD_KEY_BYTES];
    uint8_t ss_dec[AEAD_KEY_BYTES];

    if (kem_keypair(pk, sk) != 0) { printf("keypair fail\n"); return 1; }



    if (kem_encaps(ct, ss_enc, pk) != 0) { printf("enc fail\n"); return 1; }



    if (kem_decaps(ss_dec, ct, sk) != 0) { printf("dec fail\n"); return 1; }




    dump8("enc", ss_enc);
    dump8("dec", ss_dec);
    int match = (memcmp(ss_enc, ss_dec, AEAD_KEY_BYTES) == 0);
    printf("baseline match = %s\n", match ? "YES" : "NO");

    // tek bit bozma
    uint8_t pk_bad[KEM_PUB_BYTES];
    memcpy(pk_bad, pk, sizeof pk_bad);
    pk_bad[0] ^= 0x01;

    if (kem_encaps(pk_bad, ct, ss_enc) != 0) { printf("enc(badpk) fail\n"); return 1; }
    if (kem_decaps(sk, ct, ss_dec) != 0) { printf("dec(badpk) fail\n"); return 1; }

    dump8("enc_bad", ss_enc);
    dump8("dec_bad", ss_dec);
    int badmatch = (memcmp(ss_enc, ss_dec, AEAD_KEY_BYTES) == 0);
    printf("badpk match = %s\n", badmatch ? "YES" : "NO");

    return 0;
}
