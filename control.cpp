#include "control.h"

#include <Arduino.h>

#define MAX_PULSES  50

static EControlMode mode;
static float targetValue;

static int pulse_num;
static TPulsedCurrent pulse_values[MAX_PULSES];
static int pulse_idx;
static unsigned long pulse_time;

/** calculates the current pulse value */
static float PulsedCurrent(unsigned long now)
{
    TPulsedCurrent *pulse = &pulse_values[pulse_idx];
    if ((now - pulse_time) > pulse->duration) {
        pulse_time += pulse->duration;
        pulse_idx = (pulse_idx + 1) % pulse_num;
    }
    return pulse->current;
}

void ControlInit(void)
{
    ControlSetMode(OFF, 0.0);
}

void ControlSetMode(EControlMode newMode, float newTarget)
{
    mode = newMode;
    targetValue = newTarget;
}

void ControlSetPulsedMode(int num, TPulsedCurrent *pulses)
{
    // copy parameters
    if (num > MAX_PULSES) {
        num = MAX_PULSES;
    }
    pulse_num = num;
    memcpy(pulse_values, pulses, sizeof(TPulsedCurrent) * num);

    // reset pulse state
    pulse_idx = 0;
    pulse_time = micros();
    mode = CM_PI;
}

const char *ControlGetModeString(void)
{
    switch (mode) {
    case OFF:   return "OFF";
    case CC:    return "CC";
    case CP:    return "CP";
    case CR:    return "CR";
    case CM_PI: return "PI";
    default:    return "?";
    }
}

/** runs the control process */
float ControlTick(uint32_t micros, float current, float voltage)
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
    case CM_PI:
        return PulsedCurrent(micros);
    default:
        // should not come here
        mode = OFF;
        return 0.0;
    }
}


