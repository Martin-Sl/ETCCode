#define AngleDebug
#include "sensors.h"

float Sensor::getCurrentAngle() {
	getAdcValue();
	if(invertReading){
		averageAngle = totalAngleRange - ((averageAdcValue - adcAtNoTurnAngleOffset) / unitsPerDegree) - startAngle;
	}
	else{
		averageAngle = ((averageAdcValue - adcAtNoTurnAngleOffset) / unitsPerDegree) - startAngle;
	}
	
#ifdef AngleDebug
	volatile float currentAngle = 0;
	if(invertReading){
		currentAngle = totalAngleRange - ((currentAdcValue - adcAtNoTurnAngleOffset) / unitsPerDegree) - startAngle;;
	}
	else{
		currentAngle = ((currentAdcValue - adcAtNoTurnAngleOffset) / unitsPerDegree) - startAngle;
	}
	
#endif
	actuationPercentage = averageAngle/usableAngleRange;
	return averageAngle;
}

unsigned short Sensor::getAdcValue() {
	currentAdcValue = myInputPin->read_u16();
	averageAdcValue = (alpha * currentAdcValue) + (1.0 - alpha) * averageAdcValue;

	return currentAdcValue;
}

bool Sensor::isInRange(){
	if(averageAngle > 0 && averageAngle > usableAngleRange){
		return true;
	}
	return false;
}

