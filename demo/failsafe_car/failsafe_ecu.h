#ifndef FAILSAFE_ECU_H
#define FAILSAFE_ECU_H

#include <stdint.h>
#include "packet_format.h"

typedef enum {
    ECU_STATE_NORMAL = 0,
    ECU_STATE_FAILSAFE = 1
} ecu_state_t;

typedef struct {
    ecu_state_t state;
    uint32_t expected_auth_token;
} ecu_context_t;

void ecu_init(ecu_context_t *ctx, uint32_t expected_auth_token);
void ecu_handle_packet(ecu_context_t *ctx, const secure_packet_t *pkt);
int  ecu_is_failsafe(const ecu_context_t *ctx);

#endif

