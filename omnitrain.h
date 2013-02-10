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
	double J = (1.0 - p) / distanceFormula(spacing_length * .5, p * spacing_width);
	double T = p / distanceFormula(spacing_length * .5, (1.0 - p) * spacing_width);
	
	double a = k1 * (1.0 - p);
	double b = k1 * p;
	double c = k2 * J;
	double d = k2 * T;
	
	double drivetrain_input_mat[16] = {a,-a,b,-b,a,a,b,b,-c,c,d,-d,1.0,1.0,-1.0,1.0};
	
	bool errorcode = !invertMatrix(drivetrain_input_mat, drivetrain_inverse_mat);
	return errorcode;
}

/*this function should have low values [close to 0.0, relative to 1.0]
in the domain of [-1.0, 1.0], and prohibitively high values outside that range.
In addition, the prohibitively high values should increase as x gets further from
zero.*/ 
double heuristicFunc(double x) {
	if (x > 1.0) {
		return 50.0 * x - 50.0;
	}
	if (x < -1.0) {
		return -50.0 * x - 50.0;
	}
	return 0.0;
}

double sq(double x) {
    return x * x;
}

/* input: array of drivetrain voltage scalars. Output: rough power consumption */
double powerConsumption(const double drive[4]) {
	return sq(drive[0]) + sq(drive[1]) + sq(drive[2]) + sq(drive[3]);
}

//resulting heuristic function
double heuristic(const double drive[4]) {
	return powerConsumption(drive) + heuristicFunc(drive[0]) +
			heuristicFunc(drive[1]) + heuristicFunc(drive[1]) + 
			heuristicFunc(drive[3]);
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
	double highval = heuristic(drivetrain);
	
	//set low bound
	double lowbound = -4.0;
	columnvec[3] = lowbound;
	multiplyMatrix(drivetrain_inverse_mat, columnvec, drivetrain);
	double lowval = heuristic(drivetrain);
	
	double midval;
        int i;	
	for (i = 0; i < 9; i++) {
            if (highval < lowval) {
                lowbound = (lowbound + highbound) * 0.5;
            }
            else if (highval > lowval) {
                highbound = (lowbound + highbound) * 0.5;
            }
            columnvec[3] = highbound;
	    multiplyMatrix(drivetrain_inverse_mat, columnvec, drivetrain);
            highval = heuristic(drivetrain);

            columnvec[3] = lowbound;
            multiplyMatrix(drivetrain_inverse_mat, columnvec, drivetrain);
            lowval = heuristic(drivetrain); 
	}
        columnvec[3] = (lowbound + highbound) * 0.5;
        multiplyMatrix(drivetrain_inverse_mat, columnvec, drivetrain);
        return;		
}

void boundedDriveOmniTrain(double xvel, double yvel, double t, double drivetrain[4]) {
    driveOmniTrain(xvel, yvel, t, drivetrain);
    int i;
    double high = 0.0;
    for (i = 0; i < 4; i++) {
        if (fabs(drivetrain[i]) > high) {
            high = fabs(drivetrain[i]);
        }
    }
    if (fabs(high) > 1.00000000) {
        #ifdef DEBUG
        printf("%s", " \n The rent is too damn high! \n abs value of highest voltage scalar: ");
        printf("%f", high);
        #endif
        double scalefactor = 1.000000 / fabs(high);
        drivetrain[0] *= scalefactor;
        drivetrain[1] *= scalefactor;
        drivetrain[2] *= scalefactor;
        drivetrain[3] *= scalefactor;

    }
}

