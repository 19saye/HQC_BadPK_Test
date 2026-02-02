#include "packet_format_tests_a1.h"
#include <string.h>
#include <stdint.h>

uint32_t packet_compute_crc32(const uint8_t *data, size_t len) {
    uint32_t crc=0xFFFFFFF;
 
    for (size_t i=0; i<len; i++) {
        crc ^= data[i];
        for (size_t j=0; j<8; j++) {
            uint32_t mask= -(crc & 1);
            crc = (crc >> 1) ^(0xEDB88320 & mask);
       }
   }
  
   return ~crc;
}

/* crc Verification */

int packet_verify_crc32(const secure_packet_t *pkt) {
    uint32_t calc = packet_compute_crc32(pkt->payload, pkt->payload_len);
    return (calc == pkt->crc32);
}

/* Paket doğrulama(veri + crc + yetki )  */

void packet_fill(secure_packet_t *pkt, uint32_t auth_token,
                 const uint8_t *data, uint16_t len)

{
    pkt->auth_token = auth_token;
    pkt->payload_len = len;
    memcpy(pkt->payload, data, len);
    pkt->crc32 = packet_compute_crc32(data, len);
}

