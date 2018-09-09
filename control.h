#include <stdint.h>

typedef enum {
    // current switched off
    OFF,
    // constant current
    CC,
    // constant power
    CP,
    // constant resistance
    CR,
    // pulsed current
    CM_PI
} EControlMode;

typedef struct {
    int duration;
    float current;
} TPulsedCurrent;

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
 * Configures and enabled pulsed current mode.
 * 
 * @param[in] num number of (duration, current) pairs
 * @param[in] the pulse current pairs
*/
void ControlSetPulsedMode(int num, TPulsedCurrent *pulses);

/**
 * @return a string describing the current control mode
 */
const char *ControlGetModeString(void);

/**
 * Updates the control module with new measurement values.
 * @param[in] micros the current time in microseconds
 * @param[in] current the actual current in amperes
 * @param[in] voltage the actual voltage in volts
 * @return the desired output current
 */ 
float ControlTick(uint32_t micros, float current, float voltage);




