#include <stdint.h>

typedef struct {
    float cal_iset;
    float cal_i;
    float cal_v;
} TMeasureCal;

/** Initialises the measurement system. */
void MeasureInit(void);

/**
 * Sets the calibration data.
 *
 * @param[in] cal the calibration structure
 */
void MeasureCal(const TMeasureCal *cal);

/**
 * Gets a measurement.
 *
 * @param[out] time the timestamp for this measurement
 * @param[out] the current (amperes)
 * @param[out] the voltage (volts)
 */
void MeasureGet(unsigned long *time, float *current, float *voltage);

