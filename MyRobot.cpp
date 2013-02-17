#include "WPILib.h"
#include <ADXL345_SPI.h>
#include <Solenoid.h>
#include "omnitrain.h"


class RobotDemo : public IterativeRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick1; // joystick1
	Joystick stick2; //joystick2
	Jaguar* drivejags[4]; //Declartion for Jaguar 1
	Jaguar* innerlifts[2];
	Jaguar* outerlifts[4];
	int states[2];
	double drive[4]; //Drivetrain jag voltages
	//Voltage variables
	double outerliftvoltage;
	double innerliftvoltage;
	//For finding the "hang" voltage and debugging
	double outerliftavg;
	int framecount;
	//False if Xbox controllers, true if joysticks
	//bool controlmethod = *((bool *) controlChooser->GetSelected());
	bool controlmethod;
	SendableChooser *controlChooser;
	//Gyro gyro;

public:
	RobotDemo(void):
		myRobot(1, 2),	// these must be initialized in the same order
		stick1(1, 6, 12),		// as they are declared above.
		stick2(2)
	{
		myRobot.SetExpiration(0.1); /*
		drivejags[0] = new Jaguar(3);
		drivejags[1] = new Jaguar(1);
		drivejags[2] = new Jaguar(4);
		drivejags[3] = new Jaguar(2); */
		innerlifts[0] = new Jaguar(7);
		innerlifts[1] = new Jaguar(8);
		outerlifts[0] = new Jaguar(3);
		outerlifts[1] = new Jaguar(4);
		outerlifts[2] = new Jaguar(5);
		outerlifts[3] = new Jaguar(6);
		
		//doing this only because "sendableChooser" needs a pointer.
		states[0] = 0;
		states[1] = 1;
		 
	
	}
	virtual void RobotInit() {
		//Initialize the chooser in the dashboard between control modes
		controlChooser = new SendableChooser();
		controlChooser->AddDefault("Xbox Controllers", &states[0]);
		controlChooser->AddObject("ATK3 Joysticks", &states[1]);
		SmartDashboard::PutData("Control mode chooser", controlChooser);
	}

	virtual void TeleopInit() {
		outerliftavg = 0;
		framecount = 0;
		//False if Xbox controllers, true if joysticks
		controlmethod = (bool) *((int *)controlChooser->GetSelected());
		
		
		myRobot.SetSafetyEnabled(false);
		
		setupOmniTrain(2.0, 2.0);
		/*
		//Initialize the lifts
		for (int i = 0; i < 4; i++) {
			outerlifts[i]->SetSpeedReference(CANJaguar::kSpeedRef_None);
			outerlifts[i]->EnableControl();
		}
		
		for (int i = 0; i < 2; i++) {
			innerlifts[i]->SetSpeedReference(CANJaguar::kSpeedRef_None);
			innerlifts[i]->EnableControl();
		} */
	}
	
	virtual void TeleopPeriodic()
	{	
		/* boundedDriveOmniTrain(stick1.GetX(), stick1.GetY(), stick2.GetX(), drive);
		for (int i = 0; i < 4; i++) {
			drivejags[i]->Set(drive[i]);
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
		
		outerliftvoltage = -1.0 * joytrim(controlmethod ? stick1.GetY() : stick1.GetRawAxis(2));
		if (controlmethod ? stick1.GetTrigger() : stick1.GetRawButton(5)) {
			outerliftvoltage = outerliftvoltage / 2.0;
		}
		for (int i = 0; i < 4; i++) {
			outerlifts[i]->Set(outerliftvoltage * (i > 1 ? -1 : 1));
		}
		
		innerliftvoltage = 1.0 * joytrim(controlmethod ? stick2.GetY() : stick1.GetRawAxis(5));
		if (controlmethod ? stick2.GetTrigger() : stick1.GetRawButton(6)) {
			innerliftvoltage = innerliftvoltage / 2.0;
		}
		innerlifts[0]->Set(innerliftvoltage);
		innerlifts[1]->Set(innerliftvoltage);
		
		outerliftavg = ((outerliftavg * 29.0) + outerliftvoltage) / 30.0;
		  
		framecount++;
		/*
		if (framecount > 60) {
			printf("%f", outerliftavg);
		}
		*/
		//printf("%f", jag1.GetOutputVoltage());
		//printf("\n");
		Wait((1.0 / 15.0 /* / 26.25 */));				// wait for a motor update time
	}
};

START_ROBOT_CLASS(RobotDemo);

