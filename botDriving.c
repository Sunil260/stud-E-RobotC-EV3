// global constants for ports
const string COLOR_PORT = "S1";
const string GYRO_PORT = "S2";
const string TOUCH_PORT = "S3";
const string ULTRASONIC_PORT = "S4";

//constant for array
const QUESTIONS_NUM = 10;

// main function .....
#include “PC_FileIO.c”

// function to configure all sensors
void configureAllSensors ()
{
	SensorType[TOUCH_PORT] = sensorEV3_Touch;
	wait1Msec(50);
	SensorType[ULTRASONIC_PORT] = sensorEV3_Ultrasonic;
	wait1Msec(50);
	SensorType[COLOR_PORT] = sensorEV3_Color;
	wait1Msec(50);
	SensorType[GYRO_PORT] = sensorEV3_Gyro;
	wait1Msec(50);
	SensorMode[COLOR_PORT] = modeEV3Color_Color;
	wait1Msec(50);
	SensorMode[GYRO_PORT] = modeEV3Gyro_Calibration;
	wait1Msec(50);
	SensorMode[GYRO_PORT] = modeEV3Gyro_RateAndAngle;
	wait1Msec(50);
	return;
}

// drive function
void drivePower(int motorPower) 
{
	motor[motorA] = motorPower;
	motor[motorD] = -motorPower;
	return;
}

// drive a certain distance in cm function
void driveDist(float distCM, int motorPow) 
{
	const int encoderLimit=distCM*180/(PI*2.75);
	nMotorEncoder[motorB]=0;
	motor[motorB]=motor[motorC]=motorPow;
	while (nMotorEncoder[motorB]<encoderLimit)
	{ }
	motor[motorB]=motor[motorC]=0;
}

// stop driving function
void driveStop() 
{
	motor[motorA]=motor[motorD]=0;
	return;
}

// turn clockwise function
void turnCW(int angle, int motorPower) {
    resetGyro(S2);
	motor[motorA] = -motorPower;
	motor[motorD] = -motorPower;
	while(SensorValue[S2] < angle)
	{ }
	driveStop();
	wait1Msec(2000);
	return;
}

