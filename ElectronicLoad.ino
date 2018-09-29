#include "measure.h"
#include "counter.h"
#include "current.h"
#include "print.h"
#include "hwdefs.h"
#include "control.h"
#include "cmdproc.h"
#include "led.h"
#include "safety.h"
#include "editline.h"
#include "logging.h"
#include "calibrate.h"

#include "Arduino.h"

static unsigned long time;
static float curset;
static float current, voltage, power;
static double charge, energy;
static char line[128];

void setup()
{
    TMeasureCal cal;

    PrintInit();
    EditInit(line, sizeof(line));
    LedInit();
    MeasureInit();
    CounterInit();
    CurrentInit();
    ControlInit();
    SafetyInit();
    LoggingInit();
    
    CalInit();
    if (CalRead(&cal, sizeof(cal))) {
        MeasureCal(&cal);
        CurrentCal(cal.cal_iset);
    }
}

static void show_help(const cmd_t *cmds)
{
    for (const cmd_t *cmd = cmds; cmd->cmd != NULL; cmd++) {
        print("%10s: %s\n", cmd->name, cmd->help);
    }
}

static int do_help(int argc, char *argv[]);

static int do_cc(int argc, char *argv[])
{
    if (argc != 2) {
        print("Please specify a current!\n");
        return -1;
    }
    int ma = atoi(argv[1]);
    print("Set constant current mode %d mA\n", ma);
    ControlSetMode(CC, ma / 1000.0);
    LoggingOn(500);
    return 0;
}

static int do_cp(int argc, char *argv[])
{
    if (argc != 2) {
        print("Please specify a power!\n");
        return -1;
    }
    int mw = atoi(argv[1]);
    print("Set constant power mode %d mW\n", mw);
    ControlSetMode(CP, mw / 1000.0);
    LoggingOn(500);
    return 0;
}

static int do_cr(int argc, char *argv[])
{
    if (argc != 2) {
        print("Please specify a resistance!\n");
        return -1;
    }
    int ohm = atoi(argv[1]);
    print("Set constant resistance mode %d ohm\n", ohm);
    ControlSetMode(CR, ohm);
    LoggingOn(500);
    return 0;
}

static int do_off(int argc, char *argv[])
{
    ControlSetMode(OFF, 0);
    LoggingOff();
    return 0;
}

static int do_status(int argc, char *argv[])
{
    print("T=%4.6f,Iset=%2.3f A,I=%2.3f A,V=%2.3f V,P=%2.3f W,Q=%.0f mAh,E=%.0f mWh\n",
        time / 1E6, curset, current, voltage, power, charge / 3.6, energy / 3.6);

    return 0;
}

static int do_reset(int argc, char *argv[])
{
    print("Resetting counters.\n");
    CounterChargeReset();
    CounterEnergyReset();
    return 0;
}

static int do_led(int argc, char *argv[])
{
    struct TLedMode {
        const char *name;
        ELedMode mode;
        const char *explanation;
    } modes[] = {
        {"off",     E_LEDMODE_OFF,      "LED turned off"},
        {"on",      E_LEDMODE_ON,       "LED turned on"},
        {"time",    E_LEDMODE_TIME,     "LED blinks at fixed speed"},
        {"current", E_LEDMODE_CURRENT,  "LED blink frequency depends on current"},
        {NULL, E_LEDMODE_OFF}
    };

    if (argc != 2) {
        print("Please specify a LED mode, possible values:");
        for (struct TLedMode *mode = modes; mode->name != NULL; mode++) {
            print(" %s", mode->name);
        }
        print("\n");
        return -1;
    }

    for (struct TLedMode *mode = modes; mode->name != NULL; mode++) {
        if (strcmp(argv[1], mode->name) == 0) {
            print("Setting LED mode '%s' (%s)\n", mode->name, mode->explanation);
            LedSetMode(mode->mode);
            return 0;
        }
    }
    
    print("Unknown LED mode '%s'!\n", argv[1]);
    return -1;
}

static int do_log(int argc, char *argv[])
{
    if (argc != 2) {
        print("Please specify a logging interval (0 to stop)\n");
        return -1;
    }
    int interval = atoi(argv[1]);
    if (interval > 0) {
        LoggingOn(interval);
    } else {
        LoggingOff();
    }
    return 0;
}

static int do_limit(int argc, char *argv[])
{
    int ret = 0;
    float max_current, max_power, min_voltage;

    if (argc == 3) {
        char type = argv[1][0];
        int value = atoi(argv[2]);
        switch (type) {
        case 'i':
            print("Setting max current limit to %d mA\n", value);
            SafetySetMaxCurrent(value / 1000.0);
            break;
        case 'v':
            print("Setting min voltage limit to %d mV\n", value);
            SafetySetMinVoltage(value / 1000.0);
            break;
        case 'p':
            print("Setting max power limit to %d mW\n", value);
            SafetySetMaxPower(value / 1000.0);
            break;
        default:
            print("Unknown limit type '%c'!\n", type);
            ret = -1;
        }
    } else if (argc > 1) {
        print("Please specify a limit type (i,v,p) and a limit value\n");
        ret = -1;
    }
    SafetyGetLimits(&max_current, &max_power, &min_voltage);
    print("Limits are now:\nImax = %.3f A\nPmax = %.3f W\nVmin = %.3f V\n", max_current, max_power, min_voltage); 
    
    return ret;
}

