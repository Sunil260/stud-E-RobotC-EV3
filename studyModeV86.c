#include "PC_FileIO.c"

// global constants for ports


// number of questions (needed to define array size)
//Potentially move to test mode
const int QUESTIONS_NUM = 10;




// function to configure all sensors using standard config
void configureAllSensors ()
{
	SensorType[S1] = sensorEV3_Touch;
	wait1Msec(100);
	SensorType[S2] = sensorEV3_Ultrasonic;
	wait1Msec(100);
	SensorType[S3] = sensorEV3_Color;
	wait1Msec(100);
	SensorMode[S3] = modeEV3Color_Calibration;
	wait1Msec(100);
	SensorMode[S3] = modeEV3Color_Color;
	wait1Msec(100);
	SensorType[S4] = sensorEV3_Gyro;
	wait1Msec(100);
	SensorMode[S4] = modeEV3Gyro_Calibration;
	wait1Msec(100);
	SensorMode[S4] = modeEV3Gyro_RateAndAngle;
	wait1Msec(100);
	return;
}

// drive function taking even power
void drivePower(int motorPower)
{
	motor[motorA] = -motorPower;
	motor[motorD] = motorPower;
	return;
}

// stop driving function setting the power to 0
void driveStop()
{
	motor[motorA]=motor[motorD]=0;
	return;
}

// turn clockwise function
void turnCW(int angle, int motorPower) {
    resetGyro(S4);
	motor[motorA] = -motorPower;
	motor[motorD] = -motorPower;
	while(abs(SensorValue[S4]) < abs(angle))
	{
		displayString(5,"value: %d",SensorValue[S4]);
	}
	driveStop();
	wait1Msec(2000);
	return;
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
	//resets counter for menu if it goes out of its range
	if(choice < 0)
		choice = 4;
	if(choice > 4)
		choice = 0;
	//resets which skittle is selected based on choice
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
	eraseDisplay();
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

 	}
  return choice;
}

// function to open the flap
void open_flap(int time_open)
{
	nMotorEncoder(motorB) = 0;
	motor[motorB] = -5;
	while (nMotorEncoder(motorB) > -45)
	{
	}
	motor[motorB] = 0;
	time1[T1]=0;
	int shake_degree = nMotorEncoder(motorC);
	while(time1[T1] <=time_open)
  {

		motor[motorC] = 75;
		while (nMotorEncoder(motorC) < (shake_degree +7) && time1[T1] <=time_open)
		{
		}

		motor[motorC]= -75;
		while (nMotorEncoder(motorC) > (shake_degree-7) && time1[T1] <=time_open)
		{
		}
  }

  motor[motorC]=0;
	motor[motorB] = 5;
	while (nMotorEncoder(motorB) < 0)
	{
	}
	motor[motorB] = 0;
}
// asks user if they want to keep selecting skittles
bool keep_dispencing()
{

	eraseDisplay();
	displayBigTextLine(2,"Need More?");
	displayBigTextLine(4,"Up For Yes ");
	displayBigTextLine(6,"Down For No");

	while(!getButtonPress(UP_BUTTON) && !getButtonPress(DOWN_BUTTON))
	{}
	if (getButtonPress(UP_BUTTON))
	{
		while(getButtonPress(UP_BUTTON)){}
		return true;
	}
	eraseDisplay();
	return false;

}
//checks if the skittle seleceted has any skiltles left
bool empty_check()
{
	if (SensorValue[S2] <= 7.5)
		return true;
	return false;
}
// dispensing fucntion
void dispense()
{
	const int TIME_OPEN_FLAP_M = 2000;
	int degree_deviation = 0;
	//const TIME_OPEN_FLAP_M = 2000;
	do
	{
		int skittle_selected = skittleSelect();
		int rotate = -1*skittle_selected*60;
		nMotorEncoder(motorC) = 0;
		motor[motorC]= -5;
		while(nMotorEncoder(motorC) >  rotate)
		{
		}
		motor[motorC]=0;
		if (empty_check())
		{
				open_flap(TIME_OPEN_FLAP_M);
		}
		motor[motorC]= 5;
		while(nMotorEncoder(motorC) <(0 + degree_deviation))
		{
		}
		motor[motorC]= -5;
		while(nMotorEncoder(motorC) >(0 + degree_deviation))
		{
		}
		motor[motorC]=0;


	}while(keep_dispencing());
}

