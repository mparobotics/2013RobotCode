#include "WPILib.h"
#include <ADXL345_SPI.h>
#include <Solenoid.h>
#include "omnitrain.h"

class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick1; // joystick1
	Joystick stick2; //joystick2
	CANJaguar* drivejags[4]; //Declartion for Jaguar 1
	CANJaguar* innerlift;
	CANJaguar* outerlifts[2];
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
		innerlift = new CANJaguar(7, CANJaguar::kVoltage);
		outerlifts[0] = new CANJaguar(5, CANJaguar::kVoltage);
		outerlifts[1] = new CANJaguar(6, CANJaguar::kVoltage);
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
		double outerliftvoltage;
		while (IsOperatorControl())
		{
			driveOmniTrain(stick1.GetX(), stick1.GetY(), stick2.GetX(), drive);
			for (int i = 0; i < 4; i++) {
				drivejags[i]->Set(drive[i] * 12.0);
			}
			if (stick1.GetRawButton(1)) {
				innerlift->Set(11.0);
			}
			else if (stick1.GetRawButton(2)) {
				innerlift->Set(-11.0);
			}
			else {
				innerlift->Set(0.0);
			}
			
			if (stick1.GetRawButton(3)) {
				outerliftvoltage = 11.0;
			}
			else if (stick1.GetRawButton(4)) {
				outerliftvoltage = -11.0;
			}
			else {
				outerliftvoltage = 0.0;
			}
			outerlifts[0]->Set(outerliftvoltage);
			outerlifts[1]->Set(outerliftvoltage);
			
			
			//printf("%f", jag1.GetOutputVoltage());
			//printf("\n");
			Wait(1.0 / 30.0);				// wait for a motor update time
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

