
/** Initializes the safety module. */
void SafetyInit(void);

/**
 * Updates the safety module with new measurement values.
 * @param[in] current the measured current
 * @param[in] power the measured power
 */
void SafetyTick(float current, float power);

