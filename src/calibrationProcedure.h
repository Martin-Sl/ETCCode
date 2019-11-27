#pragma once
#include "buttonInput.h"
#include "constants.h"
#include "sensorDefinitions.h"
#include "mbed.h"
#include "output.h"

const float throttleStep = 0.1f;
const int measurementsForAverage = 32;
float lastAngle = 0;
const float stepSafety = 1.5f;

DebounceIn buttonPin(CalibrationButton);
ButtonInput calibrationButton(&buttonPin);

#ifdef FeedbackSupport

void calibrationButtonChecker(){
    calibrationButton.checkPresses();
    if(calibrationButton.wasButtonTriplePressed()){
        //Started to think this wast't a good idea
#ifdef DebugCodeSnippets
        volatile float checkTheCode = 5;
#endif
		calibrateBottom();

    }

}
 
float averageAngleFromTPS(){
	float currentAnglePrim = feedbackPrimary.getAngleWithoutNoise(measurementsForAverage);
	float currentAngleSec = feedbackSecondary.getAngleWithoutNoise(measurementsForAverage);
	return (currentAnglePrim+currentAngleSec)/2;
}

bool ChangeFromLastStep(float angleCriterium){
	float currentAngle = averageAngleFromTPS();
	//float percetRef = feedbackPrimary.totalAngleRange;
	if( abs(lastAngle - currentAngle) < angleCriterium){
		return false;
	}
	return true;
}

void MoveUntilEndOfRange (float step){
	
	lastAngle = feedbackPrimary.getAngleWithoutNoise(measurementsForAverage);
	//Steps: gonna be 1 degree == 1/135

	float criterium = step / servoOut->servoTotalAngle * stepSafety;

	while(ChangeFromLastStep(criterium)){

		wait_ms(10);

	}
}

void calibrateBottom(){
	//Move with step -1
	float step = -1;
	MoveUntilEndOfRange(step);
	//Reached the end
	const int endMeasurements = 64;
	float primaryFeedbackAngle = feedbackPrimary.getAngleWithoutNoise(endMeasurements);
	float secondaryFeedbackAngle = feedbackSecondary.getAngleWithoutNoise(endMeasurements);
	//This is a bit too far, for accuracy this should be half the step
	feedbackPrimary.startAngle -=  primaryFeedbackAngle - step/2;
	feedbackSecondary.startAngle -=  secondaryFeedbackAngle - step/2;
	servoOut->throttleIdleAngle = servoOut->servoAbsoluteAngle;

	//Assuming APPS ARE ACCURATE AND WE KNOW THE angle from bottom to fully open
#ifdef OnlyIdleHoming
	servoOut->throttleFullOpenAngle = servoOut->throttleIdleAngle + servoOut->throttleAngleRange;
#endif

}
#endif
