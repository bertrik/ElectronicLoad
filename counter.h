/**
 * Handles accumulation of measurement values.
 */
 
#include <stdint.h>

/** Initialises the counter system. */
void CounterInit(void);

/** Resets the charge counter. */
void CounterChargeReset(void);

/**
 * Updates the charge counter with a new current value.
 * @param[in] micros the time in microseconds
 * @param[in] current the actual current in amperes
 */
void CounterChargeUpdate(uint32_t timestamp, float current);

/** @return the charge counter value */
double CounterChargeGet(void);

/** Resets the energy counter. */
void CounterEnergyReset(void);

/**
 * Updates the energy counter with a new power value.
 * @param[in] micros the time in microseconds
 * @param[in] power the actual power in Watts
 */
void CounterEnergyUpdate(uint32_t timestamp, float power);

/** @return the energy counter value */
double CounterEnergyGet(void);

