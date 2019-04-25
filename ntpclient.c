#include "constants.h"
#include "lib.h"
#include "packets.h"
#include "servers.h"

int main(int argc, char* argv[]) {

	// Check number of arguments
	// No server
	if (argc <= 1) {

		// Describe error
		perror("No server given");

        // Exit
        exit(EXIT_FAILURE);
	}

    // Too many servers
    else if (argc > N_MAX_SERVERS + 1) {

        // Describe error
        perror("Too many servers (> 100)");

        // Exit
        exit(EXIT_FAILURE);
    }

	// Get number of servers
	int n_servers = argc - 1;

	// Initialize general variables
	int y = 1;

	// Initialize socket variables
	int s, s_listen;
	struct addrinfo hints, *servinfo;

	// Initialize buffer
	int n_bytes;
	uint8_t buffer[BUFFER_SIZE] = {0};

	// Initialize server structs
	server * servers[N_MAX_SERVERS];

	// Initialize time measurement variables
	struct timespec start_ts, end_ts;
	double t1, t2, t3, t4;
	double delays[N_REQUESTS], offsets[N_REQUESTS], root_dispersions[N_REQUESTS];

	// Initialize packets
	packet *pkt_send;
	packet *pkt_recv;

	// Fill address hints to generate socket address structure
	memset(&hints, 0, sizeof(hints)); // Make sure the struct is empty
	hints.ai_family = AF_UNSPEC;      // IPv4 or IPv6
	hints.ai_socktype = SOCK_DGRAM;   // Datagram sockets (UDP)



	// Loop on each server
	for (int i = 0; i < n_servers; i++) {

		// Initialize server
		servers[i] = malloc(sizeof(server));

		// Define its host
		servers[i] -> host = argv[i + 1];

		// Info
		printf("Connecting to: %s:%s\n", servers[i] -> host, PORT);

		// Get address structure list based on hints
		if (getaddrinfo(servers[i] -> host, PORT, &hints, &servinfo) != 0)
	    {
			// Describe error
			perror("Address error");

			// Abort
			exit(EXIT_FAILURE);
	    }

	    // Info
	    printf("Defining socket...\n");

		// Define socket
		if ((s = socket(servinfo->ai_family, servinfo->ai_socktype, servinfo->ai_protocol)) == -1) {

			// Describe error
			perror("Socket error");

			// Close socket
			close(s);

			// Abort
			exit(EXIT_FAILURE);
		}

		// Info
		printf("Configuring socket...\n");

		// Configure socket
		// SO_REUSEADDR: avoid "address already in use" error
		if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &y, sizeof(int)) == -1) {

			// Describe error
			perror("Configure error");

			// Close socket
			close(s);

			// Abort
			exit(EXIT_FAILURE);
		}

		// Info
		printf("Connecting to server...\n");

		// Connect socket
		if (connect(s, servinfo->ai_addr, servinfo->ai_addrlen) == -1) {

			// Describe error
			perror("Connect error");

			// Close socket
			close(s);

			// Abort
			exit(EXIT_FAILURE);
		}



		// Send a specific number of requests
		for (int j = 0; j < N_REQUESTS; j++) {

			// Allocate memory for packets
			pkt_send = malloc(sizeof(packet));
			pkt_recv = malloc(sizeof(packet));

            // Get origin timestamp
			clock_gettime(CLOCK_REALTIME, &start_ts);

			// Initialize packet to send with default values
			initPacket(pkt_send, start_ts.tv_sec + TIME_OFFSET + start_ts.tv_nsec / 10.0E9);

			// Fill buffer with packet
			packetToBuffer(pkt_send, buffer);

			// Show packet to send
			//printf("Sending packet:\n\n");
			//showPacket(pkt_send);

			// Send packet to server
			if (send(s, buffer, BUFFER_SIZE, 0) == -1) {

				// Describe error
				perror("Send error");

				// Close socket
				close(s);

				// Abort
				exit(EXIT_FAILURE);
			}

			// Receive data from server
			if ((n_bytes = recv(s, buffer, BUFFER_SIZE, 0)) == -1) {

				// Describe error
				perror("Receive error");

				// Close socket
				close(s);

				// Abort
				exit(EXIT_FAILURE);
			}

			// Get reference timestamp
			clock_gettime(CLOCK_REALTIME, &end_ts);

			// Parse received bytes
			bufferToPacket(pkt_recv, buffer);

			// Show packet received from server
			//printf("Received packet:\n\n");
			//showPacket(pkt_recv);



			// Convert end timestamp to doubleToSeconds
			t1 = timeToDouble(pkt_recv -> origin_ts_seconds, pkt_recv -> origin_ts_fraction, FRACTION_64_BIT_DECIMAL_PLACES);
			t4 = end_ts.tv_sec + TIME_OFFSET + end_ts.tv_nsec / 10.0E9;

			// Convert receive and transmit times to double
			t2 = timeToDouble(pkt_recv -> receive_ts_seconds, pkt_recv -> receive_ts_fraction, FRACTION_64_BIT_DECIMAL_PLACES);
			t3 = timeToDouble(pkt_recv -> transmit_ts_seconds, pkt_recv -> transmit_ts_fraction, FRACTION_64_BIT_DECIMAL_PLACES);

			// Show times in NTP format
			printf("Timestamps:\n");
			printf("t1: %lf\n", t1);
			printf("t2: %lf\n", t2);
			printf("t3: %lf\n", t3);
			printf("t4: %lf\n", t4);
			printf("\n");

			// Get root dispersion
			root_dispersions[j] = timeToDouble(pkt_recv -> root_dispersion_seconds, pkt_recv -> root_dispersion_fraction, FRACTION_32_BIT_DECIMAL_PLACES);

			// Compute delay and offset
			delays[j] = ((t4 - t1) - (t3 - t2)) / 2.0; // RTT/2
			offsets[j] = 0.5 * ((t2 - t1) + (t3 - t4));

			// Free allocated memory for packets
			free(pkt_send);
			free(pkt_recv);
		}

		// Compute averages
		servers[i] -> avg_delay = avg(delays, N_REQUESTS);
		servers[i] -> avg_offset = avg(offsets, N_REQUESTS);
		servers[i] -> avg_root_dispersion = avg(root_dispersions, N_REQUESTS);

		// Compute dispersion
		servers[i] -> dispersion = max(delays, N_REQUESTS) - min(delays, N_REQUESTS);

		// Compute dispersions sum
		servers[i] -> dispersion_sum = servers[i] -> dispersion + servers[i] -> avg_root_dispersion;

		// Show server infos
		printf("---------- SERVER %d ----------\n", i + 1);
		printf("Hostname: %s\n", servers[i] -> host);
		printf("Average root dispersion: %lf\n", servers[i] -> avg_root_dispersion);
		printf("Dispersion: %lf\n", servers[i] -> dispersion);
		printf("Average delay: %lf\n", servers[i] -> avg_delay);
		printf("Average offset: %lf\n\n", servers[i] -> avg_offset);

		// Close socket
		close(s);
	}



	// Find best server
	server * serv = getBestServer(servers, n_servers);

	// Show its values
	printf("---------- BEST SERVER ----------\n");
	printf("Hostname: %s\n", serv -> host);
	printf("Local time: %lf\n", t4);
	printf("Average offset: %lf\n", serv -> avg_offset);
	printf("Updated time: %lf\n\n", t4 + serv -> avg_offset);



	// Loop on servers
	for (int i = 0; i < n_servers; i++) {

		// Free their allocated memory
		free(servers[i]);
	}



	// Terminate
	return 0;
}
