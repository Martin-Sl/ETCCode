#include <mbed.h>
#include <math.h>
#include "constants.h"
#include "sensors.h"
#include "output.h"
#include "throttleMapInteraction.h"
#define FeedbackSupport
//#define OnlyDoubleAPPSSensor
#define SingleAPPSTPSDebug
#define FailiureLEDBlink true

bool engineRunning = true;
bool etcPower = true; 
float servoSetAngle = 0;

#ifdef FeedbackSupport
#ifndef SingleAPPSTPSDebug
bool implausabilityAPPSTimerRunning = false;
Timer implausabilityAPPSTimer;
bool appsPlausible = true;
#endif
bool errorWithFeedbackSystem = false;
#ifndef OnlyDoubleAPPSSensor
float realThrottlePercentage = 0;
bool servoDelayTimerRunning = false;
Timer servoDelayTimer;
bool implausabilityTPSTimerRunning = false;
Timer implausabilityTPSTimer;
bool tpsPlausible = true;
bool setAngleMatchesReal = true;
#endif
DigitalOut led(LED2);

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
    etcPower = false;
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
    }
    else{
      LEDBlink(0.2);
    } 
  }
}

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
      if(timerPointer->read_ms() > errorMs){
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

int main() {


  AnalogIn appsPrimaryAnalog(APPSOne);
  Sensor appsPrimary(&appsPrimaryAnalog, APPSOne);
 
#ifdef FeedbackSupport 
#ifndef SingleAPPSTPSDebug
  AnalogIn appsSecondaryAnalog(APPSTwo);
  Sensor appsSecondary(&appsSecondaryAnalog, APPSTwo);
#endif
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
#ifndef SingleAPPSTPSDebug
    float appsSecondaryAngle = appsSecondary.getCurrentAngle();
    servoSetAngle = (appsPrimaryAngle + appsSecondaryAngle)/2;
#else
    float servoSetAngle = appsPrimaryAngle;
#endif
#ifndef OnlyDoubleAPPSSensor
    float feedbackPrimaryAngle   = feedbackPrimary.getCurrentAngle();
    float feedbackSecondaryAngle = feedbackSecondary.getCurrentAngle();
    realThrottlePercentage = ((feedbackPrimaryAngle+feedbackSecondaryAngle)/2)/SERVO_RANGE;
    
#endif
#ifndef SingleAPPSTPSDebug
    appsPlausible = CheckSensorPlausability(&appsPrimary, &appsSecondary);
#endif
#ifndef OnlyDoubleAPPSSensor
    tpsPlausible = CheckSensorPlausability(&feedbackPrimary, &feedbackSecondary);
    setAngleMatchesReal = (((servoSetAngle - realThrottlePercentage*SERVO_RANGE)/ThrottleAngleRange) < AngleTolerance);
#endif
    int errorState = TimerErrors();
    if(errorState != 0){
      ETCShutdown(errorState);
      //volatile float appsSecondaryAngleDEB = appsSecondaryAngle;
      volatile float appsPrimaryAngleDEB = appsPrimaryAngle;
      volatile float tpsUno = feedbackPrimaryAngle;
      volatile float tpsDue = feedbackSecondaryAngle;
    }
#else
    

#endif
    float setPercentage = min(0.0f, max((float)(servoSetAngle/SERVO_RANGE), 1.0f));
    servoOut.setThrottlePercentage(interpolatedThrottle(setPercentage));
  }

}

