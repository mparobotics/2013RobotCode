#include "WPILib.h"
#include <Solenoid.h>
#include "convenience.h"
#define DEBUGTIME
//#define MOTORS_DISABLED
 
class RobotDemo : public IterativeRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick1; // joystick1
	Joystick stick2; //joystick2
	Jaguar* innerlifts[2];
	Jaguar* outerlifts[2];
	DriverStation* ds;
	
	DigitalInput* innerswitchb; //"b" for bottom
	DigitalInput* innerswitcht;
	DigitalInput* outerswitchb;
	DigitalInput* outerswitcht;
	
	int states[2];
	
	//Voltage variables
	double outerliftvoltage;
	double innerliftvoltage;
	
	//False if Xbox controllers, true if joysticks
	bool controlmethod;
	SendableChooser *controlChooser;

public:
	RobotDemo(void):
		myRobot(8, 9),	// these must be initialized in the same order
		stick1(1, 6, 12),		// as they are declared above.
		stick2(2, 6, 12)
	{
		myRobot.SetExpiration(0.1);
		
		//doing this only because "sendableChooser" needs a pointer.
		states[0] = 0;
		states[1] = 1;
		 
	
	}
	virtual void RobotInit() {
		//Initialize the chooser in the dashboard between control modes
		controlChooser = new SendableChooser();
		controlmethod = false;
		controlChooser->AddDefault("Xbox Controllers", &states[0]);
		controlChooser->AddObject("ATK3 Joysticks", &states[1]);
		SmartDashboard::PutData("Control mode chooser", controlChooser);
		
		innerlifts[0] = new Jaguar(3);
		innerlifts[1] = new Jaguar(4);
		outerlifts[0] = new Jaguar(1);
		outerlifts[1] = new Jaguar(2);
		ds = DriverStation::GetInstance();
		
		outerswitchb = new DigitalInput(3); //front::bottom
		outerswitcht = new DigitalInput(1); //front::top
		innerswitchb = new DigitalInput(2); //inside::bottom
		innerswitcht = new DigitalInput(4); //inside::top
		//ALL WILL BE TRUE IF DEPRESSED
		//IF DEPRESSED, STOP! ONLY ALLOW DRIVER TO MOVE IN THE RIGHT
		//DIRECTION

		/*
		//Initialize the lifts
		for (int i = 0; i < 2; i++) {
			outerlifts[i]->SetSpeedReference(CANJaguar::kSpeedRef_None);
			outerlifts[i]->EnableControl();
		}
		
		for (int i = 0; i < 2; i++) {
			innerlifts[i]->SetSpeedReference(CANJaguar::kSpeedRef_None);
			innerlifts[i]->EnableControl();
		} */
	}

	virtual void TeleopInit() {
		
		//False if Xbox controllers, true if joysticks
		controlmethod = (bool) *((int *)controlChooser->GetSelected());
		controlmethod = false;
		
		
		myRobot.SetSafetyEnabled(false);

	}
	
	virtual void TeleopPeriodic()
	{	
		outerliftvoltage = 0.9 * joytrim(controlmethod ? stick1.GetRawAxis(2) : stick1.GetRawAxis(2));
		if (controlmethod ? stick1.GetRawButton(2) : stick1.GetRawButton(9)) {
			outerliftvoltage = outerliftvoltage * 0.6;
		}
		if (stick1.GetRawButton(6) or stick1.GetRawButton(5)) {
			outerliftvoltage += 0.3;
		}
		//HOOK DOWN IS POSITIVE (robot up)\
		//If bottom is triggered, just make sure it doesn't move up
		/*
		if (outerswitchb->Get()) {
			outerliftvoltage = outerliftvoltage < 0.0 ? outerliftvoltage : 0.0;
		} */
		//If top is triggered, we need to hang
		
		if (!outerswitcht->Get()) {
			outerliftvoltage = outerliftvoltage > 0.0 ? outerliftvoltage : 0.0;
		} 
		
#ifndef MOTORS_DISABLED
		outerlifts[0]->Set(outerliftvoltage);
		outerlifts[1]->Set(-outerliftvoltage);
#endif		

		//UP is negative, down is positive [joystick]
		innerliftvoltage = 1.0 * joytrim(controlmethod ? stick2.GetRawAxis(2) : stick1.GetRawAxis(5));
		if (controlmethod ? stick2.GetRawButton(2) : stick1.GetRawButton(10)) {
			innerliftvoltage = innerliftvoltage / 3.0;
		}
		
		if (!innerswitchb->Get()) {
			innerliftvoltage = innerliftvoltage < 0 ? innerliftvoltage : 0.0;
		} /*
		if (innerswitcht->Get()) {
			innerliftvoltage = innerliftvoltage > 0 ? innerliftvoltage : 0.0;
		} */
	
#ifndef MOTORS_DISABLED
		innerlifts[0]->Set(innerliftvoltage);
		innerlifts[1]->Set(innerliftvoltage);
#endif
		
#ifdef DEBUGTIME
		float batvoltage = ds->GetBatteryVoltage();
		printf("%s", "\n \n outer voltage: ");
		printf("%f", outerliftvoltage);
		printf("%s", "    ");
		printf("%f", outerliftvoltage * batvoltage);
		printf("%s", " \n inner voltage: ");
		printf("%f", innerliftvoltage);
		printf("%s", "    ");
		printf("%f", innerliftvoltage * batvoltage);
		
		/*
		printf("%f", outerlifts[0]->Get());
		printf("%s", "\n");
		printf("%f", outerlifts[1]->Get());
		printf("%s", "\n");
		printf("%f", innerlifts[0]->Get());
		printf("%f", innerlifts[1]->Get()); */
		printf("%s", "\n \n");
		Wait((1.0 / 12.0));
#endif
		  
		Wait((1.0 / 30.0));				// wait for a motor update time
	}
};

START_ROBOT_CLASS(RobotDemo);

