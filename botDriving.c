/*
FUNCTIONS STILL NEEDED:
Dispenser stuff
Test mode stuff

ALSO MAKE SURE TO SWITCH ALL SENSOR PORTS TO MATCH OUR ROBOT
*/



const string GYRO_PORT = "S1";
const string COLOR_PORT = "S2";
const string ULTRASONIC_PORT = "S3";
const string TOUCH_PORT = "S4";

// function to configure all sensors
void configureAllSensors ()
{
	SensorType[S1] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[S2] = sensorEV3_Ultrasonic;
	wait1Msec(50);
	SensorType[S3] = sensorEV3_Color;
	wait1Msec(50);
	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[S3] = modeEV3Color_Color;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
	return;
}

// drive function
void drivePower(int motorPower) {
	motor[motorA] = motorPower;
	motor[motorD] = -motorPower;
	return;
}

// drive a certain distance in cm function
void driveDist(float distCM, int motorPow) {
	const int encoderLimit=distCM*180/(PI*2.75);
	nMotorEncoder[motorB]=0;
	motor[motorB]=motor[motorC]=motorPow;
	while (nMotorEncoder[motorB]<encoderLimit)
	{ }
	motor[motorB]=motor[motorC]=0;
}

// stop driving function
void driveStop() {
	motor[motorA]=motor[motorD]=0;
	return;
}

// turn clockwise function
void turnCW(int angle, int motorPower) {
	resetGyro(0);
	motor[motorA] = motorPower;
	motor[motorD] = motorPower;
	while(SensorValue[GYRO_PORT] < angle)
	{ }
	driveStop();
	wait1Msec(2000);
	return;
}

// function to choose the mode
int modeChoose() {
// input verification
	int mode = 0;
	while(!(mode==1||mode==2))
	{
		displayString (5, "Choose your mode:");
		displayString (7, "Press LEFT button for:");
		displayString (8, "Study Mode");
		displayString (10, "Press RIGHT button for:");
		displayString (11, "Test Mode");
// study mode=1, test mode=2
		if (getButtonPress(LEFT_BUTTON))
			{
			mode=1;
			}

		if (getButtonPress(RIGHT_BUTTON))
			{
			mode=2;
			}
	}
	eraseDisplay();
	return mode;
}

// dispense function someone make the menu
void dispense () {

}

// study mode
void studyMode (int tapeColour, int turnColour, int &lapCount)
{
	for (int count=0; count<4; count++)
	{
		drivePower(10);
		while (SensorValue[COLOR_PORT] == tapeColour)
		{
			if (SensorValue[TOUCH_PORT] == 0)
			{
				driveStop();
				dispense();
			}
		}
		while (!(SensorValue[COLOR_PORT]==tapeColour||SensorValue[COLOR_PORT]==turnColour)) {
			motor[motorA]=motor[motorD]=0;
			playSoundFile("Uh-oh.rsf");
		}
		driveStop();
		turnCW(90,20);
		wait1Msec(2000);
	}
	lapCount++;
	return;
}


void fireBullet()
{

}

float timer()
{
	displayString (5,"Press Up to Begin the Timer");
	displayString (7,"Press Up Again to Stop the Timer");
	while(!getButtonPress(UP_BUTTON))
	{
	}
    while(getButtonPress(UP_BUTTON))
	{
	}
	eraseDisplay();
	time1[T1]=0;
	while(!getButtonPress(UP_BUTTON))
	{
	}
    while(getButtonPress(UP_BUTTON))
	{
	}
	displayString (5, "Your time is %f", time1[T1]/1000.0);
}

// test mode function
void testMode()
{
// timer
timer();

// file for answers
	TFileHandle fileIn;
	bool fileOkay = openReadPC(fileIn, "answers.txt");
	if (!fileOkay)
	{
		displayString(5,"Error!");
		wait1Msec(5000);
	}
	else
	{
		int questionNum=0;
		int score = 0;
		readCharPC (fileIn,questionNum);
		// putting answers into an array
		char answers[questionNum]={};
		int j=0
		while (readCharPC (fileIn,answer[j]))
		{
			j++
		}
		//drive and scan
		driveStop();
		float grade=float(score)/questionNum;
		displayString (5, �??You scored %d/%d, score, questionNum);
			displayString( (6, �??%f�?�, grade)
			if (grade<60)
			{
				fireBullet(); // need to create this function
			}
	}
return;
}


// main function .....
#include "PC_FileIO.c"
*/

task main ()
{
	configureAllSensors();

	int done=1;

	int mode = -1;
	int lapCount= 0;
	while (done!=0)
	{
		bool modeInterface=true;
		while (modeInterface==true)
		{
			mode = modeChoose();
			modeInterface=false;
		}

		while (mode == 1)
			{

			studyMode((int)colorGreen, (int)colorBlack, lapCount);
			if (lapCount == 2) {
				done=0;
			}
		}

		while (mode==2)
		{
			testMode();
			time1[T2]=0;
			if ((time1[T2]/1000)>120)
			{
				done=0;
			}
		}


		// press enter button to go back to the menu this defo doesnt work like i want it so that at any point if u ress it it goes back to the
		// main menu
		if(getButtonPress(ENTER_BUTTON) == true)
		{
			modeInterface=true;
		}
	}
	return;
}
