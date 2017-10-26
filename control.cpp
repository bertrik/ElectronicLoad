#include "control.h"

#include <Arduino.h>

static EControlMode mode;
static float targetValue;

void ControlInit(void)
{
    ControlSetMode(OFF, 0.0);
}

void ControlSetMode(EControlMode newMode, float newTarget)
{
    mode = newMode;
    targetValue = newTarget;
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
float ControlTick(uint32_t micros, float current, float voltage, float power)
{
    float desired;

    switch (mode) {
    case OFF:
        return 0.0;
    case CC:
        return targetValue;
    case CP:
        // calculate current for desired power
        desired = (targetValue / voltage);
        return desired;
    case CR:
        // calculate current for desired resistance emulation
        desired = (voltage / targetValue);
        return desired;
    default:
        // should not come here
        mode = OFF;
        return 0.0;
    }
}


