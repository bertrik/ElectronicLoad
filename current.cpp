#include "pindefs.h"
#include "current.h"
#include "Arduino.h"

static HardwareTimer pwmtimer(2);
static float setting = 0.0;

void CurrentInit(void)
{
    pinMode(PIN_CURRENT_OUT, PWM);

    pwmtimer.setPrescaleFactor(1);
    pwmtimer.setOverflow(4096);       // Instead of 2^16 (72MHz/2^16=1100Hz) it overflows at 2^12 (4096, 17kHz)
}

float CurrentSetValue(float current)
{
    // limit current to range
    if (current < 0.0) {
        setting = 0.0;
    } else if (current > 5.0) {
        setting = 5.0;
    } else {
        setting = current;
    }

    int pwm = (int)(4095 * setting / 5.0);
    pwmWrite(PIN_CURRENT_OUT, pwm);

    return setting;
}

float CurrentGetValue(void)
{
    return setting;
}

