#ifndef _PACKETS_H_
#define _PACKETS_H_

#include "constants.h"
#include "lib.h"

typedef struct {
    uint8_t li;
    uint8_t vn;
    uint8_t mode;
    uint8_t stratum;
    uint8_t poll;
    uint8_t precision;
    uint16_t root_delay_seconds;
    uint16_t root_delay_fraction;
    uint16_t root_dispersion_seconds;
    uint16_t root_dispersion_fraction;
    uint32_t reference_id;
    uint32_t reference_ts_seconds;
    uint32_t reference_ts_fraction;
    uint32_t origin_ts_seconds;
    uint32_t origin_ts_fraction;
    uint32_t receive_ts_seconds;
    uint32_t receive_ts_fraction;
    uint32_t transmit_ts_seconds;
    uint32_t transmit_ts_fraction;
} packet;

void showPacket(packet *pkt);
void initPacket(packet *pkt, double t);
void packetToBuffer(packet *pkt, uint8_t *buffer);
void bufferToPacket(packet *pkt, uint8_t *buffer);

#endif
