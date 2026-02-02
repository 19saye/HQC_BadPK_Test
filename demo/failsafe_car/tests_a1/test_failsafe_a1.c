#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "failsafe_ecu_tests_a1.h"
#include "packet_format_tests_a1.h"

int main(void) {

    printf("\n A4 TEST: FAILSAFE INJECTION SIMULATION \n");

    ecu_context_t ecu;
    uint32_t legit_token = 0xDEADBEEF;

    ecu_init(&ecu, legit_token);

    secure_packet_t good_pkt;
    const char *good_data = "sensor: temp=25";

    packet_fill(&good_pkt,
                legit_token,
                (const uint8_t*)good_data,
                (uint16_t)strlen(good_data));

    ecu_handle_packet(&ecu, &good_pkt);

    if (ecu_is_failsafe(&ecu)) {
        printf("[FAIL] Valid packet triggered FAILSAFE (should NOT happen)\n");
        return 1;
    } else {
        printf("[PASS] Valid packet accepted normally.\n");
    }

    secure_packet_t bad_pkt;
    const char *bad_data = "sensor: inject_attack";

    packet_fill(&bad_pkt,
                0x12345678,   // yanlış token
                (const uint8_t*)bad_data,
                (uint16_t)strlen(bad_data));

    ecu_handle_packet(&ecu, &bad_pkt);

    if (ecu_is_failsafe(&ecu)) {
        printf("[PASS] Invalid packet triggered FAILSAFE as expected.\n");
    } else {
        printf("[FAIL] Invalid packet did NOT trigger FAILSAFE.\n");
        return 1;
    }

    printf("\n A4 FAILSAFE TEST COMPLETE (PASS) \n");
    return 0;
}

