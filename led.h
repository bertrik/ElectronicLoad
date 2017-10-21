
typedef enum {
    E_LEDMODE_OFF,
    E_LEDMODE_ON,
    E_LEDMODE_TIME,
    E_LEDMODE_CHARGE,
    E_LEDMODE_ENERGY,
} ELedMode;

void LedInit(void);
void LedSetMode(ELedMode newMode);
void LedUpdate(unsigned long int time, float charge, float energy);

