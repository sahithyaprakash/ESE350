/*
 * wiper_encoder.h
 *
 * Created: 2/10/2015
 *  Author: Chao Liu (chao.liu0307@gmail.com)
 */ 


#ifndef WIPER_ENCODER_H_
#define WIPER_ENCODER_H_

#include "common_peripherals.h"
#include "ADC.h"
#include <stdlib.h>
#include <avr/eeprom.h> // commented to allow manual calibration.
//#include "medfilt.h"

extern int adc0_0_eeprom;
extern int adc0_180_eeprom;
extern int adc1_0_eeprom;  // zero degree for tilt wiper
extern int adc1_180_eeprom;
extern float resolu_0_eeprom;
extern float resolu_1_eeprom;

void init_wiper_encoder(void);
int return_current_position(void);
long return_current_velocity(void);
int return_adc0(void);
int return_adc1(void);
//void get_eeprom(void);

#endif /* WIPER_ENCODER_H_ */