void studyMode (int tapeColour, int turnColour)
{
	setSoundVolume(100);

	int studyLoop = 0; // # of untouched loops it has done
  while (studyLoop <2) // if inactive loops >= 2, then it exits the function and ends the program
  {//1
  	int turnCount=0;
		while (turnCount < 4)
		{//2
			drivePower(10);
			while (SensorValue[S3]==turnColour)
			{//3
		  }//3
			while (SensorValue[S3] == tapeColour) // 2 and 1 for error correction (sometimes sees 2/blue or 1/black)
			{//4
				//Stop while in studymode for dispense
				if (SensorValue[S1]==1)
				{
					driveStop();
					playSoundFile("Hi.rsf");
					dispense();
					playSoundFile("Goodbye.rsf");
					wait1Msec(2000);
					drivePower(10); // continue driving
          studyLoop=0; // set loop count back to 0
				}
			}//4
			while(SensorValue[S3] == 1)
			{}
			wait1Msec(500);
			while(SensorValue[S3] != tapeColour && SensorValue[S3] != turnColour)
			{
				displayString(6,"v: %d",SensorValue[S3]);

				driveStop();

				playSoundFile("Uh-oh.rsf"); // play uh oh sound
				wait1Msec(1000);
			}

			if(SensorValue[S3] == turnColour)
			{
				drivePower(10);
				wait1Msec(1400);
				// turns at 87 because 90 over rotates the bot
				turnCW(91,20);
				wait1Msec(500);
				turnCount++;
			}
				//while (!(SensorValue[S3] == tapeColour||SensorValue[S3] == 2))
				//{
				//}



			drivePower(10); // drive forward a bit so it doesnt sense white anymore
		}//2
	studyLoop++;
	playSoundFile("One.rsf");
	}//1

	playSoundFile("Goodbye.rsf");
	wait1Msec(5000);
	return;
}//end of function

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


// hourglass spinning
void hourglassVisual()
{
   time1[T1] = 0;

   drawBmpfile(0,127,"Hourglass 0");
   while(time1[T1] <= 2000 && (!getButtonPress(UP_BUTTON)))
   {
   }

   drawBmpfile(0,127,"Hourglass 1");
   while(time1[T1] <= 4000 && (!getButtonPress(UP_BUTTON)))
   {
   }

   drawBmpfile(0,127,"Hourglass 2");
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
   float grade = (writtenAnswer/(float)trueAnswer)*100;
   return grade;
}

