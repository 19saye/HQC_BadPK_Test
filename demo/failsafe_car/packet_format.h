#ifndef PACKET_FORMAT_H
#define PACKET_FORMAT_H

#include <stdint.h>
#include <stddef.h>

#define MAX_PAYLOAD_LEN 64

typedef struct {
    uint32_t auth_token;
    uint16_t payload_len;
    uint8_t  payload[MAX_PAYLOAD_LEN];
    uint32_t crc32;
} secure_packet_t;

uint32_t packet_compute_crc32(const uint8_t *data, size_t len);
int      packet_verify_crc32(const secure_packet_t *pkt);
void     packet_fill(secure_packet_t *pkt,
                     uint32_t auth_token,
                     const uint8_t *data,
                     uint16_t len);

#endif

