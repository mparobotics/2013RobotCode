#include "WPILib.h"
#include <ADXL345_SPI.h>
#include <Solenoid.h>

class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	CANJaguar jag1; //Declartion for Jaguar 1
	Solenoid sol1;
	Solenoid sol2;
	Solenoid sol3;
	Solenoid sol4;
	Solenoid sol5;
	Solenoid sol6;
	Solenoid sol7;
	Solenoid sol8;
	Relay compressor;
	Gyro gyro;

public:
	RobotDemo(void):
		myRobot(1, 2),	// these must be initialized in the same order
		stick(1),		// as they are declared above.
		jag1(5,CANJaguar::kVoltage),
		sol1(1),
		sol2(2),
		sol3(3),
		sol4(4),
		sol5(5),
		sol6(6),
		sol7(7),
		sol8(8),
		compressor(1,Relay::kForwardOnly),
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
			jag1.Set(stick.GetY()*-11.0*1.09); //Set Jag
			//printf("%f", jag1.GetOutputVoltage());
			//printf("\n");
			Wait(.05);				// wait for a motor update time
			sol1.Set(stick.GetRawButton(1));
			sol2.Set(stick.GetRawButton(2));
			sol3.Set(stick.GetRawButton(3));
			sol4.Set(stick.GetRawButton(4));
			sol5.Set(stick.GetRawButton(5));
			sol6.Set(stick.GetRawButton(6));
			sol7.Set(stick.GetRawButton(7));
			//sol8.Set(stick.GetRawButton(8));
			if (stick.GetRawButton(8)) {
				compressor.Set(Relay::kOn);
			} else {
				compressor.Set(Relay::kOff);
			}
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

