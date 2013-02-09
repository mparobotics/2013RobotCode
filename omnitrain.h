#include "matrix.h"
const double spacing_length = 7.875; //length of wheel contact spacing [in]
const double spacing_width = 5.5; // width of wheel contact spacing [in]
double p = .5; //proportion of weight distribution [see drawings]

double distanceFormula(double a, double b) {
	return sqrt((a * a) + (b * b));
}

double drivetrain_inverse_mat[16];


/*input: arbitrary constant affecting non-rotational movement, arbitrary
 * constant affecting rotational movement. Output: correctly filled global
 * variable [drivetrain_inverse_mat] [MOTHER OF GOD, HELP ME -- g-ban], 
 * and an error code that is true if there is an error, false if not.
 */
bool setupOmniTrain(double k1, double k2) {
	//[see drawings, otherwise, voodoo]
	double J = (1 - p) / distanceFormula(spacing_length * .5, p * spacing_width);
	double T = p / distanceFormula(spacing_length * .5, (1 - p) * spacing_width);
	
	double a = k1 * (1 - p);
	double b = k1 * p;
	double c = k2 * J;
	double d = k2 * T;
	
	double drivetrain_input_mat[16] = {a,-a,b,-b,a,a,b,b,-c,c,d,-d,1,1,1,1};
	
	bool errorcode = !InvertMatrix(drivetrain_input_mat, drivetrain_inverse_mat);
	return error;
}

/*this function should have low values [close to 0.0, relative to 1.0]
in the domain of [-1.0, 1.0], and prohibitively high values outside that range.
In addition, the prohibitively high values should increase as x gets further from
zero.*/ 
double herusuticFunc(double x) {
	if (x > 1.0) {
		return 250.0 * x - 250.0;
	}
	if (x < -1.0) {
		return -250.0 * x - 250.0;
	}
	return 0.0;
}

/* input: array of drivetrain voltage scalars. Output: rough power consumption */
double powerConsumption(const double drive[4]) {
	return fabs(drive[0]) + fabs(drive[1]) + fabs(drive[2]) + fabs(drive[3]);
}

//resulting herusutic function
double herusutic(const double drive[4]) {
	return powerConsumption(drive) + herusuticFunc(drive[0]) +
			herusuticFunc(drive[1]) + herusuticFunc(drive[1]) + 
			herusuticFunc(drive[3]);
}

/*input: relative sideways movement, relative back/forth movement, 
relative turning. All are on arbitrary scaling according to k1 and k2 [see above]
Also input an array of four doubles [front-left wheel, front-right, back-right,
back-left] that is empty. Output: Has the side-effect of filling the array. 
NOTE: Must first call SetupOmniTrain!!!*/
void driveOmniTrain(double xvel, double yvel, double t, double drivetrain[4]) {
	//note: Voodoo unless you read math documentation!
	//Although... Note: u is a parameter that we have to optimize
	double u = 0.0;
	//since u is technically equivalent to all of the voltage scalars [-1.0 -> 1.0]
	//added together, it has a minimum value of -4.0 and a maximum of 4.0
	double columnvec[4] = {xvel, yvel, t, u};
	
	//BEGIN BINARY SEARCH ROUTINE
	//binary search for the lowest power consumption
	
	//set high bound
	double highbound = 4.0;
	columnvec[3] = highbound;
	multiplyMatrix(drivetrain_inverse_mat, columnvec, drivetrain);
	double highval = herusutic(drivetrain);
	
	//set low bound
	double lowbound = -4.0;
	columnvec[3] = lowbound;
	multiplyMatrix(drivetrain_inverse_mat, columnvec, drivetrain);
	double lowval = herusutic(drivetrain);
	
	double midval;
	
	for (int i = 0; i < 11; i++) {
		if (highval < lowval) {
			
		}
	}
	
	
}

