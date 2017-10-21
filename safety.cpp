#include <stdbool.h>

#include "control.h"
#include "print.h"

#define MAX_CURRENT 5.0
#define MAX_POWER   15.0

void SafetyInit(void)
{
    // nothing to do here yet
}

bool SafetyTick(float current, float power)
{
    if (current > MAX_CURRENT) {
        print("Safety engaged, current limit!\n");
        ControlSetMode(OFF, 0);
        return true;
    }
    if (power > MAX_POWER) {
        print("Safety engaged, power limit!\n");
        ControlSetMode(OFF, 0);
        return true;
    }
    
    return false;
}

