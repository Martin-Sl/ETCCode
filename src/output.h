#pragma once
#include "mbed.h"
#include "constants.h"
//InputSection
//#define resultAngle = ((microsecs-ShortestPulse)/(LongestPulse-ShortestPulse))*ServoAngle

class CustomServoOutput {
public:
	unsigned short longestPulse = 2100;
	unsigned short shortestPulse = 900;
	float servoTotalAngle = 135;
	float throttleAngleRange = ThrottleAngleRange;
	float servoFullClosedAngle = 10;
	float servoFullOpenAngle = servoFullClosedAngle + throttleAngleRange;
	float servoAbsoluteAngle = servoFullClosedAngle;
	PwmOut* correspondingPwm;
	float conversionMultiplier = (longestPulse - shortestPulse) / servoTotalAngle;
	unsigned short usValue = shortestPulse;

	CustomServoOutput(PwmOut* outputPointer) {
		correspondingPwm = outputPointer;
		correspondingPwm->period_ms(20);
	};

	void setThrottleAngle(float angle);
};