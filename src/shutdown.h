#pragma once

#include <mbed.h>
#include "constants.h"
#include "plausability.h"

DigitalOut led(LED2);
bool engineRunning = true;
bool etcPower = true; 

void LEDBlink(float seconds){
  if(FailiureLEDBlink){
    led.write(1);
    wait(seconds);
    led.write(0);
  }
}

void ETCShutdown(int failiureStatus){
  if(etcPower){
    //ETC POWER SHUTDOWN CODE GOES HERE
    LEDBlink(0.5);
    etcPower = false;
  }
  else{
    //STILL SHUT DOWN FROM PREVIOUS ERROR
    LEDBlink(0.05);
  }
  //Is TPS Data TRUSTWORTHY
  if(failiureStatus != TPSFailiure){
    #ifndef OnlyDoubleAPPSSensor
    if(realThrottlePercentage > AngleTolerance){
      failiureStatus = TPSFailiure;
    }
    #endif
  }
  if(failiureStatus == TPSFailiure){
    if(engineRunning){
      //ENGINE SHUTDOWN CODE GOES HERE
      LEDBlink(2);
      engineRunning = false;
    }
    else{
      LEDBlink(0.2);
    } 
  }
}