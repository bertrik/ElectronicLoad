#include "hwdefs.h"
#include "current.h"
#include "Arduino.h"

static HardwareTimer pwmtimer(2);
static float setting = 0.0;

void CurrentInit(void)
{
    pinMode(PIN_CURRENT_OUT, PWM);
    // following two lines set the PWM to 70 kHz
    pwmtimer.setPrescaleFactor(1);
    pwmtimer.setOverflow(1023); // counts to 1023 and then overflows
    // 72MHz clock / 1024 = 70.312 kHz
    // current set resolution is 1024 levels/10bits
}

float CurrentSetValue(float current)
{
    // limit current to range
    if (current < 0.005) {
        setting = 0.0;
    } else if (current > 5.0) {
        setting = 5.0;
    } else {
        setting = current;
    }

    int pwm = (int)(1024 * setting / 5.0);

    // MCU output does not go below ~4mV, causing ~6mA load current. 
    if (setting<0.005){ // for currents below 5mA
      // The pin is made input, the voltage divider pulls the voltage to 0mV and load current is 0mA.
      pinMode(PIN_CURRENT_OUT, INPUT);
    }
    else{
      pinMode(PIN_CURRENT_OUT, PWM);
      pwmWrite(PIN_CURRENT_OUT, pwm);
    }

    return setting;
}

float CurrentGetValue(void)
{
    return setting;
}

