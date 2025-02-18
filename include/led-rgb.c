#include "led-rgb.h"

// Declaração de variáveis
const float PWM_DIVISOR = 4.0;
const uint16_t WRAP_PERIOD = 4095;   

// Funções

// Função que inicializa o pino do LED e configura como saída
void led_init(uint gpio) {
    gpio_init(gpio);              // inicializa o LED
    gpio_set_dir(gpio, GPIO_OUT); // configura o LED como saída
}

// Função para configurar o PWM de um LED
void pwm_led_setup(uint led, uint *slice, uint16_t level, bool state_led) {
    gpio_set_function(led, GPIO_FUNC_PWM); // configura o pino do LED como saída PWM
    *slice = pwm_gpio_to_slice_num(led);   // obtém o slice do PWM associado ao pino do LED
    pwm_set_clkdiv(*slice, PWM_DIVISOR);   // define o divisor de clock do PWM
    pwm_set_wrap(*slice, WRAP_PERIOD);     // configura o valor máximo do contador (período do PWM)
    pwm_set_gpio_level(led, 0);            // define o level do slice e do canal
    pwm_set_enabled(*slice, true);         // habilita o PWM no slice correspondente ao LED
}