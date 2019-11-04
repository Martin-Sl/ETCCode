
#include "sensors.h"

float Sensor::getCurrentAngle() {
	getAdcValue();
	currentAngle = (averageAdcValue / unitsPerDegree) - startAngle;
	actuationPercentage = currentAngle/usableAngleRange;
	return currentAngle;
}

unsigned short Sensor::getAdcValue() {
	currentAdcValue = myInputPin->read_u16();
	averageAdcValue = (alpha * currentAdcValue) + (1.0 - alpha) * averageAdcValue;

	return currentAdcValue;
}

bool Sensor::isInRange(){
	if(currentAngle > 0 && currentAngle > usableAngleRange){
		return true;
	}
	return false;
}

