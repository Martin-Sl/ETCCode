#pragma once
#include "mbed.h"
#include "constants.h"
//InputSection
//#define resultAngle = ((microsecs-ShortestPulse)/(LongestPulse-ShortestPulse))*ServoAngle





class CustomServoOutput {
public:
	// unsigned short longestPulse = 2100;
	// unsigned short shortestPulse = 900;

	unsigned short longestPulse = 2000;
	unsigned short shortestPulse = 1000;
	//float servoTotalAngle = 135;
	
	//float servoTotalAngle = 135;
	float servoTotalAngle = 77;
	float throttleAngleRange = 77;
	float throttleIdleAngle = 0;
	float throttleFullOpenAngle = throttleIdleAngle + throttleAngleRange;
	float servoAbsoluteAngle = 0;
	PwmOut* correspondingPwm;
	float conversionMultiplier = (longestPulse - shortestPulse) / servoTotalAngle;
	unsigned short usValue = shortestPulse;

	CustomServoOutput(PwmOut* outputPointer) {
		correspondingPwm = outputPointer;
		correspondingPwm->period_ms(20);
	};

	void setThrottleAngle(float angle);

	void setThrottlePercentage(float percentage);

	void uncheckedStep(float deltaAngle);

	void setPositions(float minAngle, float angleRange);
};


