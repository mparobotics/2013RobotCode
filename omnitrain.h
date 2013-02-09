#include "matrix.h"
const double spacing_length = 7.875; //length of wheel contact spacing [in]
const double spacing_width = 5.5; // width of wheel contact spacing [in]
double p = .5; //proportion of weight distribution [see drawings]

double DistanceFormula(double a, double b) {
	return sqrt((a * a) + (b * b));
}

double drivetrain_inverse_mat[16];


/*input: arbitrary constant affecting non-rotational movement, arbitrary
 * constant affecting rotational movement. Output: correctly filled global
 * variable [drivetrain_inverse_mat] [MOTHER OF GOD, HELP ME -- g-ban], 
 * and an error code that is true if there is an error, false if not.
 */
bool SetupOmniTrain(double k1, double k2) {
	//[see drawings, otherwise, voodoo]
	double J = (1 - p) / DistanceFormula(spacing_length * .5, p * spacing_width);
	double T = p / DistanceFormula(spacing_length * .5, (1 - p) * spacing_width);
	
	double a = k1 * (1 - p);
	double b = k1 * p;
	double c = k2 * J;
	double d = k2 * T;
	
	double drivetrain_input_mat[16] = {a,-a,b,-b,a,a,b,b,-c,c,d,-d,1,1,1,1};
	
	bool errorcode = !InvertMatrix(drivetrain_input_mat, drivetrain_inverse_mat);
	return error;
}
/*input: relative sideways movement, relative back/forth movement, 
relative turning. All are on arbitrary scaling according to k1 and k2 [see above]
Also input an array of four doubles [front-left wheel, front-right, back-right,
back-left] that is empty. Output: Has the side-effect of filling the array. 
NOTE: Must first call SetupOmniTrain!!!*/
void DriveOmniTrain(double xvel, double yvel, double t, double drivetrain[4]) {
	//note: Voodoo unless you read math documentation!
	//Although... Note: u is a parameter that we have to optimize
	double u = 0.0;
	double columnvec[4] = {xvel, yvel, t, u};
	//CODE GOES HERE LOL
	
}

