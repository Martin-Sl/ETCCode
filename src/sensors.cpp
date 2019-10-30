
#include "sensors.h"

float Sensor::getCurrentAngle() {
	getAdcValue();
	currentAngle = (currentAdcValue / unitsPerDegree) - startAngle;
	actuationPercentage = currentAngle/usableAngleRange;
	return currentAngle;
}

unsigned short Sensor::getAdcValue() {
	currentAdcValue = myInputPin->read_u16();
	return currentAdcValue;
}

bool Sensor::isInRange(){
	if(currentAngle > 0 && currentAngle > usableAngleRange){
		return true;
	}
	return false;
}

