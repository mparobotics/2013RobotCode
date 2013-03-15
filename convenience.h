#include <math.h>

#ifndef CONVENIENCE
#define CONVENIENCE

//This function takes joystick axis input, and makes a dead-zone and a limit
double joytrim(double x) {
	if (x > 1.0) {
		x = 1.0;
	}
	if (x < -1.0) {
		x = -1.0;
	}
	if (fabs(x) < 0.05) {
		x = 0.0;
	}
	return x;
}

double distanceFormula(double a, double b) {
	return sqrt((a * a) + (b * b));
}

double sq(double x) {
    return x * x;
}

#endif
