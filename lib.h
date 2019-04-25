#ifndef _LIB_H_
#define _LIB_H_

#include "constants.h"

unsigned long unpackBytes(uint8_t *bytes, int n_bytes);
void packBytes(uint8_t *bytes, int n_bytes, unsigned long x);
double avg(double *values, int n_values);
double min(double *values, int n_values);
double max(double *values, int n_values);
double timeToDouble(double seconds, double fraction, int n_decimals);
long doubleToSeconds(double t);
long doubleToFraction(double t);

#endif
