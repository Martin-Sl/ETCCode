
#include "output.h"

void CustomServoOutput::setThrottleAngle(float angle) {
	servoAbsoluteAngle = angle + servoFullClosedAngle;
	usValue = shortestPulse + servoAbsoluteAngle * conversionMultiplier;

	if (angle > SERVO_RANGE) {
		usValue = shortestPulse + servoFullOpenAngle * conversionMultiplier;

	}

	if (angle < 0) {
		usValue = shortestPulse + servoFullClosedAngle * conversionMultiplier;
	}

	correspondingPwm->pulsewidth_us(usValue);
}

void CustomServoOutput::setThrottlePercentage(float percentage){
	setThrottleAngle(percentage * ThrottleAngleRange);
}