static int do_cal(int argc, char *argv[])
{
    TMeasureCal cal0 = {1.0, 1.0, 1.0};
    float current, voltage;
    unsigned long time;
    TMeasureCal cal;

    // read current calibration value
    if (!CalRead(&cal, sizeof(cal))) {
        cal = cal0;
        CalWrite(&cal, sizeof(cal));
    }
    print("Current calibration factor (Iset,I,V): %.6f %.6f %.6f\n", cal.cal_iset, cal.cal_i, cal.cal_v);
    
    // read uncalibrated actual value
    MeasureCal(&cal0);
    MeasureGet(&time, &current, &voltage);

    char item = (argc > 1) ? argv[1][0] : '?'; 
    int actual = (argc > 2) ? atoi(argv[2]) : 0;
    
    float ratio;
    switch (item) {
    case 'i':
        ratio = actual / (1000.0 * current);
        cal.cal_iset = 1000.0 * CurrentGetValue() / actual;
        cal.cal_i = ratio;
        break;
    case 'v':
        ratio = actual / (1000.0 * voltage);
        cal.cal_v = ratio;
        break;
    case 'r':
        print("Resetting calibration\n");
        ratio = 1.0;
        cal.cal_iset = 1.0;
        cal.cal_i = 1.0;
        cal.cal_v = 1.0;
        break;
    default:
        print("Invalid calibration item '%c'!\n", item);
        return -1;
    }
    // calibrate current / voltage reading
    if ((ratio > 0.8) && (ratio < 1.2)) {
        MeasureCal(&cal);
        CurrentCal(cal.cal_iset);
        CalWrite(&cal, sizeof(cal));
        print("Updated calibration factor (Iset,I,V): %.6f %.6f %.6f\n", cal.cal_iset, cal.cal_i, cal.cal_v);
    } else {
        print("Value is off by %.f %%, ignoring!\n", 100.0 * (ratio - 1.0));
    }
    
    return 0;
}

static int do_pulse(int argc, char *argv[])
{
    if ((argc < 3) || (argc % 2) != 1) {
        print("Need pairs of time and current!\n");
        return -1;
    }

    // parse time and current pairs
    print("Configuring stepped current waveform:\n");
    TPulsedCurrent pairs[50];
    int step = 0;
    for (int i = 1; i < argc; i += 2) {
        pairs[step].duration = atoi(argv[i]);
        pairs[step].current = atoi(argv[i + 1]) / 1000.0;
        print("* %6d us at %.3f mA\n", pairs[step].duration, pairs[step].current);
        step++;
    }

    // configure it
    ControlSetPulsedMode(step, pairs);
    return 0;
}

const cmd_t commands[] = {
    {"help",    do_help,    "Show help"},
    {"cc",      do_cc,      "<mA>, set constant current mode"},
    {"cp",      do_cp,      "<mW>, set constant power mode"},
    {"cr",      do_cr,      "<ohm>, set constant resistance mode"},
    {"off",     do_off,     "Turn current off"},
    {"s",       do_status,  "Show status"},
    {"led",     do_led,     "<mode> Set LED mode"},
    {"reset",   do_reset,   "Reset charge/energy counters"},
    {"log",     do_log,     "<interval> Starts logging with <interval> ms"},
    {"limit",   do_limit,   "<i,v,p> <value> Set the current/voltage/power limit in mA/mW/mV"},
    {"cal",     do_cal,     "<i,v> <actual> calibrate current/voltage towards actual value (mA/mV)"},
    {"pulse",   do_pulse,   "[d, i]+ pulsed current, with pairs of duration (us) and current (ma)"},
    {NULL, NULL, NULL}
};

static int do_help(int argc, char *argv[])
{
    show_help(commands);
    return 0;
}

void loop()
{
    MeasureGet(&time, &current, &voltage);
    power = current * voltage;
    
    CounterChargeUpdate(time, current);
    CounterEnergyUpdate(time, power);
    charge = CounterChargeGet();
    energy = CounterEnergyGet();

    curset = ControlTick(time, current, voltage);
    float cursafe = SafetyTick(curset, voltage);
    if (curset > cursafe) {
        // TODO show some kind of warning, but limit it in time (e.g. only once per second)
        curset = cursafe;
    }
    CurrentSetValue(curset);
    
    LedUpdate(time, charge, energy);
    LoggingUpdate(time, curset, current, voltage, power, charge, energy);

    bool haveLine = false;
    if (Serial.available()) {
        char c;
        haveLine = EditLine(Serial.read(), &c);
        Serial.print(c);
    }
    if (haveLine) {
        int result = cmd_process(commands, line);
        switch (result) {
        case CMD_OK:
            print("OK\n");
            break;
        case CMD_NO_CMD:
            break;
        case CMD_UNKNOWN:
            print("Unknown command, available commands:\n");
            show_help(commands);
            break;
        default:
            print("%d\n", result);
            break;
        }
        print("%s>", ControlGetModeString());
    }
}

