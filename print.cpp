#include <stdarg.h>
#include "print.h"
#include "Arduino.h"

void PrintInit(void)
{
    Serial.begin(115200);
}

void print(const char *fmt, ...)
{
    // format it
    char buf[256];
    va_list args;
    va_start (args, fmt);
    vsnprintf(buf, sizeof(buf), fmt, args);
    va_end (args);

    // send it to serial
    Serial.write(buf);
}

