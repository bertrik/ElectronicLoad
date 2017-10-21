#include "measure.h"
#include "counter.h"
#include "current.h"
#include "print.h"
#include "pindefs.h"
#include "control.h"
#include "cmdproc.h"

#include "Arduino.h"

void setup()
{
    PrintInit();
    MeasureInit();
    CounterInit();
    CurrentInit();
    ControlInit();
    
    pinMode(PIN_LED, OUTPUT);
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

static int do_help(int argc, char *argv[]);

static int do_cc(int argc, char *argv[])
{
    if (argc != 2) {
        print("Please specify a current!\n");
        return -1;
    }
    int ma = atoi(argv[1]);
    print("Constant current %d mA\n", ma);
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
    print("Constant power %d mW\n", mw);
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
    print("Constant resistance %d mA\n", ohm);
    ControlSetMode(CR, ohm);
    return 0;
}

const cmd_t commands[] = {
    {"help",    do_help,    "Shows help"},
    {"cc",      do_cc,      "<mA>, sets constant current mode"},
    {"cp",      do_cp,      "<mW>, sets constant power mode"},
    {"cr",      do_cr,      "<ohm>, sets constant resistance mode"},
    {NULL, NULL, NULL}
};

static int do_help(int argc, char *argv[])
{
    for (const cmd_t *cmd = commands; cmd->cmd != NULL; cmd++) {
        print("%10s: %s\n", cmd->name, cmd->help);
    }
    return 0;
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

    ControlTick(time, current, voltage, power);

    if (Serial.available()) {        // read from serial until return
        haveLine = EditLine(Serial.read(), &c, line, sizeof(line));
        Serial.print(c);
    }
    if (haveLine) {
        int result = cmd_process(commands, line);
        switch (result) {
        case CMD_NO_CMD:
            break;
        case CMD_UNKNOWN:
            print("Unknown command!");
            break;
        default:
            print("%d", result);
            break;
        }
        print("\n%s>", ControlGetModeString());
    }
}

