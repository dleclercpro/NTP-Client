#include "servers.h"

server * getBestServer(server ** servers, int n_servers) {

	// Initialize best server with first one
	server *serv = servers[0];

	// Loop on the rest and try to find a better one
	for (int i = 1; i < n_servers; i++) {

		// Use sum of average root dispersion and dispersion as comparison criteria
		if (servers[i] -> dispersion_sum < serv -> dispersion_sum) {

			// Update best server
			serv = servers[i];
		}
	}

	// Return best server
	return serv;
}