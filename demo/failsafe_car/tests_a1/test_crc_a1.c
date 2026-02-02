#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include "packet_format_tests_a1.h"

int main(void) {

    printf("\n A3 TEST: CRC32 INTEGRITY CHECK \n");

    const char *msg1 = "hello-world";
    const char *msg2 = "hello-world!";
    const char *msg3 = "hello-world";

    uint32_t crc1 = packet_compute_crc32((const uint8_t*)msg1, strlen(msg1));
    uint32_t crc2 = packet_compute_crc32((const uint8_t*)msg2, strlen(msg2));
    uint32_t crc1_again = packet_compute_crc32((const uint8_t*)msg3, strlen(msg3));

    int pass = 1;

    // SAME INPUT → SAME CRC
    if (crc1 == crc1_again) {
        printf("[PASS] Same input → same CRC.\n");
    } else {
        printf("[FAIL] Same input produced DIFFERENT CRC!\n");
        pass = 0;
    }

    // DIFFERENT INPUT → DIFFERENT CRC
    if (crc1 != crc2) {
        printf("[PASS] Different inputs → different CRC.\n");
    } else {
        printf("[FAIL] Different inputs produced SAME CRC!\n");
        pass = 0;
    }

    printf("\nA3 CRC TEST %s\n", pass ? "COMPLETE (PASS)" : "COMPLETE (FAIL)");
    return pass ? 0 : 1;
}

