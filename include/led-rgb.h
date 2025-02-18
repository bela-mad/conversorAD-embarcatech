#ifndef LED_RGB_H
#define LED_RGB_H

#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/pwm.h"

// Defines
#define GREEN_LED_PIN 11
#define BLUE_LED_PIN 12
#define RED_LED_PIN 13

// Cabeçalhos das funções
void led_init(uint gpio);
void pwm_led_setup(uint led, uint *slice, uint16_t level, bool state_led);

#endif // LED_RGB_H