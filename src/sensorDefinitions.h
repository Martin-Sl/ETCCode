#pragma once
#include "sensors.h"
#include "constants.h"
#include "output.h"

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

CustomServoOutput* servoOut;

