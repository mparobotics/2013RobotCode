#include "WPILib.h"
#include <ADXL345_SPI.h>
#include <Solenoid.h>
#include "omnitrain.h"

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

class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick1; // joystick1
	Joystick stick2; //joystick2
	CANJaguar* drivejags[4]; //Declartion for Jaguar 1
	CANJaguar* innerlifts[2];
	CANJaguar* outerlifts[4];
	//Gyro gyro;

public:
	RobotDemo(void):
		myRobot(1, 2),	// these must be initialized in the same order
		stick1(1),		// as they are declared above.
		stick2(2)
	{
		myRobot.SetExpiration(0.1);
		drivejags[0] = new CANJaguar(3, CANJaguar::kVoltage);
		drivejags[1] = new CANJaguar(1, CANJaguar::kVoltage);
		drivejags[2] = new CANJaguar(4, CANJaguar::kVoltage);
		drivejags[3] = new CANJaguar(2, CANJaguar::kVoltage);
		innerlifts[0] = new CANJaguar(9, CANJaguar::kVoltage);
		innerlifts[1] = new CANJaguar(10, CANJaguar::kVoltage);
		outerlifts[0] = new CANJaguar(5, CANJaguar::kVoltage);
		outerlifts[1] = new CANJaguar(6, CANJaguar::kVoltage);
		outerlifts[2] = new CANJaguar(7, CANJaguar::kVoltage);
		outerlifts[3] = new CANJaguar(8, CANJaguar::kVoltage);
	}

	void Autonomous(void)
	{
		
	}

	
	void OperatorControl(void)
	{
		myRobot.SetSafetyEnabled(true);
		for (int i = 0; i < 4; i++) {
			drivejags[i]->SetSpeedReference(CANJaguar::kSpeedRef_None);
			drivejags[i]->EnableControl();
		}
		setupOmniTrain(2.0, 2.0);
		double drive[4];
		
		for (int i = 0; i < 4; i++) {
			outerlifts[i]->SetSpeedReference(CANJaguar::kSpeedRef_None);
			outerlifts[i]->EnableControl();
		}
		
		for (int i = 0; i < 2; i++) {
			innerlifts[i]->SetSpeedReference(CANJaguar::kSpeedRef_None);
			innerlifts[i]->EnableControl();
		}
		
		
		double outerliftvoltage;
		double innerliftvoltage;
		
		double outerliftavg = 0;
		int framecount = 0;
		
		while (IsOperatorControl())
		{        
			/* boundedDriveOmniTrain(stick1.GetX(), stick1.GetY(), stick2.GetX(), drive);
			for (int i = 0; i < 4; i++) {
				drivejags[i]->Set(drive[i] * 12.0);
			} */
			/*
			if (stick1.GetRawButton(1)) {
				innerlift->Set(12.0);
			}
			else if (stick1.GetRawButton(2)) {
				innerlift->Set(-12.0);
			}
			else {
				innerlift->Set(0.0);
			}
			
			if (stick1.GetRawButton(3)) {
				outerliftvoltage = 12.0;
			}
			else if (stick1.GetRawButton(4)) {
				outerliftvoltage = -12.0;
			}
			else {
				outerliftvoltage = 0.0;
			}
			*/
			
			outerliftvoltage = 12.0 * joytrim(stick1.GetY());
			for (int i = 0; i < 4; i++) {
				outerlifts[i]->Set(outerliftvoltage * (i > 1 ? -1 : 1));
			}
			innerliftvoltage = 12.0 * joytrim(stick2.GetY());
			innerlifts[0]->Set(outerliftvoltage);
			innerlifts[1]->Set(innerliftvoltage);
			
			outerliftavg = ((outerliftavg * 29.0) + outerliftvoltage) / 30.0;
			
			framecount++;
			
			if (framecount > 60) {
				printf("%f", outerliftavg);
			}
			
			
			//printf("%f", jag1.GetOutputVoltage());
			//printf("\n");
			Wait((1.0 / 26.25));				// wait for a motor update time
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

