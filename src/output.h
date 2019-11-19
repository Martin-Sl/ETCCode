#pragma once
#include "mbed.h"
#include "constants.h"
//InputSection
//#define resultAngle = ((microsecs-ShortestPulse)/(LongestPulse-ShortestPulse))*ServoAngle
#define CALIBRATION
#define POTANGLEATIDLE 21.1f
#define POTANGLEATFULL 90.0f
#define SERVO_RANGE (POTANGLEATFULL-POTANGLEATIDLE)


class CustomServoOutput {
public:
	unsigned short longestPulse = 2100;
	unsigned short shortestPulse = 900;
	float servoTotalAngle = 135;
	#ifndef CALIBRATION
	float servoFullClosedAngle = 15;
	#else
	float servoFullClosedAngle = POTANGLEATIDLE;
	#endif
	float servoFullOpenAngle = servoFullClosedAngle + SERVO_RANGE;
	float servoAbsoluteAngle = servoFullClosedAngle;
	PwmOut* correspondingPwm;
	float conversionMultiplier = (longestPulse - shortestPulse) / servoTotalAngle;
	unsigned short usValue = shortestPulse;

	CustomServoOutput(PwmOut* outputPointer) {
		correspondingPwm = outputPointer;
		correspondingPwm->period_ms(20);
	};

	void setThrottleAngle(float angle);

	void setThrottlePercentage(float percentage);
};