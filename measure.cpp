#include "pindefs.h"
#include "measure.h"

#include "Arduino.h"

/**
 * Initialises the measurement system.
 */
void MeasureInit(void)
{
    pinMode(PIN_MEASURE_A, INPUT_ANALOG);
    pinMode(PIN_MEASURE_V, INPUT_ANALOG);
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

