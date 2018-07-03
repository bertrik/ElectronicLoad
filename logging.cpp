#include <stdbool.h>

#include "print.h"
#include "logging.h"

static bool show_header;
static int interval;
static unsigned long lasttick;

void LoggingInit(void)
{
    show_header = false;
    interval = 0;
}

void LoggingOn(int intervalMs)
{
    show_header = true;
    interval = intervalMs;
}

void LoggingOff(void)
{
    show_header = false;
    interval = 0;
}

void LoggingUpdate(unsigned long micros, float cur_set, float current, float voltage, float power, float charge, float energy)
{
    unsigned long tick, millis;
    
    if (show_header) {
        print("\nT(sec);Iset(A);I(A);V(V);P(W);Q(mAh);E(mWh)\n");
        show_header = false;
    }

    millis = micros / 1000;
    if (interval > 0) {
        tick = millis / interval;
        if (tick != lasttick) {
            print("%.1f;%2.3f;%2.3f;%2.3f;%2.3f;%.3f;%.3f\n",
                millis / 1000.0, cur_set, current, voltage, power, charge / 3.6, energy / 3.6);
            lasttick = tick;
        }
    }
}


