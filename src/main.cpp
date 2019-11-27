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
    volatile float percentage = (float)servoSetAngle/(float)servoOut->throttleAngleRange;
    volatile float setPercentage = min(1.0f, max((float)(percentage), 0.0f));
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
    setAngleMatchesReal = (((setPercentage - realThrottlePercentage)) < AngleTolerance);
#endif
    int errorState = TimerErrors();
    if(errorState != 0){
      ETCShutdown(errorState);
      //volatile float appsSecondaryAngleDEB = appsSecondaryAngle;
      volatile float appsPrimaryAngleDEB = appsPrimaryAngle;
      // volatile float tpsUno = feedbackPrimaryAngle;
      // volatile float tpsDue = feedbackSecondaryAngle;
    }
    calibrationButtonChecker();
#else
    servoSetAngle = appsPrimaryAngle;
#endif

    
    servoOut->setThrottlePercentage((setPercentage));
  }
}

