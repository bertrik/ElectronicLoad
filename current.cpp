#include "pindefs.h"
#include "current.h"
#include "Arduino.h"

void CurrentInit(void)
{
    pinMode(PIN_CURRENT_OUT, PWM);
}

void CurrentSetValue(float current)
{
    if (current < 0.0) {
        current = 0.0;
    } else if (current > 5.0) {
        current = 5.0;
    }

    int val = (int)(65535 * current / 5.0);
    pwmWrite(PIN_CURRENT_OUT, val);
}

