
/** Initialises the current module. */
void CurrentInit(void);

/**
 * Set the current setting calibration value
 * @param[in] value the calibration value
 */
void CurrentCal(float value);

/**
 * Sets the output current.
 * @param[in] the output current
 */
float CurrentSetValue(float current);

/** @return the output current setting */
float CurrentGetValue(void);

