#include <Arduino.h>

#include "hwdefs.h"
#include "led.h"

static ELedMode mode;

void LedInit(void)
{
    pinMode(PIN_LED, OUTPUT);
    LedSetMode(E_LEDMODE_TIME);
}

void LedSetMode(ELedMode newMode)
{
    mode = newMode;
    
    switch (mode) {
    case E_LEDMODE_ON:
        digitalWrite(PIN_LED, 0);
        break;
    case E_LEDMODE_OFF:
        digitalWrite(PIN_LED, 1);
        break;
    default:
        break;
    }
}

void LedUpdate(unsigned long int time, float charge, float energy)
{
    int tick;

    switch (mode) {
    case E_LEDMODE_TIME:
        tick = time / 500000;
        break;
    case E_LEDMODE_CURRENT:
        tick = charge / 0.1;
        break;
    case E_LEDMODE_OFF:
    case E_LEDMODE_ON:
    default:
        // do nothing
        return;
    }
    
    digitalWrite(PIN_LED, tick & 1);
}