// test mode function
void testMode()
{

   // timer
   timer();

   // file for answers
   TFileHandle fileIn;
   bool fileOkay = openReadPC(fileIn, "answers.txt");

	 const int STOP_DELAY = 500;
	 const int START_DELAY = 1500;

	 //Array for collected answer
   int inputAns[QUESTIONS_NUM];

   // fills the inputAns array with blank spaces in each index
  /* for(int count1 =	QUESTIONS_NUM ; count1 > 0 ; count1--)
   {
     int count1 = 0;
     inputAns[count1] = -1;
     count1++;
   }*/


   //Checks if file can open
   if (!fileOkay)
   {
       displayString(5,"Error!");
       wait1Msec(5000);
   }
   else
   {

       //drive and scan
       setSoundVolume(100);
       drivePower(7);
       displayString(1,"Initial start");

       //At the start wait untill the color black i s presented then enter the test loop
       while(SensorValue[S3] != ((int)colorBlack))
       {
       }
       drivePower(7);
       wait1Msec(STOP_DELAY);
		 	 driveStop();

		 	 eraseDisplay();
       displayString(1,"Started scanning");
			playSoundFile("One.rsf");
       playSoundFile("Black.rsf");
       playSoundFile("Detected.rsf");
       wait1Msec(START_DELAY);
       drivePower(7);
       //Drive until we get the right number of answers
       int questionCount = 0;
       while(questionCount < QUESTIONS_NUM)
       {


           if(SensorValue[S3] == (int)colorRed  ||SensorValue[S3] == (int)colorGreen || SensorValue[S3] == (int)colorBlue
              || SensorValue[S3] == (int)colorYellow)
           {
             wait1Msec(STOP_DELAY);
             driveStop();
             playSoundFile("Two.rsf");
             if (SensorValue[S3] == (int)(colorBlack))
             {
               playSoundFile("White.rsf");
               playSoundFile("Detected.rsf");
             }
             if (SensorValue[S3] == (int)(colorBlue))
             {
               playSoundFile("Blue.rsf");
             playSoundFile("Detected.rsf");
               }
             if (SensorValue[S3] == (int)(colorGreen))
             {
               playSoundFile("Green.rsf");
               playSoundFile("Detected.rsf");
             }
             if (SensorValue[S3] == (int)(colorYellow))
             {
               playSoundFile("Yellow.rsf");
               playSoundFile("Detected.rsf");
             }
             if (SensorValue[S3] == (int)(colorRed))
             {
               playSoundFile("Red.rsf");
               playSoundFile("Detected.rsf");
             }
             if (SensorValue[S3] == (int)(colorWhite))
             {
               playSoundFile("White.rsf");
               playSoundFile("Detected.rsf");
             }
             if (SensorValue[S3] == (int)(colorBrown))
             {
               playSoundFile("Brown.rsf");
               playSoundFile("Detected.rsf");
             }
             //Saving the value from scan
             inputAns[questionCount] = SensorValue[S3];
						wait1Msec(START_DELAY);
             drivePower(7);

             while(SensorValue[S3] != (int)colorBlack )
           	 {}
          	 wait1Msec(STOP_DELAY);
          		driveStop();
							playSoundFile("Three.rsf");
          		playSoundFile("Detected.rsf");
          		playSoundFile("Black.rsf");
          		wait1Msec(START_DELAY);
         			drivePower(7);
         			questionCount++;
         	 }
       }

       driveStop();
       int correctAnswers = 0;


        //Color sensor values
       // a = red = 5
		   // b = green = 3
		   // c = blue = 2
		   // d = yellow = 4
       for(int count1 =	0 ; count1 < QUESTIONS_NUM ; count1++)
		   {
		     char temp = ' ';
    		 readCharPC(fileIn,temp);

    		 if(temp == 'a' && inputAns[count1] == 5 )
         {
             correctAnswers++;
         }
         else if(temp == 'b' && inputAns[count1] == 3)
         {
             correctAnswers++;
         }
         else if(temp == 'c' && inputAns[count1] == 2)
         {
             correctAnswers++;
         }
         else if(temp == 'd' && inputAns[count1] == 4)
         {
             correctAnswers++;
         }
		     //Check answers
		   }

       //Checking answers then calculating the mark

       float grade = calculateGrade(correctAnswers, QUESTIONS_NUM);

       displayCenteredBigTextLine(2,"num: %d",correctAnswers);
       displayCenteredBigTextLine (5, "You scored %d/%d", correctAnswers, QUESTIONS_NUM);
       displayCenteredBigTextLine (9, "%f percent", grade);
 				wait1Msec(5000);
       if(grade < 60)
       {
           playSoundFile("Boo.rsf");
           wait1Msec(5000);
       }
       else if(grade >= 60)
       {

           playSoundFile("Good job.rsf");
           wait1Msec(5000);
       }
   	}
	}

task main ()
{
	configureAllSensors();
	int done = 1;
	int mode = -1;

	while (done!=0)
	{

		mode = modeChoose();

		if (mode == 1)
		{
			studyMode((int)colorGreen, (int)colorRed);
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
