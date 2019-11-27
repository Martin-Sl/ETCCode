#pragma once
#include "sensors.h"
#include "constants.h"


float realThrottlePercentage = 0;
#ifdef FeedbackSupport
#ifndef SingleAPPSTPSDebug
bool implausabilityAPPSTimerRunning = false;
Timer implausabilityAPPSTimer;
bool appsPlausible = true;
#endif
bool errorWithFeedbackSystem = false;
#ifndef OnlyDoubleAPPSSensor
bool servoDelayTimerRunning = false;
Timer servoDelayTimer;
bool implausabilityTPSTimerRunning = false;
Timer implausabilityTPSTimer;
bool tpsPlausible = true;
bool setAngleMatchesReal = true;
#endif

bool CheckSensorPlausability(Sensor* sensor1, Sensor* sensor2){
   //DifferenceChecking
   volatile float difference = abs((sensor1->actuationPercentage)-(sensor2->actuationPercentage));
   if(difference > AngleTolerance){
     return false;
   }
   //RangeChecking
   float sensor1Percentage = sensor1->actuationPercentage;
   float sensor2Percentage = sensor2->actuationPercentage;
   if(sensor1Percentage < -AngleTolerance || sensor1Percentage > 1+AngleTolerance || sensor2Percentage < -AngleTolerance || sensor2Percentage > 1+AngleTolerance){
     return false;
   }
   return true;
}

bool EntryIsPlausible(bool plausability, Timer* timerPointer, bool& timerRunning, unsigned short errorMs){
  if(!plausability){
    if(timerRunning){
      #ifdef DebugCodeSnippets
      volatile int passedTime = timerPointer->read_ms();
      #endif
      if(passedTime > errorMs){
        if(FailiureLEDBlink){
          timerRunning = false;
          timerPointer->stop();
          timerPointer->reset();
        }
        return false;
      } 
    }
    else {
      timerRunning = true;
      timerPointer->start();
    }
  }
  else if (timerRunning){
    timerRunning = false;
    timerPointer->stop();
    timerPointer->reset();
  }
  return true;
}

int TimerErrors(){
  #ifndef SingleAPPSTPSDebug
  bool appsFine = EntryIsPlausible(appsPlausible, &implausabilityAPPSTimer, implausabilityAPPSTimerRunning, ImplausabilityIntervalMS);
  #endif
  bool tpsFine = true;
  bool servoFine = true;
  #ifndef OnlyDoubleAPPSSensor
  tpsFine = EntryIsPlausible(tpsPlausible, &implausabilityTPSTimer, implausabilityTPSTimerRunning, ImplausabilityIntervalMS);
  servoFine = EntryIsPlausible(setAngleMatchesReal, &servoDelayTimer, servoDelayTimerRunning, PositionMustBeReachedIn);
  #endif
  #ifndef SingleAPPSTPSDebug
  if(!(appsFine && servoFine)){
    return APPSFailure;
  }
  #else
  if(!(servoFine)){
    return APPSFailure;
  }
  #endif
  if(!tpsFine)
  {
    return TPSFailiure;
  }
  return 0;
}
#endif
