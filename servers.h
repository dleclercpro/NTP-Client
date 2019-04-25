#ifndef _SERVERS_H_
#define _SERVERS_H_

#include "constants.h"

typedef struct {
    char *host;
    double avg_root_dispersion;
    double dispersion;
    double dispersion_sum;
    double avg_delay;
    double avg_offset;
} server;

server * getBestServer(server ** servers, int n_servers);

#endif
