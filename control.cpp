#include "current.h"
#include "control.h"

#include <Arduino.h>

static EControlMode mode;
static float targetValue;

void ControlInit(void)
{
    ControlSetMode(OFF, 0.0);
}

static void UpdateCurrent(float desired)
{
    // perhaps run some kind of loop filter here?
    CurrentSetValue(desired);
}

void ControlSetMode(EControlMode newMode, float newTarget)
{
    mode = newMode;
    targetValue = newTarget;

    switch (mode) {
    case OFF:
        CurrentSetValue(0.0);
        break;
    case CC:
        CurrentSetValue(targetValue);
        break;
    default:
        break;
    }
}

const char *ControlGetModeString(void)
{
    switch (mode) {
    case OFF:   return "OFF";
    case CC:    return "CC";
    case CP:    return "CP";
    case CR:    return "CR";
    default:    return "?";
    }
}

/** runs the control process */
void ControlTick(uint32_t micros, float current, float voltage, float power)
{
    float desired;

    switch (mode) {
    case OFF:
    case CC:
        // nothing to do
        break;
    case CP:
        // calculate current for desired power
        desired = (targetValue / voltage);
        UpdateCurrent(desired);
        break;
    case CR:
        // calculate current for desired resistance emulation
        desired = (voltage / targetValue);
        UpdateCurrent(desired);
        break;
    default:
        // should not come here
        mode = OFF;
        break;
    }
}


