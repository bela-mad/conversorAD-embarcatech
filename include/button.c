#include "button.h"

// Funções

// Função que inicializa o pino do botão e configura como entrada e pull up
void button_init(uint gpio) {
    gpio_init(gpio);             // inicializa o botão
    gpio_set_dir(gpio, GPIO_IN); // configura o botão como entrada
    gpio_pull_up(gpio);          // habilita o pull up interno
}