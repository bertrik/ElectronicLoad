#include "measure.h"

#include "Arduino.h"

/**
 * Initialises the measurement system.
 */
void MeasureInit(void)
{
}

/**
 * Gets a measurement.
 *
 * @param[out] time the timestamp for this measurement
 * @param[out] the current (amperes)
 * @param[out] the voltage (volts)
 */
void MeasureGet(uint32_t *time, float *current, float *voltage)
{
    *time = micros();
    *current = 0;
    *voltage = 0;
}

