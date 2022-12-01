#include "PC_FileIO.c"

// global constants for ports
const string GYRO_PORT = "S1";
const string COLOR_PORT = "S2";
const string ULTRASONIC_PORT = "S3";
const string TOUCH_PORT = "S4";

// number of questions (needed to define array size)
const int QUESTIONS_NUM = 10;

// drive function
void drivePower(int motorPower)
{
   motor[motorA] = -motorPower;
   motor[motorD] = motorPower;
   return;
}

void driveStop()
{
   motor[motorA]=motor[motorD]=0;
   return;
}

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
   // timer();

   // file for answers
   TFileHandle fileIn;
   bool fileOkay = openReadPC(fileIn, "answers.txt");

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
       //readCharPC (fileIn,questionNum);
       // creating answers array with each index with R to signify error
       char answers[QUESTIONS_NUM];

     //  for(int count1 =	0 ; count1 < QUESTIONS_NUM ; count1++)
		   //{
		   //  char temp = ' ';
    	//	 readCharPC(fileIn,temp);
    	//	 answers[count1] = temp;
		   //  count1++;
		   //  //Check answers
		   //  displayString(count1+1,"input: %c",answers[count1]);
		   //}



       //drive and scan
       driveStop();
       drivePower(10);


       int count = 0;

       //Drive until count is the same as the number of answers
       while(count < QUESTIONS_NUM)
       {
         	 //int temp_color = 0;
           //Need to save colors into an array when its specific 1/4 (RGBY)
           if(SensorValue[S1] == (int)colorRed ||SensorValue[S1] == (int)colorBlue ||SensorValue[S1] == (int)colorGreen ||SensorValue[S1] == (int)colorYellow)
           {
             playSoundFile("Boo.rsf");
             wait1Msec(3000);

             inputAns[count] = SensorValue[S1];
             playSoundFile("Boo.rsf");
             wait1Msec(3000);
             playSoundFile("Boo.rsf");
             while(SensorValue[S1] != ((int)colorWhite || 2))
           		{
          		}
          		playSoundFile("Boo.rsf");
          		wait1Msec(3000);
         		count++;
         	 }
       }

       driveStop();
       int correctAnswers = 0;

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

       //Color sensor values
       // a = red = 5
		   // b = green = 3
		   // c = blue = 2
		   // d = yellow = 4



       //for(int k = 0; k < QUESTIONS_NUM; k++)
       //{
       //  if(answers[k] == 'a' && inputAns[k] == 5 )
       //  {
       //      correctAnswers++;
       //  }
       //  else if(answers[k] == 'b' && inputAns[k] == 3)
       //  {
       //      correctAnswers++;
       //  }
       //  else if(answers[k] == 'c' && inputAns[k] == 2)
       //  {
       //      correctAnswers++;
       //  }
       //  else if(answers[k] == 'd' && inputAns[k] == 4)
       //  {
       //      correctAnswers++;
       //  }
       //}

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
   //configureAllSensors();
   SensorType[S1] = sensorEV3_Color;
   wait1Msec(50);
   
   SensorMode[S1] = modeEV3Color_Color;
   wait1Msec(50);

   int done=1;
   int mode = -1;
   int lapCount= 0;

   while (done!=0)
   {
       while (!(mode==1||mode==2))
       {
           mode = modeChoose();
       }

       //if (mode == 1)
       //{
       //    while (lapcount<2)
       //    {
       //        studyMode((int)colorGreen, (int)colorBlack, lapCount);
       //    }
       //    done=0;
       //}

       if (mode==2)
       {
           testMode();
           done=0;
       }
   }
   return;
}
