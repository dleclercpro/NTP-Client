#include "packets.h"

void showPacket(packet *pkt) {

	// Show packet contents
	printf("---------- PACKET (BEGIN) ----------\n");
	printf("Leap indicator: %d\n", pkt -> li);
	printf("Version: %d\n", pkt -> vn);
	printf("Mode: %d\n", pkt -> mode);
	printf("Stratum: %d\n", pkt -> stratum);
	printf("Poll: %d\n", pkt -> poll);
	printf("Precision: %d\n", pkt -> precision);
	printf("Root delay: %u.%u\n", pkt -> root_delay_seconds, pkt -> root_delay_fraction);
	printf("Root dispersion: %u.%u\n", pkt -> root_dispersion_seconds, pkt -> root_dispersion_fraction);
	printf("Reference ID: %d\n", pkt -> reference_id);
	printf("Reference timestamp: %u.%u\n", pkt -> reference_ts_seconds, pkt -> reference_ts_fraction);
	printf("Origin timestamp: %u.%u\n", pkt -> origin_ts_seconds, pkt -> origin_ts_fraction);
	printf("Receive timestamp: %u.%u\n", pkt -> receive_ts_seconds, pkt -> receive_ts_fraction);
	printf("Transmit timestamp: %u.%u\n", pkt -> transmit_ts_seconds, pkt -> transmit_ts_fraction);
	printf("---------- PACKET (END) ----------\n");

	// New line
	printf("\n");
}

void initPacket(packet *pkt, double t) {

	// Define packet bytes
	pkt -> li = 0;
	pkt -> vn = 4;
	pkt -> mode = 3;
	pkt -> stratum = 0;
	pkt -> poll = 0;
	pkt -> precision = 0;
	pkt -> root_delay_seconds = 0;
	pkt -> root_delay_fraction = 0;
	pkt -> root_dispersion_seconds = 0;
	pkt -> root_dispersion_fraction = 0;
	pkt -> reference_id = 0;
	pkt -> reference_ts_seconds = 0;
	pkt -> reference_ts_fraction = 0;
	pkt -> origin_ts_seconds = 0;
	pkt -> origin_ts_fraction = 0;
	pkt -> receive_ts_seconds = 0;
	pkt -> receive_ts_fraction = 0;
	pkt -> transmit_ts_seconds = doubleToSeconds(t);
	pkt -> transmit_ts_fraction = doubleToFraction(t);
}

void packetToBuffer(packet *pkt, uint8_t *buffer) {

	// Pack buffer with packet
	buffer[0] = (pkt -> li) << 6 | (pkt -> vn) << 3 | (pkt -> mode) << 0;
	buffer[1] = pkt -> stratum;
	buffer[2] = pkt -> poll;
	buffer[3] = pkt -> precision;
	packBytes(buffer + 4, 2, pkt -> root_delay_seconds);
	packBytes(buffer + 6, 2, pkt -> root_delay_fraction);
	packBytes(buffer + 8, 2, pkt -> root_dispersion_seconds);
	packBytes(buffer + 10, 2, pkt -> root_dispersion_fraction);
	packBytes(buffer + 12, 4, pkt -> reference_id);

	// Timestamps
	packBytes(buffer + 16, 4, pkt -> reference_ts_seconds);
	packBytes(buffer + 20, 4, pkt -> reference_ts_fraction);
	packBytes(buffer + 24, 4, pkt -> origin_ts_seconds);
	packBytes(buffer + 28, 4, pkt -> origin_ts_fraction);
	packBytes(buffer + 32, 4, pkt -> receive_ts_seconds);
	packBytes(buffer + 36, 4, pkt -> receive_ts_fraction);
	packBytes(buffer + 40, 4, pkt -> transmit_ts_seconds);
	packBytes(buffer + 44, 4, pkt -> transmit_ts_fraction);
}

void bufferToPacket(packet *pkt, uint8_t *buffer) {

	// Unpack buffer to packet
	pkt -> li = (buffer[0] & LI) >> 6;
	pkt -> vn = (buffer[0] & VN) >> 3;
	pkt -> mode = (buffer[0] & MODE) >> 0;
	pkt -> stratum = buffer[1];
	pkt -> poll = buffer[2];
	pkt -> precision = buffer[3];
	pkt -> root_delay_seconds = unpackBytes(buffer + 4, 2);
	pkt -> root_delay_fraction = unpackBytes(buffer + 6, 2);
	pkt -> root_dispersion_seconds = unpackBytes(buffer + 8, 2);
	pkt -> root_dispersion_fraction = unpackBytes(buffer + 10, 2);
	pkt -> reference_id = unpackBytes(buffer + 12, 4);

	// Timestamps
	pkt -> reference_ts_seconds = unpackBytes(buffer + 16, 4);
	pkt -> reference_ts_fraction = unpackBytes(buffer + 20, 4);
	pkt -> origin_ts_seconds = unpackBytes(buffer + 24, 4);
	pkt -> origin_ts_fraction = unpackBytes(buffer + 28, 4);
	pkt -> receive_ts_seconds = unpackBytes(buffer + 32, 4);
	pkt -> receive_ts_fraction = unpackBytes(buffer + 36, 4);
	pkt -> transmit_ts_seconds = unpackBytes(buffer + 40, 4);
	pkt -> transmit_ts_fraction = unpackBytes(buffer + 44, 4);
}
