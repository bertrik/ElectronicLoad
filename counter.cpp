/**
 * Handles accumulation of measurement values.
 */

#include "counter.h"

typedef struct {
    float       integrated;
    uint32_t    last_time;
    float       last_value;
} TIntegrator;

static TIntegrator charge;
static TIntegrator energy;
 
/* performs basic trapezoid integration */ 
static void integrate(TIntegrator *state, uint32_t timestamp, float value)
{
    float average = (value + state->last_value) / 2;
    float sum = average * (timestamp - state->last_time) * 1E-6;
    state->integrated += sum;
    state->last_time = timestamp;
    state->last_value = value;
}

/**
 * Initialises the counter system.
 */
void CounterInit(void)
{
    charge.integrated = 0;
    charge.last_time = 0;
    charge.last_value = 0;
    
    energy.integrated = 0;
    energy.last_time = 0;
    energy.last_value = 0;
}

void CounterChargeReset(void)
{
    charge.integrated = 0;
}

void CounterChargeUpdate(uint32_t timestamp, float current)
{
    integrate(&charge, timestamp, current);
}

float CounterChargeGet(void)
{
    return charge.integrated;
}

void CounterEnergyReset(void)
{
    energy.integrated = 0;
}

void CounterEnergyUpdate(uint32_t timestamp, float power)
{
    integrate(&energy, timestamp, power);
}

float CounterEnergyGet(void)
{
    return energy.integrated;
}

