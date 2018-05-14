#include <string.h>

#include "Arduino.h"
#include <EEPROM.h>

#include "calibrate.h"

#define CAL_ADDR    0
#define CAL_MAGIC   "CAL"

typedef struct {
    char magic[4];
    size_t len;
    uint8_t data[120];
} TCalHeader;

static TCalHeader header;

void CalInit(void)
{
    // nothing to do yet
}

static void eeprom_put(int address, const void *data, int len)
{
    const char *pc = (char *)data;
    for (int i = 0; i < len; i++) {
        EEPROM.update(address + i, pc[i]);
    }
}

static void eeprom_get(int address, void *data, int len)
{
    char *pc = (char *)data;
    for (int i = 0; i < len; i++) {
        pc[i] = EEPROM.read(address + i);
    }
}


bool CalRead(void *data, size_t len)
{
    // read it
    eeprom_get(CAL_ADDR, &header, sizeof(header));
    
    // verify
    if (strcmp(header.magic, CAL_MAGIC) != 0) {
        return false;
    }
    if (header.len != len) {
        return false;
    }
    memcpy(data, header.data, len);
    return true;
}

bool CalWrite(void *data, size_t len)
{
    if (len > sizeof(header.data)) {
        return false;
    }
    strcpy(header.magic, CAL_MAGIC);
    header.len = len;
    memset(header.data, 0, sizeof(header.data));
    memcpy(header.data, data, len);
    eeprom_put(CAL_ADDR, &header, sizeof(header));
        
    return true;    
}

