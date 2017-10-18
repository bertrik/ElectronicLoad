#include "measure.h"

#include "Arduino.h"

#define PIN_MEASURE_A   PA0   
#define PIN_MEASURE_V   PA2

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
    *current = 5.0 * analogRead(PIN_MEASURE_A) / 4096;
    *voltage = 19.8 * analogRead(PIN_MEASURE_V) / 4096;
}

