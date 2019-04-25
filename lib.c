#include "lib.h"

unsigned long unpackBytes(uint8_t *bytes, int n_bytes) {

	// Initialize result
	unsigned long result = 0;

	// Loop on bytes
	for (int i = 0; i < n_bytes; i++) {

		// Add them to result
		result |= bytes[i] << (8 * (n_bytes - 1 - i));
	}

	// Return unpacked bytes
	return result;
}

void packBytes(uint8_t *bytes, int n_bytes, unsigned long x) {

	// Loop on bytes
	for (int i = 0; i < n_bytes; i++) {

		// Add them to result
		bytes[n_bytes - 1 - i] = (x & (0xFF << 8 * i)) >> 8 * i;
	}
}

double avg(double *values, int n_values) {

	//Initialize average
	double avg = 0;

	// Compute sum of values
	for (int i = 0; i < n_values; i++) {
		avg += values[i];
	}

	// Divide by number of values
	avg /= n_values;

	// Return average
	return avg;
}

double min(double *values, int n_values) {

	// Initialize min value
	double min = DBL_MAX;

	// Find min
	for (int i = 0; i < n_values; i++) {

		// Smaller than current min
		if (min > values[i]) {

			// Update it
			min = values[i];
		}
	}

	// Return min value
	return min;
}

double max(double *values, int n_values) {

	// Initialize max value
	double max = -DBL_MAX;

	// Find max
	for (int i = 0; i < n_values; i++) {

		// Greater than current max
		if (max < values[i]) {

			// Update it
			max = values[i];
		}
	}

	// Return max value
	return max;
}

double timeToDouble(double seconds, double fraction, int n_decimals) {

	// Normalize fraction to unit
	for (int i = 0; i < n_decimals; i++) {

		// Move comma
		fraction /= 10.0;
	}

	// Return seconds and fraction as double
	return seconds + fraction;
}

long doubleToSeconds(double t) {

    // Cut decimals
    return (long) t;
}

/*
    WARNING: will not convert fractions with 0s upfront. For instance: 4.05 will
    have its fraction converted to 5. In our case, it doesn't matter, since fractions
    are padded with zeros with the function timeToDouble when received from servers
*/
long doubleToFraction(double t) {

	// Get fraction part
	double fraction = t - doubleToSeconds(t);

    // Initialize precision
	int p = 0;

    // Compare each decimal
	while ((double) (long) fraction != fraction && p++ < FRACTION_PRECISION) {

		// Move comma
		fraction *= 10.0;
	}

	// Return fraction as integer
	return (long) fraction;
}
