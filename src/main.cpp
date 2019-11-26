#include <mbed.h>
#include <math.h>
#include "constants.h"
#include "sensorDefinitions.h"
#include "sensors.h"
#include "output.h"
#include "throttleMapInteraction.h"
#include "plausability.h"
#include "shutdown.h"
#include "buttonInput.h"
#include "calibrationProcedure.h"

float servoSetAngle = 0;

int main() {
// AnalogIn appsPrimaryAnalog(APPSOne);
// Sensor appsPrimary(&appsPrimaryAnalog, APPSOne);
/*
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
*/
	//PwmOut pwmOutput(PA_6);
	//CustomServoOutput servoOut(&pwmOutput);
	PwmOut servoPWM(PA_6);
	CustomServoOutput servoOutInstance(&servoPWM);
	servoOut = &servoOutInstance;
	

  while(1) {
    // put your main code here, to run repeatedly:
    float appsPrimaryAngle = appsPrimary.getCurrentAngle();

#ifdef FeedbackSupport
#ifndef SingleAPPSTPSDebug
    float appsSecondaryAngle = appsSecondary.getCurrentAngle();
    servoSetAngle = (appsPrimaryAngle + appsSecondaryAngle)/2;
#else
    servoSetAngle = appsPrimaryAngle;
#endif
#ifndef OnlyDoubleAPPSSensor
    float feedbackPrimaryAngle   = feedbackPrimary.getCurrentAngle();
    float feedbackSecondaryAngle = feedbackSecondary.getCurrentAngle();
    realThrottlePercentage = ((feedbackPrimaryAngle+feedbackSecondaryAngle)/2)/servoOut->servoTotalAngle;
#endif
#ifndef SingleAPPSTPSDebug
    appsPlausible = CheckSensorPlausability(&appsPrimary, &appsSecondary);
#endif
#ifndef OnlyDoubleAPPSSensor
    tpsPlausible = CheckSensorPlausability(&feedbackPrimary, &feedbackSecondary);
    setAngleMatchesReal = (((servoSetAngle - realThrottlePercentage*servoOut->servoTotalAngle)/servoOut->throttleAngleRange) < AngleTolerance);
#endif
    int errorState = TimerErrors();
    if(errorState != 0){
      ETCShutdown(errorState);
      //volatile float appsSecondaryAngleDEB = appsSecondaryAngle;
      volatile float appsPrimaryAngleDEB = appsPrimaryAngle;
      // volatile float tpsUno = feedbackPrimaryAngle;
      // volatile float tpsDue = feedbackSecondaryAngle;
    }
#else
    servoSetAngle = appsPrimaryAngle;
#endif
    calibrationButtonChecker();
    float setPercentage = min(0.0f, max((float)(servoSetAngle/servoOut->throttleAngleRange), 1.0f));
    //servoOut.setThrottlePercentage(interpolatedThrottle(setPercentage));
  }
}

