
#include "output.h"

void CustomServoOutput::setThrottleAngle(float angle) {
	servoAbsoluteAngle = angle + throttleIdleAngle;
	usValue = shortestPulse + servoAbsoluteAngle * conversionMultiplier;
//TODO: This isn't always necessary
	if (angle > throttleAngleRange) {
		usValue = shortestPulse + throttleFullOpenAngle * conversionMultiplier;

	}
	if (angle < 0) {
		usValue = shortestPulse + throttleIdleAngle * conversionMultiplier;
	}
//ENDTODO

	correspondingPwm->pulsewidth_us(usValue);
}

void CustomServoOutput::setThrottlePercentage(float percentage){
	setThrottleAngle(percentage * throttleAngleRange);
}


void CustomServoOutput::setMinimalPosition(){

}

void CustomServoOutput::setMaximalPosition(){

}

void CustomServoOutput::setPositions(float minAngle, float angleRange){

}