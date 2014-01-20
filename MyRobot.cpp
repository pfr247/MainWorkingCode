#include "WPILib.h"

/**
 * This is a demo program showing the use of the RobotBase class.
 * The SimpleRobot class is the base of a robot application that will automatically call your
 * Autonomous and OperatorControl methods at the right time as controlled by the switches on
 * the driver station or the field controls.
 */ 
class RobotDemo : public SimpleRobot
{
	RobotDrive myRobot; // robot drive system
	Joystick stick; // only joystick
	Encoder digEncoder;
	AnalogChannel ultra;
	Servo servo;

public:
	RobotDemo(void):
		myRobot(8, 1, 9, 2),
		stick(1),
		digEncoder(13, 14),
		ultra(1, 1),
		servo(5)
	{											
		myRobot.SetExpiration(0.1);
		myRobot.SetInvertedMotor(myRobot.kFrontRightMotor, true);
		//myRobot.SetInvertedMotor(myRobot.kFrontLeftMotor, true);
		myRobot.SetInvertedMotor(myRobot.kRearRightMotor, true);
		//myRobot.SetInvertedMotor(myRobot.kRearLeftMotor, true);
		
	}

	/**
	 * Drive forward for 10 feet and briefly stop if the ultrasonic sees something
	 */
	void Autonomous(void)
	{
		myRobot.SetSafetyEnabled(false);
		bool go = true;
		digEncoder.Reset();
		digEncoder.Start();
		while {digEncoder.GetDistance()*-12*3.14159/500.0 < 10*12.0 && go == true)
		{
			if (ultra.GetVoltage()/40.0 < 24)
			{
				for (int i = 0; i < 2; i++)
				{
					wait(1.0);
					if (ultra.GetVoltage()/40.0 < 24)
					{
						go = true;
						break;
					}
					else
					{
						go = false;
					}
				}
			}
			myRobot.MecanumDrive_Cartesian(0, -.2, 0);
			SmartDashboard::PutNumber("Encoder Post Conversion", digEncoder.GetDistance()*-12*3.14159/250.0);
		}
		/*while (digEncoder.GetDistance()*-12*3.14159/500.0 < 10*12.0) {
			myRobot.MecanumDrive_Cartesian(0, -.2, 0);
			SmartDashboard::PutNumber("Encoder Post Conversion", digEncoder.GetDistance()*-12*3.14159/250.0);
		}*/
		digEncoder.Stop();
		
		myRobot.MecanumDrive_Cartesian(0, 0, 0);
	}

	/*
	  Runs the motors with arcade steering. 
	 */
	
	void OperatorControl(void)
	{
		myRobot.SetSafetyEnabled(true);
		digEncoder.Start();
		//ultrasonic1.SetAutomaticMode(true);
		//ultra.SetAverageBits(12);
		 const double ppsTOrpm = 60.0/250.0;        //Convert from Pos per Second to Rotations per Minute by multiplication
                                                        // (See the second number on the back of the encoder to replace 250 for different encoders)
                const float VoltsToIn = 41.0;         // Convert from volts to cm by multiplication (volts from ultrasonic).
                                                // This value worked for distances between 1' and 10'.
		
		while (IsOperatorControl())
		{
			if (stick.GetRawButton(4)) {
				myRobot.MecanumDrive_Cartesian(stick.GetX(), stick.GetY(), -1);
			} else if (stick.GetRawButton(5)) {
				myRobot.MecanumDrive_Cartesian(stick.GetX(), stick.GetY(), 1);
			} else {
				myRobot.MecanumDrive_Cartesian(stick.GetX(), stick.GetY(), 0);
			}
			
			myRobot.MecanumDrive_Cartesian(stick.GetX(), stick.GetY(), 0);
			
			SmartDashboard::PutNumber("Digital Encoder RPM", digEncoder.GetRate()*ppsTOrpm);
			//SmartDashboard::PutNumber("Digital Encoder Raw", digEncoder.GetRaw());
			//SmartDashboard::PutNumber("Ultrasonic Distance", ultrasonic1.GetRangeInches());
			SmartDashboard::PutNumber("Ultrasonic Distance inch", (double) ultra.GetAverageVoltage()*VoltsPerInch);
			/*if (stick.GetRawButton(1)) {
				servo.SetAngle(0.0);
				SmartDashboard::PutNumber("Servo", (double) servo.GetAngle());
			} else {
				servo.SetAngle(90.0);
				SmartDashboard::PutNumber("Servo", (double) servo.GetAngle());
			}*/
			servo.SetAngle(servo.GetMaxAngle()*stick.GetThrottle());
			Wait(0.1);
		}
		digEncoder.Stop();
		//ultrasonic1.SetAutomaticMode(false);
	}
	
	
	/**
	 * Runs during test mode
	 */
	void Test() {
			
	}
};

START_ROBOT_CLASS(RobotDemo);

