/**
 * Handles accumulation of measurement values.
 */
 
#include <stdint.h>

/** Initialises the counter system. */
void CounterInit(void);

void CounterChargeReset(void);
void CounterChargeUpdate(uint32_t timestamp, float current);
double CounterChargeGet(void);

void CounterEnergyReset(void);
void CounterEnergyUpdate(uint32_t timestamp, float power);
double CounterEnergyGet(void);

