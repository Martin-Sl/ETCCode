#pragma once
#include <mbed.h>
#include "DebounceIn.h"

class ButtonInput{
    public:
    Timer buttonTimeOutTimer;
    bool timerRunning = false;
    DebounceIn* debouncedInput;
    int pressCounter = 0;
    int lastState = 0;
    const int overflowS = 1;

    void startTimer(){
        buttonTimeOutTimer.start();
        timerRunning = true;
    }
    void stopAndResetTimer(){
        buttonTimeOutTimer.stop();
        buttonTimeOutTimer.reset();
        timerRunning = false;
    }

    void checkPresses(){
        
        int readValue = debouncedInput->read();
        if(lastState != readValue){
            if(!timerRunning){
                startTimer();
            }
            pressCounter++;
            lastState = readValue; 
        }
        else{
            if(timerRunning && buttonTimeOutTimer.read() > overflowS){
                stopAndResetTimer();
                pressCounter = 0;
            }
        }
    }
   


    ButtonInput(DebounceIn* myDigitalIn) {
        debouncedInput = myDigitalIn;
	};

    bool wasButtonPressed(){
        if(pressCounter > 1){
            return true;
        }
        return false;
    };

    bool wasButtonDoublePressed(){
        if(pressCounter > 3){
            return true;
        }
         return false;
    };

    bool wasButtonTriplePressed(){
        if(pressCounter > 5){
            return true;
        }
         return false;
    };
};


