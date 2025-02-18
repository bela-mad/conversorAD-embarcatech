#include <stdio.h>
#include <stdlib.h>
#include "hardware/adc.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "pico/stdlib.h"
#include "./include/button.h"
#include "./include/font.h"
#include "./include/joystick.h"
#include "./include/led-rgb.h"
#include "./include/ssd1306.h"

// Defines
#define I2C_PORT i2c1
#define I2C_SDA 14
#define I2C_SCL 15
#define endereco 0x3C
#define SQUARE_SIZE 8
#define DISPLAY_WIDTH 128
#define DISPLAY_HEIGHT 64

// Declaração de variáveis
uint16_t led_b_level = 0, led_r_level = 0; // inicialização dos níveis de PWM para os LEDs
uint slice_led_b, slice_led_r;             // variáveis para armazenar os slices de PWM correspondentes aos LEDs
absolute_time_t debounce;
uint32_t last_time = 0;                              // variável para contar o último tempo verificado
volatile bool state_led = true, state_border = true; // variáveis de estado do led e da borda
ssd1306_t ssd;
uint16_t vrx_value, vry_value, sw_value; // variáveis
int square_x = 64;                       // centraliza quadrado no eixo x
int square_y = 32;                       // centraliza quadrado no eixo y

// Cabeçalho da função de callback do botão
void button_irq_handler(uint gpio, uint32_t events);
void draw_moving_square(ssd1306_t *ssd, int x, int y);
void update_square_position(int *square_x, int *square_y, uint16_t vrx_value, uint16_t vry_value);

// Função principal
int main()
{
    stdio_init_all();

    // Inicialização dos pinos
    button_init(SW);
    button_init(BUTTON_A_PIN);
    setup_joystick();
    pwm_led_setup(BLUE_LED_PIN, &slice_led_b, led_b_level, state_led); // configura o PWM para o LED azul
    pwm_led_setup(RED_LED_PIN, &slice_led_r, led_r_level, state_led);  // configura o PWM para o LED vermelho
    led_init(GREEN_LED_PIN);

    // Interrupções dos botões A e SW
    gpio_set_irq_enabled_with_callback(SW, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &button_irq_handler);

    debounce = delayed_by_ms(get_absolute_time(), 200); // inicializa o debounce

    i2c_init(I2C_PORT, 400 * 1000); // inicialização do I2C, usando-o em 400KHz

    gpio_set_function(I2C_SDA, GPIO_FUNC_I2C);                    // set the GPIO pin function to I2C
    gpio_set_function(I2C_SCL, GPIO_FUNC_I2C);                    // set the GPIO pin function to I2C
    gpio_pull_up(I2C_SDA);                                        // pull up the data line
    gpio_pull_up(I2C_SCL);                                        // pull up the clock line
    ssd1306_init(&ssd, WIDTH, HEIGHT, false, endereco, I2C_PORT); // inicializa o display
    ssd1306_config(&ssd);                                         // configura o display
    ssd1306_send_data(&ssd);                                      // envia os dados para o display

    // Limpa o display, desenha a borda e envia os dados para serem exibidos
    ssd1306_fill(&ssd, false);
    ssd1306_rect(&ssd, 3, 3, 122, 58, true);
    ssd1306_send_data(&ssd);

    while (true)
    {
        joystick_read_axis(&vrx_value, &vry_value); // lê os valores dos eixos do joystick

        if ((vrx_value >= 2400 || vrx_value <= 2120) && state_led)
        {
            led_r_level = abs(vrx_value - 2047) * 2;
        }
        else
        {
            led_r_level = 0;
        }

        if ((vry_value >= 2100 || vry_value <= 1850) && state_led)
        {
            led_b_level = abs(vry_value - 2047) * 2;
        }
        else
        {
            led_b_level = 0;
        }

        // Ajusta os níveis PWM dos LEDs de acordo com os valores do joystick
        pwm_set_gpio_level(RED_LED_PIN, led_r_level);
        pwm_set_gpio_level(BLUE_LED_PIN, led_b_level);

        ssd1306_fill(&ssd, false); // limpa o display

        // Atualiza a posição do quadrado com base nos valores do joystick
        update_square_position(&square_x, &square_y, vrx_value, vry_value);

        if (state_border) 
        {
            ssd1306_rect(&ssd, 3, 3, 122, 58, true);
        }
        else
        {
            ssd1306_rect_hearts(&ssd, 3, 3, 122, 58, true);
        }

        // desenha o quadrado de 8x8 pixels
        ssd1306_rect(&ssd, square_x - 4, square_y - 8, SQUARE_SIZE, SQUARE_SIZE, true);

        ssd1306_send_data(&ssd);

        // Pequeno delay antes da próxima leitura
        sleep_ms(40);
    }
}

// Função que trata a interrupção do botão
void button_irq_handler(uint gpio, uint32_t events)
{
    if (time_reached(debounce)) // verifica se o tempo de debounce já passou
    {
        if (gpio == SW)
        {
            state_border = !state_border;
            gpio_put(GREEN_LED_PIN, !gpio_get(GREEN_LED_PIN)); // alterna o LED verde
        }
        else if (gpio == BUTTON_A_PIN)
        {
            state_led = !state_led;
        }
        debounce = delayed_by_ms(get_absolute_time(), 200); // atualiza o debounce
    }
}

void draw_moving_square(ssd1306_t *ssd, int x, int y)
{
    ssd1306_rect(ssd, x - 4, y - 8, SQUARE_SIZE, SQUARE_SIZE, true);

    // Atualiza o display
    // ssd1306_send_data(ssd);
}

void update_square_position(int *square_x, int *square_y, uint16_t vrx_value, uint16_t vry_value)
{
    // Calcula a nova posição com base nos valores do joystick
    int delta_y = vrx_value / 32;          // Ajuste da sensibilidade
    int delta_x = (4095 - vry_value) / 64; // Ajuste da sensibilidade

    // Atualiza as coordenadas do quadrado
    *square_x = delta_x;
    *square_y = delta_y;

    // Limita o movimento do quadrado dentro da área da borda (3, 3, 122, 58)
    if (*square_x < 8)
    {
        *square_x = 8;
    }
    if (*square_x > 56)
    { // 122 - 8 (tamanho do quadrado)
        *square_x = 56;
    }
    if (*square_y < 12)
    {
        *square_y = 12;
    }
    if (*square_y > 124)
    { // 58 - 8 (tamanho do quadrado)
        *square_y = 124;
    }
}
