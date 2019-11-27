#pragma once
#include "buttonInput.h"
#include "constants.h"
#include "sensorDefinitions.h"
#include "mbed.h"
#include "output.h"

const float throttleStep = 0.1f;
const int measurementsForAverage = 32;
float lastAngle = 0;
const float stepSafety = 2;

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
    }

}
 
bool ChangeFromLastStep(float angleCriterium){
	float currentAngle = feedbackPrimary.getAngleWithoutNoise(measurementsForAverage);
	float percetRef = feedbackPrimary.totalAngleRange;
	if( abs(lastAngle - currentAngle) < angleCriterium){
		return false;
	}
	return true;
}

void MoveUntilEndOfRange (){
	
	lastAngle = feedbackPrimary.getAngleWithoutNoise(measurementsForAverage);
	//Steps: gonna be 1 degree == 1/135
	const float stepAngle = 1;
	float criterium = stepAngle / servoOut->servoTotalAngle * stepSafety;

	while(ChangeFromLastStep(criterium)){
		
		wait_ms(10);

	}
}
#endif
