#include "throttleMapInteraction.h"
#include "throttlemap.h"

float engineRpm = 6000;
float maxRPM = 16000;

int selectedMapNumber = 0;

int APPSPoints = 20;
int RpmPoints = 12;

int mapIndexFromAppsPercentage(float percentage){
  return (int)(percentage*APPSPoints);
}

int mapIndexFromRpm(float engineRpm){
    return (int)((engineRpm/maxRPM)*RpmPoints);
}

float interpolatedThrottle(float requestedPercentage){
    int lowerRPMIndex = mapIndexFromRpm(engineRpm);
    int lowerAPPSIndex = mapIndexFromAppsPercentage(requestedPercentage);

    float valueOne = thrMapArray[selectedMapNumber][lowerRPMIndex][lowerAPPSIndex];
    float valueTwo = thrMapArray[selectedMapNumber][lowerRPMIndex + 1][lowerAPPSIndex];
    float valueThree = thrMapArray[selectedMapNumber][lowerRPMIndex][lowerAPPSIndex + 1];
    float valueFour = thrMapArray[selectedMapNumber][lowerRPMIndex + 1][lowerAPPSIndex + 1];

    float rpmMu = (float)(engineRpm - (maxRPM / RpmPoints) * lowerRPMIndex) / (maxRPM / RpmPoints);
    float throttleMu = (requestedPercentage-(lowerAPPSIndex/APPSPoints)) * (1.0 / APPSPoints);

    float rpmBasedOne = (valueOne * (1.0 - rpmMu) + valueTwo * rpmMu);
    float rpmBasedTwo = (valueThree * (1.0 - rpmMu) + valueFour * rpmMu);
    float appsInterp = (rpmBasedTwo * (1.0 - throttleMu) + rpmBasedOne * throttleMu);
    return appsInterp;
}