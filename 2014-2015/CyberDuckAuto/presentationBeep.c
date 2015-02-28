#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     GYRO,           sensorAnalogInactive)
#pragma config(Sensor, S4,     HTSMUX,         sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     rightFront,    tmotorTetrix, openLoop, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     rightBack,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     leftBack,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     leftFront,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_1,     beaterBar,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_2,     lift,          tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S2_C1_1,    backRightServo,       tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    backLeftServo,        tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    topServo,             tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    fghjk,                tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    sdfgbhnm,             tServoStandard)
#pragma config(Servo,  srvo_S2_C1_6,    sdfghj,               tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//#include "JoystickDriver.c"

#include "../drivers-3x/hitechnic-sensormux.h"
#include "../drivers-3x/hitechnic-irseeker-v2.h"
#include "../drivers-3x/hitechnic-gyro.h"
#include "../drivers-3x/hitechnic-eopd.h"
#include "../drivers-3x/lego-light.h"

const tMUXSensor IRS1 = msensor_S4_1;
const tMUXSensor IRS2 = msensor_S4_2;
const tMUXSensor HTEOPD = msensor_S4_3;
const tMUXSensor LEGOLS = msensor_S4_4;

float GyroTolerance = 6;
float currHeading = 0;
float ofset = 28;
float ti = 15;
const short ROTDISTANCE = 8.37758040957278;

float high = -11.25*360*4;
float mid = -8*360*4;
float low = -4.5*360*4;
// Task to keep track of the current heading using the HT Gyro
int acS1a, acS2a, acS3a, acS4a, acS5a = 0;
int acS1b, acS2b, acS3b, acS4b, acS5b = 0;
int th = 15;
task infared(){
	int _dirAC1 = 0;
  //int 1acS1, 1acS2, 1acS3, 1acS4, 1acS5 = 0;
  int _dirAC2 = 0;
  //int 2ac2S1, 2acS2, 2acS3, 2acS4, 2acS5 = 0;

	while(true){
		_dirAC1 = HTIRS2readACDir(IRS1);
    if (_dirAC1 < 0)
      break; // I2C read error occurred

    _dirAC2 = HTIRS2readACDir(IRS2);
    if (_dirAC2 < 0)
      break; // I2C read error occurred

    if (!HTIRS2readAllACStrength(IRS1, acS1a, acS2a, acS3a, acS4a, acS5a))
      break; // I2C read error occurred
    if (!HTIRS2readAllACStrength(IRS2, acS1b, acS2b, acS3b, acS4b, acS5b))
      break; // I2C read error occurred
	}
}
task getHeading(){
	float delTime = 0;
	float prevHeading = 0;
	float curRate = 0;

  HTGYROstartCal(GYRO);
  //PlaySound(soundBeepBeep);
  while (true) {
    time10[T1] = 0;
    curRate = HTGYROreadRot(GYRO);
    if (abs(curRate) > 3) {
      prevHeading = currHeading;
      currHeading = prevHeading + curRate * delTime;
      if (currHeading > 360) currHeading -= 360;
      else if (currHeading < 0) currHeading += 360;
    }
    wait1Msec(5);
    delTime = ((float)time1[T1]) / 1000;
    //delTime /= 1000;
  }
}
// heading is never negative, but it can flip from 1 to 359, so do modulus 180
float SubtractFromCurrHeading (float x) {
  float result = 0.0;
  float ch = currHeading;
  float diff = abs(ch - x);
  if (diff >= 180) { // more than 180d neg apart, so flip
    result = 360.0 - diff;
    if (x < 180) { result = -result; }
  } else {
    result = ch - x;
  }
  return result;
}
void xSet(float power){
	motor[rightBack] = power;
	motor[leftBack] = power;
	motor[leftFront] = power;
	motor[rightFront] = power;
}
void GoStraightByGyro (float straightHead, float power) {
      	// stay on heading from before button was pressed
        if (SubtractFromCurrHeading(straightHead) > GyroTolerance) {
          motor[rightBack] = power+ti;
	  			motor[rightFront] = power+ti;
      } else if (SubtractFromCurrHeading(straightHead) < -GyroTolerance) {
        	motor[leftBack] = power+ti;
	  			motor[leftFront] = power+ti;
      } else  {
          xSet(power);
      }
}
void forwardInc(float inches, float power){
	nMotorEncoder[rightBack] = 0;
  nMotorEncoder[leftBack] = 0;

  //int head = currHeading;
	if(inches > 0){
	  GoStraightByGyro(currHeading, power);

	  while(nMotorEncoder[rightBack] < (inches/ROTDISTANCE)*360 || nMotorEncoder[leftBack] < (inches/ROTDISTANCE)*360){
	  	/*
	  	if(head+4 < currHeading){
	  		motor[rightBack] = power+ti;
	  		motor[rightFront] = power+ti;
	  	}else if(head-4 > currHeading){
	  		motor[leftBack] = power-ti;
	  		motor[leftFront] = power-ti;
	  	}else{
	  		xSet(power);
	  	}
	  	*/

	  }
	}else{
		GoStraightByGyro(currHeading, -power);
	  while(nMotorEncoder[rightBack] > (inches/ROTDISTANCE)*360 || nMotorEncoder[leftBack] > (inches/ROTDISTANCE)*360){
	  	/*
	  	if(head+4 < currHeading){
	  		motor[rightBack] = power-ti;
	  		motor[rightFront] = power-ti;
	  	}else if(head-4 > currHeading){
	  		motor[leftBack] = power+ti;
	  		motor[leftFront] = power+ti;
	  	}else{
	  		xSet(power);
	  	}
	  	*/
	  }
	}
	xSet(0);
}
void turnDeg(float deg, float power){

	if(deg > 0){
		//currHeading -= ofset;
		motor[leftBack] = power;
  	motor[rightBack] = -power;
  	while(deg >= currHeading){
			wait1Msec(1);
			nxtDisplayTextLine(2, "servo: %d", ServoValue[servo1]);
  		nxtDisplayTextLine(3, "head: %3.0f", currHeading);
  		nxtDisplayTextLine(5, "diff: %3.0f", SubtractFromCurrHeading (deg));
		}
	}else{
	  //currHeading += ofset;
		motor[leftBack] = -power;
  	motor[rightBack] = power;
  	while(deg <= currHeading){
			wait1Msec(1);
			nxtDisplayTextLine(2, "servo: %d", ServoValue[servo1]);
  nxtDisplayTextLine(3, "head: %3.0f", currHeading);
  nxtDisplayTextLine(5, "diff: %3.0f", SubtractFromCurrHeading (deg));
		}
	}
	xSet(0);
}
void initializeRobot(){
	nMotorEncoder[lift] = 0;
}
void liftPos(float x){
	servo[topServo] = 235;
	if(x == 1){
		x = low;
	}else if(x == 2){
		x = mid;
	}else if(x == 3){
		x = high;
	}
	//remember that x is negative bc lift up is counting down and lift down is counting up probs
	if(nMotorEncoder[lift] >= x){
			motor[lift] = 100;
			while(nMotorEncoder[lift] >= x){}
	}else if(nMotorEncoder[lift] <= x){
   		motor[lift] = -100;
   		while(nMotorEncoder[lift] <= x){}
	}
	motor[lift] = 0;
	servo[topServo] = 100;
}


