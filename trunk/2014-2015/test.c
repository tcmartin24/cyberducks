#pragma config(Sensor, S4,     GYRO,           sensorI2CHiTechnicGyro)
/*!!Code automatically generated by 'ROBOTC' configuration wizard

int rotation(int spins){
	nMotorEncoder[motorA] = 0;
	nMotorEncoder[motorB] = 0;
	int degrees = (spins * 360);

	while(nMotorEncoder[motorA] < degrees && nMotorEncoder[motorB] < degrees){

			motor[motorA] = 50;
		  motor[motorB] = 50;
		}
	motor[motorA] = 0;
	motor[motorB] = 0;
	return true;
}
*/
task main(){

}
