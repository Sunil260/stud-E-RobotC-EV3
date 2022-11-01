/*
Group 28 
Driving code 
*/


// function to configure all sensors
void configureAllSensors () 
{
	SensorType[S1] = sensorEV3_Touch; 
	wait1MSec(50); 
	SensorType[S2] = sensorEV3_Ultrasonic; 
	wait1MSec(50);
	SensorType[S3] = sensorEV3_Color; 
	wait1MSec(50);
	SensorType[S4] = sensorEV3_Gyro; 
	wait1MSec(50);
	SensorMode[S3] = modeEV3Color_Color; 
	wait1MSec(50);
	SensorMode[S4] = modeEV3Gyro_Calibration; 
	wait1MSec(50); 
	SensorMode[S4] = modeEV3Gyro_RateAndAngle; 
	wait1MSec(50);
	return;
}

// drive function
void drivePower(int motorPower) {
	motor[motorA]=motor[motorD]=motorPower;
	return;
}

// driveDist cm function
void driveDist(float distCM, int motorPow) {
	const int encoderLimit=distCM*180/(PI*2.75);
	nMotorEncoder[motorB]=0;
	motor[motorB]=motor[motorC]=motorPow;
	while (nMotorEncoder[motorB]<encoderLimit)
	{ }
	motor[motorB]=motor[motorC]=0;
}

// stop drive function
void driveStop() {
	motor[motorA]=motor[motorD]=0;
	return;
}

// turn clockwise function
void turnCW(int angle, int motorPower) {
	resetGyro(S1);
motor[motorA] = -motorPower;
motor[motorD] = motorPower;
while(SensorValue[S1] < angle)
{ }
driveStop();
wait1Msec(2000);
return;	
}


// function to choose the mode
int modeChoose () {
// input verification
 while(!(mode==1||mode==2)) {
int mode=0;
	
displayString (5, “Choose your mode:”);
displayString (7, “Press left button for Study Mode”);
displayString (8, “Press right button for Test Mode”);
// study mode=1, test mode=2
if (getButtonPress==LEFT_BUTTON) {
mode=1;
}

if (getButtonPress==RIGHT_BUTTON) {
mode=2;
}
}
return mode;
}
// follow a line function
void followLine (int tapeColour) {
	while (SensorValue[]==tapeColour) {
		drivePower(20);
	}
	driveDist(howeverlong, 20);
	turnCW(90,20);
	return;
} 

// follow line in a square function
void lineLoop() {
for (int count=1; count<=4; count++) {
	followLine(int(colorRed));
}
	return;
}




// main function
task main () {
configureAllSensors();

while (!done) {
bool modeInterface=true;
while (modeInterface==true) {
	modeChoose();
	modeInterface=false;
}

while (mode==1) {
	//pnrng
}

	while (mode==2) {
	//pnrng
}

	// press enter button to go back to the menu
	if (getButtonPress==ENTER_BUTTON) {
		modeInterface=true;
	}
}
return;
}

task main()
{

}
