#pragma once
#include "buttonInput.h"
#include "constants.h"

DebounceIn buttonPin(CalibrationButton);
ButtonInput calibrationButton(&buttonPin);

void calibrationChecker(){
    calibrationButton.checkPresses();
    if(calibrationButton.wasButtonTriplePressed()){
        //Started to think this wast't a good idea
#ifdef DebugCodeSnippets
        volatile float checkTheCode = 5;
#endif
    }
}