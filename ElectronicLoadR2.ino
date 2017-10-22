#include "measure.h"
#include "counter.h"
#include "current.h"
#include "print.h"
#include "pindefs.h"
#include "control.h"
#include "cmdproc.h"
#include "led.h"
#include "safety.h"
#include "editline.h"

#include "Arduino.h"

static unsigned long time;
static float current, voltage, power, charge, energy;
static float curset;
static char line[128];

void setup()
{
    PrintInit();
    EditInit(line, sizeof(line));
    LedInit();
    MeasureInit();
    CounterInit();
    CurrentInit();
    ControlInit();
    SafetyInit();
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
    return 0;
}

static int do_off(int argc, char *argv[])
{
    ControlSetMode(OFF, 0);
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
    } modes[] = {
        {"off",     E_LEDMODE_OFF},
        {"on",      E_LEDMODE_ON},
        {"time",    E_LEDMODE_TIME},
        {"current", E_LEDMODE_CURRENT},
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
            print("Setting LED mode '%s' (blink frequency depends on %s)\n", mode->name, mode->name);
            LedSetMode(mode->mode);
            return 0;
        }
    }
    
    print("Unknown LED mode '%s'!\n", argv[1]);
    return -1;
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
    {NULL, NULL, NULL}
};

static int do_help(int argc, char *argv[])
{
    for (const cmd_t *cmd = commands; cmd->cmd != NULL; cmd++) {
        print("%10s: %s\n", cmd->name, cmd->help);
    }
    return 0;
}

void loop()
{
    char c;
    bool haveLine = false;
    
    curset = CurrentGetValue();
    MeasureGet(&time, &current, &voltage);
    power = current * voltage;
    
    CounterChargeUpdate(time, current);
    CounterEnergyUpdate(time, power);
    charge = CounterChargeGet();
    energy = CounterEnergyGet();

    ControlTick(time, current, voltage, power);
    SafetyTick(current, power);
    LedUpdate(time, charge, energy);

    if (Serial.available()) {
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
            print("Unknown command!\n");
            break;
        default:
            print("%d\n", result);
            break;
        }
        print("%s>", ControlGetModeString());
    }
}

