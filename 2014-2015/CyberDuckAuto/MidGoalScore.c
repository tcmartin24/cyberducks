#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Sensor, S3,     GYRO,           sensorAnalogInactive)
#pragma config(Sensor, S4,     HTSMUX,         sensorI2CCustom)
#pragma config(Motor,  mtr_S1_C1_1,     rightFront,    tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     rightBack,     tmotorTetrix, PIDControl, reversed, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     leftBack,      tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_2,     leftFront,     tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S2_C2_1,     beaterBar,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_2,     lift,          tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S2_C1_1,    backRightServo,       tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    backLeftServohh,      tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    topServo,             tServoStandard)
#pragma config(Servo,  srvo_S2_C1_4,    backLeftServo,        tServoStandard)
#pragma config(Servo,  srvo_S2_C1_5,    sdfgbhnm,             tServoStandard)
#pragma config(Servo,  srvo_S2_C1_6,    sdfghj,               tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "JoystickDriver.c"

#include "../drivers-3x/hitechnic-sensormux.h"
#include "../drivers-3x/hitechnic-irseeker-v2.h"
#include "../drivers-3x/hitechnic-gyro.h"

float GyroTolerance = 6;
float currHeading = 0;
float ofset = 28;
float ti = 20;
const short ROTDISTANCE = 8.37758040957278;

float high = -11.25*360*4;
float mid = -8.25*360*4;
float low = -4.5*360*4;
// Task to keep track of the current heading using the HT Gyro
task getHeading () {
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
          //motor[rightBack] = power+ti;
	  			//motor[rightFront] = power+ti;
        		motor[rightBack] = -power;
						motor[leftBack] = power;
						motor[leftFront] = power;
						motor[rightFront] = -power;
						PlayImmediateTone(1024, 10);
      } else if (SubtractFromCurrHeading(straightHead) < -GyroTolerance) {
        	//motor[leftBack] = power+ti;
	  			//motor[leftFront] = power+ti;
      		 	motor[rightBack] = power;
						motor[leftBack] = -power;
						motor[leftFront] = -power;
						motor[rightFront] = power;
						PlayImmediateTone(1024, 10);
      } else  {
          xSet(power);
      }
}
void forwardInc(float inches, float power){
	nMotorEncoder[rightBack] = 0;
  nMotorEncoder[leftBack] = 0;

  int head = currHeading;
	if(inches > 0){
	  while(nMotorEncoder[rightBack] < (inches/ROTDISTANCE)*360 || nMotorEncoder[leftBack] < (inches/ROTDISTANCE)*360){GoStraightByGyro(head, power);}
	}else{
	  while(nMotorEncoder[rightBack] > (inches/ROTDISTANCE)*360 || nMotorEncoder[leftBack] > (inches/ROTDISTANCE)*360){GoStraightByGyro(head, -power);}
	}
	xSet(0);
}
void turnDeg(float deg, float power){
	if(deg > 0){
		//currHeading -= ofset;
		motor[leftBack] = power;
		motor[leftFront] = power;
		motor[rightFront] = -power;
  	motor[rightBack] = -power;
  	while(deg >= currHeading){
			wait1Msec(1);
  		nxtDisplayTextLine(3, "head: %3.0f", currHeading);
  		nxtDisplayTextLine(5, "diff: %3.0f", SubtractFromCurrHeading (deg));
		}
	}else{
	  //currHeading += ofset;
		motor[leftBack] = -power;
		motor[leftFront] = -power;
		motor[rightFront] = power;
  	motor[rightBack] = power;
  	while(deg <= currHeading){
			wait1Msec(1);
  		nxtDisplayTextLine(3, "head: %3.0f", currHeading);
 			nxtDisplayTextLine(5, "diff: %3.0f", SubtractFromCurrHeading (deg));
		}
	}
	xSet(0);
}
void liftPos(float x){
	servo[topServo] = 235;
	if(x == 1){
		x = low;
	}else if(x == 2){
		x = mid;
	}else if(x == 3){
		x = high;
	}else if(x == 0){
		x = 0;
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
}
void servoPos(short x, bool b){
	servo[backLeftServo] = x;
	if(b){
		servo[backRightServo] = 256;
		if(ServoValue[backRightServo] > servoTarget[backRightServo]){
				servo[backRightServo] = 127;
		}
	}else{
		servo[backRightServo] = 0;
		if(ServoValue[backRightServo] < servoTarget[backRightServo]){
				servo[backRightServo] = 127;
		}
	}
}
void initializeRobot(){
	  servo[topServo] = 235;
	  nMotorEncoder[lift] = 0;
}
task main(){
	initializeRobot();
	wait1Msec(200);
	//waitForStart();
	wait1Msec(100);
	StartTask(getHeading);
	wait1Msec(500);
	servoPos(50, true);
	motor[beaterBar] = -30;
	forwardInc(-100, 15);
	forwardInc(-48, 20);
	motor[beaterBar] = 0;
	servoPos(190, false);
	wait1Msec(1000);
	liftPos(2);
	servo[topServo] = 100;
	wait1Msec(3000);
	liftPos(0);
	wait1Msec(500);
	StopTask(getHeading);
	currHeading = 0;
	StartTask(getHeading);
	wait10Msec(50);
	turnDeg(30, 20);
	wait10Msec(20);
	forwardInc(175, 40);
	wait10Msec(40);
	turnDeg(180, 30);
}
