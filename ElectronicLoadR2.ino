#include "measure.h"
#include "counter.h"

#include "Arduino.h"

void setup()
{
    MeasureInit();
    CounterInit();
}

void loop()
{
    uint32_t time;
    float current, voltage;
    
    MeasureGet(&time, &current, &voltage);
    float power = current * voltage;
    
    CounterChargeUpdate(time, current);
    CounterEnergyUpdate(time, power);
}

