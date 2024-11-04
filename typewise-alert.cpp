#include "typewise-alert.h"
#include <stdio.h>

// Define temperature limits for each cooling type
typedef struct {
    int lowerLimit;
    int upperLimit;
} CoolingLimits;

CoolingLimits getCoolingLimits(CoolingType coolingType) {
    switch(coolingType) {
        case PASSIVE_COOLING: return {0, 35};
        case HI_ACTIVE_COOLING: return {0, 45};
        case MED_ACTIVE_COOLING: return {0, 40};
        default: return {0, 0};  // Default case, if necessary
    }
}

BreachType inferBreach(double value, double lowerLimit, double upperLimit) {
    if (value < lowerLimit) return TOO_LOW;
    if (value > upperLimit) return TOO_HIGH;
    return NORMAL;
}

BreachType classifyTemperatureBreach(CoolingType coolingType, double temperatureInC) {
    CoolingLimits limits = getCoolingLimits(coolingType);
    return inferBreach(temperatureInC, limits.lowerLimit, limits.upperLimit);
}

void alertBasedOnTarget(AlertTarget alertTarget, BreachType breachType) {
    if (alertTarget == TO_CONTROLLER) {
        sendToController(breachType);
    } else if (alertTarget == TO_EMAIL) {
        sendToEmail(breachType);
    }
}

void checkAndAlert(AlertTarget alertTarget, BatteryCharacter batteryChar, double temperatureInC) {
    BreachType breachType = classifyTemperatureBreach(batteryChar.coolingType, temperatureInC);
    alertBasedOnTarget(alertTarget, breachType);
}

void sendToController(BreachType breachType) {
    const unsigned short header = 0xfeed;
    printf("%x : %x\n", header, breachType);
}

void sendToEmail(BreachType breachType) {
    const char* recepient = "a.b@c.com";
    if (breachType == TOO_LOW) {
        printf("To: %s\nHi, the temperature is too low\n", recepient);
    } else if (breachType == TOO_HIGH) {
        printf("To: %s\nHi, the temperature is too high\n", recepient);
    }
}
