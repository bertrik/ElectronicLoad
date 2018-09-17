#include <stdbool.h>
#include "safety.h"
#include "hwdefs.h"

static float max_current;
static float max_power;
static float min_voltage;

void SafetyInit(void)
{
    max_current = MAX_CURRENT;
    max_power = MAX_POWER;
    min_voltage = 1.0;
}

static float constrain(float min, float max, float value)
{
    if (value < min) {
        return min;
    } else if (value < max) {
        return value;
    } else {
        return max;
    }
}

float SafetySetMaxCurrent(float current)
{
    max_current = constrain(0.0, MAX_CURRENT, current);
    return max_current;
}

float SafetySetMaxPower(float power)
{
    max_power = constrain(0.0, MAX_POWER, power);
    return max_power;
}

float SafetySetMinVoltage(float voltage)
{
    min_voltage = constrain(0.0, MAX_VOLTAGE, voltage);
    return min_voltage;
}

void SafetyGetLimits(float *current, float *power, float *voltage)
{
    *current = max_current;
    *power = max_power;
    *voltage = min_voltage;
}

float SafetyTick(float current, float voltage)
{
    float cur_out = current;
    
    // constrain power
    float power = voltage * cur_out;
    if (power > max_power) {
        cur_out = max_power / voltage;
    }
    
    // constrain current
    if (cur_out > max_current) {
        cur_out = max_current;
    }
    
    // constrain voltage
    if (voltage < min_voltage) {
        cur_out = 0.0;
    }

    return cur_out;    
}

