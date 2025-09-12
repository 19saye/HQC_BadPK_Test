#include "crypto.h"
#include <stdio.h>
#include <stdint.h>
#include <string.h>

// api.h include'u Makefile'daki -I yolu ile bulunacak

static void dump8(const char *tag, const uint8_t *p) {
    printf("%s:", tag);
    for (int i = 0; i < 8; i++) printf(" %02X", p[i]);
    printf("\n");
}

int main(void) {
    printf("Hello Day2\n");
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

    if (mock_hqc_keypair(pk, sk) != 0) { printf("keypair fail\n"); return 1; }

    // ---- SELF-CHECK #1:  pk_from_sk = sk ^ 0xA5
    uint8_t pk_from_sk[KEM_PUB_BYTES];
    for (int i = 0; i < KEM_PUB_BYTES; i++) pk_from_sk[i] = sk[i % KEM_SEC_BYTES] ^ 0xA5;
    int pk_eq = (memcmp(pk_from_sk, pk, KEM_PUB_BYTES) == 0);
    dump8("pk     ", pk);
    dump8("pk_calc", pk_from_sk);
    printf("PK selfcheck = %s\n", pk_eq ? "EQUAL" : "DIFF");


    if (mock_hqc_encaps(pk, ct, ss_enc) != 0) { printf("enc fail\n"); return 1; }

    // ---- SELF-CHECK #2ss_exp = ct ^ pk
    uint8_t ss_exp[AEAD_KEY_BYTES];
    for (int i = 0; i < AEAD_KEY_BYTES; i++) ss_exp[i] = ct[i % KEM_CT_BYTES] ^ pk[i % KEM_PUB_BYTES];
    int ss_enc_ok = (memcmp(ss_exp, ss_enc, AEAD_KEY_BYTES) == 0);
    dump8("ss_exp ", ss_exp);
    printf("ENC exp==out = %s\n", ss_enc_ok ? "YES" : "NO");


    if (mock_hqc_decaps(sk, ct, ss_dec) != 0) { printf("dec fail\n"); return 1; }

    // ---- SELF-CHECK #3: 
    int ss_dec_ok = (memcmp(ss_exp, ss_dec, AEAD_KEY_BYTES) == 0);
    printf("DEC exp==out = %s\n", ss_dec_ok ? "YES" : "NO");



    dump8("enc", ss_enc);
    dump8("dec", ss_dec);
    int match = (memcmp(ss_enc, ss_dec, AEAD_KEY_BYTES) == 0);
    printf("baseline match = %s\n", match ? "YES" : "NO");

    // tek bit bozma
    uint8_t pk_bad[KEM_PUB_BYTES];
    memcpy(pk_bad, pk, sizeof pk_bad);
    pk_bad[0] ^= 0x01;

    if (mock_hqc_encaps(pk_bad, ct, ss_enc) != 0) { printf("enc(badpk) fail\n"); return 1; }
    if (mock_hqc_decaps(sk, ct, ss_dec) != 0) { printf("dec(badpk) fail\n"); return 1; }

    dump8("enc_bad", ss_enc);
    dump8("dec_bad", ss_dec);
    int badmatch = (memcmp(ss_enc, ss_dec, AEAD_KEY_BYTES) == 0);
    printf("badpk match = %s\n", badmatch ? "YES" : "NO");

    return 0;
}
