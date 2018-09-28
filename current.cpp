#include "hwdefs.h"
#include "current.h"

#include "Arduino.h"
#include "Adafruit_MCP4725.h"

#define MCP4725_I2C_ADDR 0x60

static HardwareTimer pwmtimer(2);
static float setting = 0.0;
static float cal_iset = 1.0;

static bool have_dac = false;
static Adafruit_MCP4725 dac;

void CurrentInit(void)
{
    // check for presence of I2C DAC
    Wire.begin();
    Wire.setClock(400000);
    Wire.beginTransmission(MCP4725_I2C_ADDR);
    have_dac = (Wire.endTransmission() == 0);

    // initialise dac if present
    if (have_dac) {
        dac.begin(MCP4725_I2C_ADDR);
        dac.setVoltage(0, true); // writes default setting (0) to eeprom, is used directly after power on.
    } else {
        pinMode(PIN_CURRENT_OUT, PWM);
        // following two lines set the PWM to 70 kHz
        pwmtimer.setPrescaleFactor(1);
        pwmtimer.setOverflow(1023); // counts to 1023 and then overflows
        // 72MHz clock / 1024 = 70.312 kHz
        // current set resolution is 1024 levels/10bits
    }
}

void CurrentCal(float value)
{
    cal_iset = value;
}

float CurrentSetValue(float current)
{
    // limit current to range
    if (current < 0.0) {
        setting = 0.0;
    } else if (current > 5.0) {
        setting = 5.0;
    } else {
        setting = current;
    }

    // set it
    if (have_dac) {
        int dacSetting = 4095 * setting * cal_iset / 5.0;
        if (dacSetting > 4095) {
            dacSetting = 4095;
        }
        dac.setVoltage(dacSetting, false); 
    } else {
        int pwm = (int) (1024 * setting * cal_iset / 5.0);

        // MCU output does not go below ~4mV, causing ~6mA load current. 
        if (setting < 0.005) {      // for currents below 5mA
            // The pin is made input, the voltage divider pulls the voltage to 0mV and load current is 0mA.
            pinMode(PIN_CURRENT_OUT, INPUT);
        } else {
            pinMode(PIN_CURRENT_OUT, PWM);
            pwmWrite(PIN_CURRENT_OUT, pwm);
        }
    }

    return setting;
}

float CurrentGetValue(void)
{
    return setting;
}

