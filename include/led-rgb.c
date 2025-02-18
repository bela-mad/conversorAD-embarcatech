#include "led-rgb.h"

const float PWM_DIVISOR = 4.0;  // Definição
const uint16_t WRAP_PERIOD = 4095;    // Definição

// Funções

// Função que inicializa o pino do LED e configura como saída
void led_init(uint gpio) {
    gpio_init(gpio);              // inicializa o LED
    gpio_set_dir(gpio, GPIO_OUT); // configura o LED como saída
}

// Função para configurar o PWM de um LED (genérica para azul e vermelho)
void pwm_led_setup(uint led, uint *slice, uint16_t level, bool state_led) {
    gpio_set_function(led, GPIO_FUNC_PWM); // Configura o pino do LED como saída PWM
    *slice = pwm_gpio_to_slice_num(led);   // Obtém o slice do PWM associado ao pino do LED
    pwm_set_clkdiv(*slice, PWM_DIVISOR);   // Define o divisor de clock do PWM
    pwm_set_wrap(*slice, WRAP_PERIOD);          // Configura o valor máximo do contador (período do PWM)
    pwm_set_gpio_level(led, 0);
    pwm_set_enabled(*slice, true);         // Habilita o PWM no slice correspondente ao LED
}