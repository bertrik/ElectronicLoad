#include <stdint.h>

typedef enum {
    // current switched off
    OFF,
    // constant current
    CC,
    // constant power
    CP,
    // constant resistance
    CR
} EControlMode;

/**
 * Initializes the control module.
 */
void ControlInit(void);

/**
 * Sets the control mode.
 * @param[in] newMode the new mode
 * @param(in] newTarget the target value (e.g. current, power, resistance)
 */ 
void ControlSetMode(EControlMode newMode, float newTarget);

/**
 * @return a string describing the current control mode
 */
const char *ControlGetModeString(void);

/**
 * Updates the control module with new measurement values.
 * @param[in] micros the current time in microseconds
 * @param[in] current the actual current in amperes
 * @param[in] voltage the actual voltage in volts
 * @param[in] power the actual power in Watts
 */ 
void ControlTick(uint32_t micros, float current, float voltage, float power);




