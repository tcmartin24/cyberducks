#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  none,     none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     rightFront,    tmotorTetrix, PIDControl, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     rightBack,     tmotorTetrix, PIDControl, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     leftBack,      tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S1_C2_2,     leftFront,     tmotorTetrix, PIDControl)
#pragma config(Servo,  srvo_S2_C1_1,    backLeftServo,        tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    backRightServo,       tServoStandard)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//all naming is this program are based of positive y being forwards and positive x being right
#include "JoystickDriver.c"

void initializeRobot(){
  return;
}
void ySet(short speed){
	motor[leftFront] = speed;
	motor[leftBack] = speed;
	motor[rightFront] = speed;
	motor[rightBack] = speed;
}
void xSet(short speed){
	motor[leftFront] = -speed;
	motor[leftBack] = speed;
	motor[rightFront] = speed;
	motor[rightBack] = -speed;
}
void turn(short speed){
	motor[leftFront] = speed;
	motor[leftBack] = speed;
	motor[rightFront] = -speed;
	motor[rightBack] = -speed;
}

task main(){
  initializeRobot();
  // wait for start of tele-op phase
  waitForStart();

  //actual control of the robot
  short zero = 10;
  short bls = 0;
  short brs = 255;
  short turnMod = 2; //the modifier that effects the turn speed
  bool tubeClamp = false;
  while (true){
		getJoystickSettings(joystick);
		//code for robot panning controlled by the left joystick
		if(joystick.joy1_y1 > zero){ //forwards
			ySet(joystick.joy1_y1);
		}else if(joystick.joy1_y1 < -zero){ //backwards
			ySet(joystick.joy1_y1);
		}else if(joystick.joy1_x1 < -zero){ //left
			xSet(joystick.joy1_x1/2);
		}else if(joystick.joy1_x1 > zero){ //right
			xSet(joystick.joy1_x1/2);
		}else{
			ySet(0);
			xSet(0);
		}
		//code for robot rotation and turning
		if(joystick.joy1_x2 < -zero){ //left turn
			turn(joystick.joy1_x2/turnMod);
		}else if(joystick.joy1_x2 > zero){ //right turn
			turn(joystick.joy1_x2/turnMod);
		}else{

		}
		//button actions
		switch(joystick.joy1_Buttons){
			case 1:
				if(tubeClamp)
					tubeClamp = false;
			  else
			  	tubeClamp = true;
				break;
			case 2:
				break;
			case 3:
				break;
			case 4:
				break;
			case 5:
				break;
			case 6:
				break;
			case 7:
				break;
			case 8:
				break;
			default:
				break;
		}

		if(joystick.joy1_TopHat != -1){
    	if(joystick.joy1_TopHat == 0){
    	}else if(joystick.joy1_TopHat == 1){
    	}else if(joystick.joy1_TopHat == 2){
    	}else if(joystick.joy1_TopHat == 3){
    	}else if(joystick.joy1_TopHat == 4){
    	}else if(joystick.joy1_TopHat == 5){
    	}else if(joystick.joy1_TopHat == 6){
    	}else if(joystick.joy1_TopHat == 7){
    	}
    }

    if(!tubeClamp){
    	bls = 0;
			brs = 255;
    }else{
    	bls = 120;
    	brs = 135;
    }
  	servo[backLeftServo] = bls;
    servo[backRightServo] = brs;
	}
}