task irBeep(){

	if(th > (acS2a+acS4b/2)+10){
		while(true){
			int ir1 = acS1a+acS2a+acS3a+acS4a+acS5a;
			int ir2 = acS1b+acS2b+acS3b+acS4b+acS5b;
			ir1 = ir1/5;
			ir2 = ir2/5;
		 	PlayImmediateTone(ir1+ir2/2, 10);
		}
	}
}
task eopd(){
	int _raw = 0;
  int _processed = 0;

  // Set the sensor to short range
  HTEOPDsetShortRange(HTEOPD);

  while(true){
	  // Read the raw sensor value
    _raw = HTEOPDreadRaw(HTEOPD);
    // read the processed value which is linear with
    // the distance detected.  Use the processed value
    // when you want to determine distance to an object
    _processed = HTEOPDreadProcessed(HTEOPD);
    nxtDisplayClearTextLine(3);
    nxtDisplayClearTextLine(4);
    nxtDisplayTextLine(4, "Proc:  %4d", _processed);
    nxtDisplayTextLine(3, "Raw :  %4d", _raw);
    wait1Msec(50);
    if (_processed > 41) {
      LSsetActive(LEGOLS); // turn light on
      PlaySound(soundBeepBeep);
      while(bSoundActive){};
      LSsetInactive(LEGOLS); // turn light off
    } // if ball close
  }
}
float a = 0;
float b = 0;
float c = 0;
float d = 0;

task main()
{
	servo[topServo] = 235;
	initializeRobot();
	th = (acS2a+acS4b/2)+5;
	StartTask(infared);
	//StartTask(eopd);
	//StartTask(irBeep);

	while(true){
			int ir1 = acS1a+acS2a+acS3a+acS4a+acS5a;
			int ir2 = acS1b+acS2b+acS3b+acS4b+acS5b;
			ir1 = ir1/5;
			ir2 = ir2/5;

			if(a == b){
		 		PlayImmediateTone(1024, 10);
		 		a = 0;
			}else if(a == 0){
				b = ir1;
			}else{a++}
			if(c == d){
		 		PlayImmediateTone(440, 10);
		 		c = 0;
			}else if(c == 0){
				d = ir2;
			}else{c++;}

		 	ClearSounds();
			wait1Msec(100);
		 	nxtDisplayTextLine(4, "ir1:  %4d", ir1);
      nxtDisplayTextLine(3, "ir2:  %4d", ir2);

	}
}
