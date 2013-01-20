#include "WPILib.h"
#include <Solenoid.h>

class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	CANJaguar jag1; //Declartion for Jaguar 1
        Solenoid solenoids[9];

public:
	RobotDemo(void):
		myRobot(1, 2),	// these must be initialized in the same order
		stick(1),		// as they are declared above.
		jag1(5,CANJaguar::kVoltage)
	{
                int i = 1;
                while (i < 9) {
                    solenoids[i] = new Solenoid(i);
                    i++;
                }
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
		while (IsOperatorControl())
		{
			jag1.Set(stick.GetY()*-11.0*1.09); //Set Jag
			printf("%f", jag1.GetOutputVoltage());
			printf("\n");
			Wait(.05);				// wait for a motor update time
                        int i = 1;
                        while (i < 9) {
                            solenoids[i].Set(stick.GetRawButton(i));
                            i++;
                        }
    		}
	}
	
	/**
	 * Runs during test mode
	 */
	void Test() {

	}
};

START_ROBOT_CLASS(RobotDemo);

