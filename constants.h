#ifndef _CONSTANTS_H_
#define _CONSTANTS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <time.h>
#include <float.h>
#include <math.h>

#define LOCALHOST               "127.0.0.1"
#define PORT                    "123"
#define BUFFER_SIZE             48         // Number of bytes in packets
#define TIME_OFFSET             2208988800 // Time offset between UNIX and NTP time (s)
#define N_REQUESTS              100
#define N_MAX_SERVERS           100
#define FRACTION_PRECISION      9          // Time as double to fraction conversion precision
#define FRACTION_32_BIT_DECIMAL_PLACES 5   // Max fraction value for 32-bit timestamps is 2 ^ 16 - 1 = 65 535 (5 digits)
#define FRACTION_64_BIT_DECIMAL_PLACES 10  // Max fraction value for 64-bit timestamps is 2 ^ 32 - 1 = 4 294 967 295 (10 digits)

#define LI        (0x03 << 6)
#define VN        (0x07 << 3)
#define MODE      (0x07 << 0)

#endif
