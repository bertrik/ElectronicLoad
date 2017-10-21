#include <Arduino.h>

#include "pindefs.h"
#include "led.h"

static ELedMode mode;
static float unit;

void LedInit(void)
{
    pinMode(PIN_LED, OUTPUT);
    digitalWrite(PIN_LED, 0);
    
    LedSetMode(E_LEDMODE_TIME);
}

void LedSetMode(ELedMode newMode)
{
    mode = newMode;
    
    switch (mode) {
    case E_LEDMODE_ON:
        digitalWrite(PIN_LED, 1);
        break;
    case E_LEDMODE_OFF:
        digitalWrite(PIN_LED, 0);
        break;
    case E_LEDMODE_TIME:
        unit = 500000;
        break;
    case E_LEDMODE_CHARGE:
        unit = 0.01;
        break;
    case E_LEDMODE_ENERGY:
        unit = 0.001;
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
        tick = time / unit;
        break;
    case E_LEDMODE_CHARGE:
        tick = charge / unit;
        break;
    case E_LEDMODE_ENERGY:
        tick = energy / unit;
        break;
    default:
        // do nothing
        return;
    }
    
    digitalWrite(PIN_LED, tick & 1);
}

