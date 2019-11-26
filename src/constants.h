#pragma once
#include <mbed.h>

#define ImplausabilityIntervalMS 50
#define PositionMustBeReachedIn 500
#define AngleTolerance (0.1)
#define ADCResolution 65536
#define ThrottleHALLEffect PA_4 
#define ThrottlePotentiometer PB_0
#define APPSOne PA_0
#define APPSTwo PA_1
#define CalibrationButton PC_13
#define TPSFailiure 2
#define APPSFailure 1
#define SERVOFailiure 1
#define FailiureLEDBlink true
#define FeedbackSupport
#define OnlyDoubleAPPSSensor
//#define SingleAPPSTPSDebug

#define SensorAndTPSCalibration

#define DebugCodeSnippets