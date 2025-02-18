#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"

// Defines
#define VRX 26         
#define VRY 27        
#define ADC_CHANNEL_0 0
#define ADC_CHANNEL_1 1

// Cabeçalhos das funções
void joystick_read_axis(uint16_t *vrx_value, uint16_t *vry_value);
void setup_joystick();

#endif