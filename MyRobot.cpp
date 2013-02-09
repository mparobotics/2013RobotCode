#include "WPILib.h"
#include <ADXL345_SPI.h>
#include <Solenoid.h>
#include "matrix.h"

class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	CANJaguar jag1; //Declartion for Jaguar 1
	Gyro gyro;

public:
	RobotDemo(void):
		myRobot(1, 2),	// these must be initialized in the same order
		stick(1),		// as they are declared above.
		jag1(5,CANJaguar::kVoltage),
		gyro(1)
	{
		myRobot.SetExpiration(0.1);
	}

	void Autonomous(void)
	{
		
	}

	
	void OperatorControl(void)
	{
		myRobot.SetSafetyEnabled(true);
		jag1.SetSpeedReference(CANJaguar::kSpeedRef_None);
		jag1.EnableControl();
		gyro.Reset(); 
		while (IsOperatorControl())
		{
			jag1.Set(stick.GetY()*-12.0); //Set Jag
			//printf("%f", jag1.GetOutputVoltage());
			//printf("\n");
			Wait(.05);				// wait for a motor update time
			printf("%f", gyro.GetAngle());
		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

