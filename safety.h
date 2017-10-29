
/** Initializes the safety module. */
void SafetyInit(void);

/**
 * Updates the safety module with new measurement values.
 * @param[in] current the measured current
 * @param[in] voltage the measured voltage
 * @return the constrained voltage
 */
float SafetyTick(float current, float voltage);

float SafetySetMaxCurrent(float current);
float SafetySetMaxPower(float power);
float SafetySetMinVoltage(float voltage);

void SafetyGetLimits(float *current, float *power, float *voltage);

