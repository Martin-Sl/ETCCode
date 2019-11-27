#define AngleDebug
#include "sensors.h"

float Sensor::getCurrentAngle() {
	getAdcValue();
	averageAngle = convertAdcValueToAngle(averageAdcValue);
#ifdef AngleDebug
	volatile float currentAngle = convertAdcValueToAngle(currentAdcValue);
#endif
	actuationPercentage = averageAngle/usableAngleRange;
	return averageAngle;
}

float Sensor::getAngleWtihoutAveraging(){
	currentAdcValue = myInputPin->read_u16();
	convertAdcValueToAngle(currentAdcValue);
	return currentAbsoluteAngle - startAngle;
}

float Sensor::convertAdcValueToAngle(float adcValue){
	if(invertReading){
		currentAbsoluteAngle = totalAngleRange - ((adcValue - adcAtNoTurnAngleOffset) / unitsPerDegree);
	}
	else{
		currentAbsoluteAngle = ((adcValue - adcAtNoTurnAngleOffset) / unitsPerDegree);
	}
	return currentAbsoluteAngle - startAngle;
}

unsigned short Sensor::getAdcValue() {
	currentAdcValue = myInputPin->read_u16();
	#ifdef ADCAveraging
	averageAdcValue = (alpha * currentAdcValue) + (1.0 - alpha) * averageAdcValue;
	#else
	averageAdcValue = currentAdcValue;
	#endif
	return currentAdcValue;
}

bool Sensor::isInRange(){
	if(averageAngle > 0 && averageAngle > usableAngleRange){
		return true;
	}
	return false;
}

float Sensor::getAbsoluteCalibrationAngle(){
	//delay for the average to stabilize
	float averageAngle = getAngleWithoutNoise(512);	
	return averageAngle;
}

void Sensor::calibrateZero(){
	float foundAngle = getAbsoluteCalibrationAngle();
	startAngle = foundAngle;
}

void Sensor::calibrateFull(){
	float foundAngle = getAbsoluteCalibrationAngle();
	usableAngleRange = abs(foundAngle - startAngle);
}

float Sensor::getAngleWithoutNoise(int measurements){
	float angleWithoutNoise = getAngleWtihoutAveraging();
	for (int i = measurements; i > 0; i--)
	{
		angleWithoutNoise =  (alpha * getAngleWtihoutAveraging()) + (1.0 - alpha) * angleWithoutNoise;
	}
}
