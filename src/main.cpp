#include <mbed.h>
#include "constants.h"
#include "sensors.h"
#include "output.h"
#define FeedbackSupport
#define OnlyDoubleAPPSSensor


#ifdef FeedbackSupport

bool implausabilityAPPSTimerRunning = false;
Timer implausabilityAPPSTimer;
bool appsPlausible = true;
bool errorWithFeedbackSystem = false;
#ifndef OnlyDoubleAPPSSensor
bool servoDelayTimerRunning = false;
Timer servoDelayTimer;
bool implausabilityTPSTimerRunning = false;
Timer implausabilityTPSTimer;
bool tpsPlausible = true;
bool setAngleMatchesReal = true;
#endif
DigitalOut led(LED2);

void ErrorHandling(){
  led.write(1);
  wait(0.1);
  led.write(0);
}

bool CheckSensorPlausability(Sensor* sensor1, Sensor* sensor2){
   //DifferenceChecking
   volatile float difference = abs((sensor1->actuationPercentage)-(sensor2->actuationPercentage));
   if(difference > AngleTolerance){
     return false;
   }
   //RangeChecking
   float apps1Percentage = sensor1->actuationPercentage;
   float apps2Percentage = sensor2->actuationPercentage;
   if(apps1Percentage < -AngleTolerance || apps1Percentage > 1+AngleTolerance || apps2Percentage < -AngleTolerance || apps2Percentage > 1+AngleTolerance){
     return false;
   }
   return true;
}

bool EntryIsPlausible(bool plausability, Timer* timerPointer, bool& timerRunning, unsigned short errorMs){
  if(!plausability){
    if(timerRunning){
      if(timerPointer->read_ms() > errorMs){
        timerRunning = false;
        timerPointer->stop();
        timerPointer->reset();
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

bool TimerErrors(){
  bool appsFine = EntryIsPlausible(appsPlausible, &implausabilityAPPSTimer, implausabilityAPPSTimerRunning, ImplausabilityIntervalMS);
  bool tpsFine = true;
  bool servoFine = true;
  #ifndef OnlyDoubleAPPSSensor
  tpsFine = EntryIsPlausible(tpsPlausible, &implausabilityTPSTimer, implausabilityTPSTimerRunning, ImplausabilityIntervalMS);
  servoFine = EntryIsPlausible(setAngleMatchesReal, &servoDelayTimer, servoDelayTimerRunning, PositionMustBeReachedIn);
  #endif
  return !(appsFine&&tpsFine&&servoFine);
 
}
#endif

int main() {


  AnalogIn appsPrimaryAnalog(APPSOne);
  Sensor appsPrimary(&appsPrimaryAnalog, APPSOne);
 
#ifdef FeedbackSupport 
  
  AnalogIn appsSecondaryAnalog(APPSTwo);
  Sensor appsSecondary(&appsSecondaryAnalog, APPSTwo);

#ifndef OnlyDoubleAPPSSensor
  AnalogIn feedbackPrimaryAnalog(ThrottlePotentiometer);
  Sensor feedbackPrimary(&feedbackPrimaryAnalog, ThrottlePotentiometer);

  AnalogIn feedbackSecondaryAnalog(ThrottleHALLEffect);
  Sensor feedbackSecondary(&feedbackSecondaryAnalog, ThrottleHALLEffect);
#endif
#endif

  PwmOut pwmOutput(PA_6);
  CustomServoOutput servoOut(&pwmOutput);

  while(1) {
    // put your main code here, to run repeatedly:
    float appsPrimaryAngle = appsPrimary.getCurrentAngle();

#ifdef FeedbackSupport
    float appsSecondaryAngle = appsSecondary.getCurrentAngle();
    float servoSetAngle = (appsPrimaryAngle + appsSecondaryAngle)/2;
#ifndef OnlyDoubleAPPSSensor
    float servoRealAngle = (feedbackPrimary.getCurrentAngle()+feedbackSecondary.getCurrentAngle())/2;
#endif
    appsPlausible = CheckSensorPlausability(&appsPrimary, &appsSecondary);
#ifndef OnlyDoubleAPPSSensor
    tpsPlausible = CheckSensorPlausability(&appsPrimary, &appsSecondary);
    setAngleMatchesReal = (((servoSetAngle - servoRealAngle)/ThrottleAngleRange) < AngleTolerance);
#endif
    if(TimerErrors()){

      ErrorHandling();
      volatile float appsSecondaryAngleDEB = appsSecondaryAngle;
      volatile float appsPrimaryAngleDEB = appsPrimaryAngle;
    }
#else
    float servoSetAngle = appsPrimaryAngle;

#endif
    servoOut.setThrottleAngle(servoSetAngle);
  
  }

}

