#include "failsafe_ecu_tests_a1.h"

void ecu_init(ecu_context_t *ctx, uint32_t expected_auth_token) {
    ctx->state = ECU_STATE_NORMAL;
    ctx->expected_auth_token = expected_auth_token;
}

void ecu_handle_packet(ecu_context_t *ctx, const secure_packet_t *pkt) {
    // Auth check
    if (pkt->auth_token != ctx->expected_auth_token) {
        ctx->state = ECU_STATE_FAILSAFE;
        return;
    }

    // CRC check
    if (!packet_verify_crc32(pkt)) {
        ctx->state = ECU_STATE_FAILSAFE;
        return;
    }
}

int ecu_is_failsafe(const ecu_context_t *ctx) {
    return (ctx->state == ECU_STATE_FAILSAFE);
}

