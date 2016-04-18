#ifndef ATM_CALI_
#define ATM_CALI_

#include "wiper_encoder.h"
#include "TWI_slave.h"
#include <avr/eeprom.h>

// auto calibration cmds
#define CALI_ON               0xA7
#define CALI_SWITCH           0xA8
#define CALI_OFF              0xA9
#define CALI_ON_TILT_M90      0xAA
#define CALI_ON_TILT_0        0xAB
#define CALI_ON_TILT_90       0xAC

void cali_mode();
void cali_off_mode();
void cali_on_tilt(int address);

int get_adc0_0();
int get_adc1_0();
int get_adc0_180();
int get_adc1_180();
float get_resolution_0();
float get_resolution_1();

#endif //ATM_CALI_