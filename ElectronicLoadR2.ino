#include "measure.h"
#include "counter.h"
#include "current.h"

#include "Arduino.h"

void setup()
{
    MeasureInit();
    CounterInit();
    CurrentInit();
    
    Serial.begin();
    pinMode(PC13, OUTPUT);
}

static bool EditLine(char cin, char *cout, char line[], int size)
{
    static int pos = 0;
    *cout = cin;                // echo by default
    switch (cin) {              // carriage return is ignored
    case '\r':
        break;
    case '\n':                 // end-of-line
        line[pos] = '\0';
        pos = 0;
        return true;
    case 0x7F:
    case 0x08:                 // backspace
        if (pos > 0) {
            pos--;
        }
        break;
    default:
        if (pos < (size - 1)) { // store char as long as there is space to do so
            line[pos++] = cin;
        } else {
            *cout = 0x07;       // bell
        }
        break;
    }
    return false;
}

static char line[80];

void loop()
{
    uint32_t time;
    float current, voltage;
    char c;
    bool haveLine = false;
    
    MeasureGet(&time, &current, &voltage);
    float power = current * voltage;
    
    CounterChargeUpdate(time, current);
    CounterEnergyUpdate(time, power);
    float charge = CounterChargeGet();
    float energy = CounterEnergyGet();

    if (Serial.available()) {        // read from serial until return
        haveLine = EditLine(Serial.read(), &c, line, sizeof(line));
        Serial.print(c);
    }
    if (haveLine) {
        // status
        if (strncmp(line, "s", 2) == 0) {
            Serial.print("current=");
            Serial.print(current, 3);
            Serial.print(", voltage=");
            Serial.print(voltage, 3);
            Serial.print(", power=");
            Serial.print(power, 3);
            Serial.print(", charge=");
            Serial.print(charge, 3);
            Serial.print(", energy=");
            Serial.println(energy, 3);
        }
    }
}

