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

void ControlInit(void);

void ControlSetMode(EControlMode newMode, int newTarget);

void ControlTick(uint32_t micros, float current, float voltage, float power);