// function to choose the mode
int modeChoose() 
{
	int mode = 0;
	// input verification
	while(!(mode==1||mode==2))
	{
		// Mode menu
		displayCenteredBigTextLine (2, "Mode Menu");
		displayBigTextLine (5, "Press LEFT for");
		displayBigTextLine (7, "Study Mode");
		displayBigTextLine (10, "Press RIGHT for");
		displayBigTextLine (12, "Test Mode");

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

//resets the variables for the skittle menu
void reset_char(string & var1, string & var2, string & var3, string & var4, string & var5)
{
	var1 = ">";
	var2 = " ";
	var3 = " ";
	var4 = " ";
	var5 = " ";
}

//changes the selected menu option for skittles
void change_select(int & choice, string & Yellow_char, string & Red_char, string & Green_char,string & Purple_char,string & Orange_char)
{
	if(choice < 0)
		choice = 4;
	if(choice > 4)
		choice = 0;
	if(choice == 0)
		reset_char(Yellow_char, Red_char, Green_char, Purple_char, Orange_char);
	else if (choice == 1)
		reset_char(Red_char, Green_char, Purple_char, Orange_char, Yellow_char);
	else if (choice == 2)
		reset_char(Green_char, Purple_char, Orange_char, Yellow_char,Red_char);
  	else if (choice == 3)
  		reset_char(Purple_char, Orange_char, Yellow_char,Red_char,Green_char);
	else if (choice == 4)
  		reset_char(Orange_char, Yellow_char,Red_char,Green_char,Purple_char);
} 

//menu for selecting the skittles
int skittleSelect()
{
	int choice = 0; 
	string Yellow_char = " ";
	string Red_char= " ";
	string Green_char = " ";
	string Purple_char = " " ;
	string Orange_char = " ";
	
	while(!getButtonPress(ENTER_BUTTON))
	{
		if (getButtonPress(DOWN_BUTTON))
		{
			choice++;
			while(getButtonPress(DOWN_BUTTON))
		  	{
			}
		}
		
		if(getButtonPress(UP_BUTTON))
	  	{
			choice--;
			while(getButtonPress(UP_BUTTON))
			{
			}
	  	}
		change_select(choice, Yellow_char, Red_char, Green_char, Purple_char, Orange_char);
		displayCenteredBigTextLine(2, "Skittle Menu");
		displayBigTextLine(5,"%s Yellow", Yellow_char);
		displayBigTextLine(7,"%s Red", Red_char);
		displayBigTextLine(9,"%s Green", Green_char);
		displayBigTextLine(11,"%s Purple", Purple_char);
		displayBigTextLine(13,"%s Orange", Orange_char);
		displayBigTextLine(15,"%d",choice);
 	}
  return choice;
}

// function to open the flap
void open_flap(int time_open)
{
	nMotorEncoder(motorB) = 0;
	motor[motorB] = -100;
	while (nMotorEncoder(motorB) > -1500)
	{
	}
	motor[motorB] = 0;
	time1[T1]=0;
	int shake_degree = nMotorEncoder(motorC);
	while(time1[T1] <=time_open)
  	{

		motor[motorC] = 25;
		while (nMotorEncoder(motorC) < (shake_degree +10) && time1[T1] <=time_open)
		{
		}
		motor[motorC]= -25;
		while (nMotorEncoder(motorC) > (shake_degree-10) && time1[T1] <=time_open)
		{
		}
  	}
  	motor[motorC]= -15;
	while (nMotorEncoder(motorC) > (shake_degree-10) && time1[T1] <=time_open)
	{
	}
  	motor[motorC]=0;
	motor[motorB] = 100;
	while (nMotorEncoder(motorB) < 0)
	{
	}
	motor[motorB] = 0;
}

// dispensing fucntion
void dispense()
{
	const int TIME_OPEN_FLAP_M = 2000;
	int degree_deviation = -2;
	//const TIME_OPEN_FLAP_M = 2000;
	bool dispensing = true;
	
	while(dispensing == true)
	{
		int skittle_selected = skittleSelect();
		int rotate = -1*skittle_selected*60;
		nMotorEncoder(motorC) = 0;
		motor[motorC]= -15;
		while(nMotorEncoder(motorC) >  rotate)
		{
		}
		motor[motorC]=0;
		open_flap(TIME_OPEN_FLAP_M);
		motor[motorC]= 15;
		while(nMotorEncoder(motorC) <(0 + degree_deviation))
		{
		}
		motor[motorC]=0;
	}
}

// study mode IT WORKS but fix something!!
void studyMode (int tapeColour, int turnColour)
{
  int studyLoop=0; // # of loops it has done
    while (studyLoop<=2) // if loops>2, then it exits the function
    {
        for (int turnCount=0;turnCount<=4;turnCount++) // 4 turns in one loop
        {
            drivePower(20);
            while (SensorValue(COLOR_PORT)==tapeColour||SensorValue(COLOR_PORT)==1) // while it senses the tape colour or black because of colour variation
            {
                if (SensorValue(TOUCH_PORT)==1) // if the touch button is pushed
                {
                    driveStop(); // stop 
                    dispense() // dispense a skittle of choosing
                    drivePower(20); // continue driving
                    studyLoop=0; // set loop count back to 0
                }
            }
            if (SensorValue(COLOR_PORT)==turnColour) // turns at the corners
            {
                driveStop(); // stops
                turnCW(87,20); // turns at 87 because 90 over rotates the bot
                drivePower(20); // drive forward a bit so it doesnt sense white anymore
                wait1Msec(1000);
            }
            else // if it sees an unrelated colour, fix this cuz if it senses this it adds to turn loop
			{
                driveStop(); // stop
                setSoundVolume(100);
                playSoundFile("Uh-oh.rsf"); // play uh oh sound
                while (!(SensorValue(COLOR_PORT)==tapeColour||SensorValue(COLOR_PORT)==1)) // wait until it gets placed back on track
				{
				}
            }
        }
        studyLoop++;
    }
}

// hourglass spinning
void hourglassVisual() 
{
	time1[T1] = 0;

	drawBmpfile(0,127,"Hourglass 0"); // top full
	while(time1[T1] <= 2000 && (!getButtonPress(UP_BUTTON)))
	{
	}

	drawBmpfile(0,127,"Hourglass 1"); // bottom full
	while(time1[T1] <= 4000 && (!getButtonPress(UP_BUTTON)))
 	{
	}

	drawBmpfile(0,127,"Hourglass 2"); // turn
	while(time1[T1] <= 4800 && (!getButtonPress(UP_BUTTON)))
	{
	}
}

// test timer
void timer() 
{
	displayCenteredBigTextLine(3,"Press up to");
	displayCenteredBigTextLine(6,"begin the timer");
	displayCenteredBigTextLine(9,"and");
	displayCenteredBigTextLine(12,"end the timer");

	while(!getButtonPress(UP_BUTTON)) 
	{
	}
    while(getButtonPress(UP_BUTTON))
	{
	}

	time1[T2]=0;
	eraseDisplay();

	while(!getButtonPress(UP_BUTTON)) 
	{
		hourglassVisual();
	}
    while(getButtonPress(UP_BUTTON))
	{
	}

	int totalSeconds = time1[T2]/1000.0;
	int testSeconds = 0, testMinutes = 0, testHours = 0;
	eraseDisplay();

	testMinutes = totalSeconds / 60;
	testSeconds = totalSeconds % 60;
	testHours = testMinutes / 60;
	testMinutes = testMinutes % 60;

	displayCenteredBigTextLine(2,"Your time is:");
	displayBigTextLine (6, "%d hours", testHours);
	displayBigTextLine (9, "%d minutes", testMinutes);
	displayBigTextLine (12, "%d seconds", testSeconds);

	wait1Msec(5000);
}

// calculates test score
float calculateGrade (int writtenAnswer, int trueAnswer) 
{
	int grade=(writtenAnswer/trueAnswer)*100;
	return grade;
}

// test mode function
void testMode() 
{ 
	// timer
	timer();

	// file for answers
	TFileHandle fileIn;
	bool fileCondition = openReadPC(fileIn, "answers.txt");
	if (!fileCondition)
	{
		displayString(5,"Error!");
		wait1Msec(5000);
	}
	//Array for collected answer
	string inputAns [QUESTIONS_NUM-1] = {""};

	else
	{
		//readCharPC (fileIn,questionNum);
		// putting answers into an array
		char answers[QUESTIONS_NUM-1]={''};

		int questionNum=0;
		while (readCharPC (fileIn,answers[questionNum]))
		{
			questionNum++
		}

		//drive and scan
		driveStop();
		drivePower(20);

		int answerNum = 0;
		//Drive until black color
		while ( SensorValue[COLOR_PORT] != (int)colorBlack )
		{
			//Need to save colors into an array when its specific 1/4 (RGBY)
			if(SensorValue[COLOR_PORT] == (int)colorRed ||SensorValue[COLOR_PORT] == (int)colorBlue ||SensorValue[COLOR_PORT] == (int)colorGreen ||SensorValue[COLOR_PORT] == (int)colorYellow)
			{
				inputAns[i]	== SensorValue[COLOR_PORT].color;
				answerNum++;
			}

		}
		driveStop();

		int correctAnswers = 0;

		for (int count = 0; count < QUESTIONS_NUM ; count++)
		{
			if(inputAns[count] == answers[count])
			{
				correctAnswers++;
			}
		}
		
		float grade = calculateGrade(correctAnswers, QUESTIONS_NUM);
		
		displayCenteredBigTextLine (5, "You scored %d/%d", correctAnswers, QUESTIONS_NUM);
		displayCenteredBigTextLine (9, "%f percent", grade);

		if (grade<60) 
		{
			playSoundFile("Boo.rsf");
			wait1Msec(5000);
		}
		else if (grade=>60) 
		{
			playSoundFile("Good job.rsf");
			wait1Msec(5000);
		}
		else 
		{
			playSoundFile("Error.rsf");
			wait1Msec(5000);
		}
	}
}

task main ()
{
	configureAllSensors();

	int done=1;
	int mode = -1;

	while (done!=0)
	{
		while (!(mode==1||mode==2))
		{
			mode = modeChoose();
		}

		if (mode == 1)
		{
			studyMode((int)colorGreen, (int)colorWhite);
			done=0;
		}

		else if (mode==2)
		{
			testMode();
			done=0;
		}
	}
	return;
}
