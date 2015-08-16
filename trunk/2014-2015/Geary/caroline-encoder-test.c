#pragma config(Hubs,  S1, HTMotor,  HTMotor,  none,     none)
#pragma config(Sensor, S1,     ,               sensorI2CMuxController)
#pragma config(Sensor, S2,     sonar2,         sensorSONAR)
#pragma config(Sensor, S3,     HTSPB,          sensorI2CCustom9V)
#pragma config(Sensor, S4,     sonar4,         sensorSONAR)
#pragma config(Motor,  mtr_S1_C1_1,     left,          tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C1_2,     right,         tmotorTetrix, PIDControl, encoder)
#pragma config(Motor,  mtr_S1_C2_1,     hopper,        tmotorTetrix, openLoop)
#pragma config(Motor,  mtr_S1_C2_2,     fingers,       tmotorTetrix, PIDControl, reversed, encoder)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

//                  TETRIX nMotorEncoderTarget Example
//
// The program below demonstrates how to use the nMotorEndocerTarget function
// There are four steps to using the function
//  Step 1 - Zero out the encoder using "nMotorEncoder(motorX) = 0"   Lines 27 and 28.
//  Step 2 - Set the target distance using the nMotorEncoderTarget function.
//           Lines 30 and 31.
//  Step 3 - Turn the motors on. Lines 33 and 34.
//  Step 4 - Wait for the motor's "runstate" to reach idle. Line 34 through 38.
//
//  nMotorTargetEncoder is coupled with the nMotorRunState function.
//    The nMotorRunState function watches the "state" of the motor in three conditions:
//       1. "runStateRunning" - While the motor is running
//       2. "runStateHoldPosition" - While the motor is begining to slow down.
//          nMotorEncoderTarget sets the condition when the motor begins to slow down.
//       3. "runStateIdle" - Reports back to the controller that the motor is in an
//           idle position; this is when the motor is stopped.


task main()
{
	int testMotor = left;

  nMotorEncoder[testMotor] = 0;  //clear the TETRIX motor encoders
  nMotorEncoder[hopper] = 0;  //clear the TETRIX motor encoders

  nMotorEncoderTarget[testMotor] = 3000; //set the target stoping position
  motor[testMotor] = 30; //turn both motors on at 30 percent power
  while (nMotorRunState[testMotor] != runStateIdle) //while the encoder wheel turns one revolution
  {
  	int encoderVal = nMotorEncoder[left] + nMotorEncoder[right] + nMotorEncoder[hopper] + nMotorEncoder[fingers];
	  nxtDisplayCenteredBigTextLine(1, "%4d", nMotorEncoder[encoderVal]);
	  wait1Msec(200);
  }
  motor[testMotor] = 0; //turn both motors off

  nMotorEncoder[testMotor] = 0;  //clear the TETRIX motor encoders
  nMotorEncoderTarget[testMotor] = -3000; //set the target stoping position
  motor[testMotor] = -30; //turn both motors on at 30 percent power
  while (nMotorRunState[testMotor] != runStateIdle) //while the encoder wheel turns one revolution
  {
  	int encoderVal = nMotorEncoder[left] + nMotorEncoder[right] + nMotorEncoder[hopper] + nMotorEncoder[fingers];
	  nxtDisplayCenteredBigTextLine(1, "%4d", nMotorEncoder[encoderVal]);
	  wait1Msec(200);
  }
  motor[testMotor] = 0; //turn both motors off



  wait1Msec(3000);  // wait 3 seconds to see feedback on the debugger screens
                    // open the "NXT Devices" window to see the distance the encoder spins.
                    // the robot will come to a stop at the nMotorEncoderTarget position.
}
