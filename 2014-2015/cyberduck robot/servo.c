#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Hubs,  S2, HTServo,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     ,               sensorI2CMuxController)
#pragma config(Motor,  mtr_S1_C1_1,     rightFront,    tmotorTetrix, PIDControl, reversed)
#pragma config(Motor,  mtr_S1_C1_2,     rightBack,     tmotorTetrix, PIDControl, reversed)
#pragma config(Motor,  mtr_S1_C2_1,     leftBack,      tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S1_C2_2,     leftFront,     tmotorTetrix, PIDControl)
#pragma config(Motor,  mtr_S2_C2_1,     beaterBar,     tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S2_C2_2,     lift,          tmotorTetrix, openLoop, encoder)
#pragma config(Servo,  srvo_S2_C1_1,    backRightServo,       tServoStandard)
#pragma config(Servo,  srvo_S2_C1_2,    backLeftServo,        tServoStandard)
#pragma config(Servo,  srvo_S2_C1_3,    rightServo,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_4,    leftServo,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_5,    servo5,               tServoNone)
#pragma config(Servo,  srvo_S2_C1_6,    servo6,               tServoNone)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

task main()
{
	servo[servo3] = 255;


}
