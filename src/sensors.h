#pragma once
#include "mbed.h"
#include "constants.h"

class Sensor {
public:
	//Angle range to maximum
	float totalAngleRange = 180;
	//Reasonable values
	float usableAngleRange = 80;
	//Zero point of the sensor
	float startAngle = 0;
	//adc points per degree
	float unitsPerDegree = ADCResolution / totalAngleRange;

	float adcUsableRange = 1;

	float adcAtNoTurn = 0;

	unsigned short adcAtNoTurnAngleOffset = 0;

	bool invertReading = false;

	float averageAngle = 0;

	float actuationPercentage = 0;
	
	unsigned short currentAdcValue = 0;

	unsigned short averageAdcValue = 0;
	float alpha = 0.1;

	AnalogIn* myInputPin;
	
	Sensor(AnalogIn* myAnalogIn, int pinName) {
		myInputPin = myAnalogIn;
		switch (pinName)
		{
		case ThrottleHALLEffect: {
			totalAngleRange = 360;
			usableAngleRange = 80;
			startAngle = 90;
			unitsPerDegree = ADCResolution / totalAngleRange;
			break;
		}
		case ThrottlePotentiometer: {
			totalAngleRange = 300;
			usableAngleRange = 80;
			startAngle = 30;
			unitsPerDegree = ADCResolution / totalAngleRange;
			break;
		}
		case APPSOne: {
			totalAngleRange = 225;
			usableAngleRange = 80;
			startAngle = 0;
			adcUsableRange = 1;
			adcAtNoTurn = 0;
			adcAtNoTurnAngleOffset = adcAtNoTurn * ADCResolution;
			unitsPerDegree = (ADCResolution*adcUsableRange) / totalAngleRange;
			break;
		}
		case APPSTwo: {
			totalAngleRange = 360;
			usableAngleRange = 80;
			startAngle = 0;
			invertReading = true;
			adcUsableRange = 0.8;
			adcAtNoTurn = 0.10;
			unitsPerDegree = (ADCResolution*adcUsableRange) / totalAngleRange;
			adcAtNoTurnAngleOffset = adcAtNoTurn * ADCResolution;
			break;
		}
		default:
			break;
		}
	};

	float getCurrentAngle();

	unsigned short getAdcValue();

	bool isInRange();
};
