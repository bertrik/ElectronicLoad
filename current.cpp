#include "pindefs.h"
#include "current.h"
#include "Arduino.h"

static HardwareTimer pwmtimer(2);

void CurrentInit(void)
{
    pinMode(PIN_CURRENT_OUT, PWM);

    pwmtimer.setPrescaleFactor(1);
    pwmtimer.setOverflow(4096);       // Instead of 2^16 (72MHz/2^16=1100Hz) it overflows at 2^12 (4096, 17kHz)
}

void CurrentSetValue(float current)
{
    // limit current to range
    if (current < 0.0) {
        current = 0.0;
    } else if (current > 5.0) {
        current = 5.0;
    }

    int val = (int)(4095 * current / 5.0);
    
    pwmWrite(PIN_CURRENT_OUT, val);
}

