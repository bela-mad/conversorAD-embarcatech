#include "ssd1306.h"

// Funções

/**
 * @brief Inicializa o display SSD1306.
 * 
 * Configura as dimensões, endereço I2C e cria os buffers necessários para envio de dados ao display.
 *
 * @param ssd Ponteiro para a estrutura do display.
 * @param width Largura do display em pixels.
 * @param height Altura do display em pixels.
 * @param external_vcc Indica se o display usa alimentação externa (true) ou interna (false).
 * @param address Endereço I2C do display.
 * @param i2c Ponteiro para a instância I2C utilizada.
 */
void ssd1306_init(ssd1306_t *ssd, uint8_t width, uint8_t height, bool external_vcc, uint8_t address, i2c_inst_t *i2c) {
    ssd->width = width;
    ssd->height = height;
    ssd->pages = height / 8U;
    ssd->address = address;
    ssd->i2c_port = i2c;
    ssd->bufsize = ssd->pages * ssd->width + 1;
    ssd->ram_buffer = calloc(ssd->bufsize, sizeof(uint8_t));
    ssd->ram_buffer[0] = 0x40;
    ssd->port_buffer[0] = 0x80;
}

/**
 * @brief Configura o display SSD1306 com os parâmetros necessários.
 *
 * Envia uma sequência de comandos para configurar a memória de exibição, contraste, 
 * orientação e outras configurações essenciais para funcionamento adequado.
 *
 * @param ssd Ponteiro para a estrutura do display.
 */
void ssd1306_config(ssd1306_t *ssd) {
    ssd1306_command(ssd, SET_DISP | 0x00);
    ssd1306_command(ssd, SET_MEM_ADDR);
    ssd1306_command(ssd, 0x01);
    ssd1306_command(ssd, SET_DISP_START_LINE | 0x00);
    ssd1306_command(ssd, SET_SEG_REMAP | 0x01);
    ssd1306_command(ssd, SET_MUX_RATIO);
    ssd1306_command(ssd, HEIGHT - 1);
    ssd1306_command(ssd, SET_COM_OUT_DIR | 0x08);
    ssd1306_command(ssd, SET_DISP_OFFSET);
    ssd1306_command(ssd, 0x00);
    ssd1306_command(ssd, SET_COM_PIN_CFG);
    ssd1306_command(ssd, 0x12);
    ssd1306_command(ssd, SET_DISP_CLK_DIV);
    ssd1306_command(ssd, 0x80);
    ssd1306_command(ssd, SET_PRECHARGE);
    ssd1306_command(ssd, 0xF1);
    ssd1306_command(ssd, SET_VCOM_DESEL);
    ssd1306_command(ssd, 0x30);
    ssd1306_command(ssd, SET_CONTRAST);
    ssd1306_command(ssd, 0xFF);
    ssd1306_command(ssd, SET_ENTIRE_ON);
    ssd1306_command(ssd, SET_NORM_INV);
    ssd1306_command(ssd, SET_CHARGE_PUMP);
    ssd1306_command(ssd, 0x14);
    ssd1306_command(ssd, SET_DISP | 0x01);
}

/**
 * @brief Envia um comando ao display SSD1306 via I2C.
 *
 * @param ssd Ponteiro para a estrutura do display.
 * @param command Comando a ser enviado.
 */
void ssd1306_command(ssd1306_t *ssd, uint8_t command) {
    ssd->port_buffer[1] = command;
    i2c_write_blocking(
        ssd->i2c_port,
        ssd->address,
        ssd->port_buffer,
        2,
        false);
}

/**
 * @brief Envia os dados do buffer RAM para o display SSD1306.
 *
 * Atualiza o display enviando os dados armazenados no buffer interno via I2C.
 *
 * @param ssd Ponteiro para a estrutura do display.
 */
void ssd1306_send_data(ssd1306_t *ssd) {
    ssd1306_command(ssd, SET_COL_ADDR);
    ssd1306_command(ssd, 0);
    ssd1306_command(ssd, ssd->width - 1);
    ssd1306_command(ssd, SET_PAGE_ADDR);
    ssd1306_command(ssd, 0);
    ssd1306_command(ssd, ssd->pages - 1);
    i2c_write_blocking(
        ssd->i2c_port,
        ssd->address,
        ssd->ram_buffer,
        ssd->bufsize,
        false);
}

/**
 * @brief Define o estado de um pixel no buffer do display.
 *
 * @param ssd Ponteiro para a estrutura do display.
 * @param x Coordenada X do pixel.
 * @param y Coordenada Y do pixel.
 * @param value Estado do pixel (true para ligado, false para desligado).
 */
void ssd1306_pixel(ssd1306_t *ssd, uint8_t x, uint8_t y, bool value) {
    uint16_t index = (y >> 3) + (x << 3) + 1;
    uint8_t pixel = (y & 0b111);
    if (value)
        ssd->ram_buffer[index] |= (1 << pixel);
    else
        ssd->ram_buffer[index] &= ~(1 << pixel);
}

/**
 * @brief Preenche todo o display com um único valor (ligado ou desligado).
 *
 * @param ssd Ponteiro para a estrutura do display.
 * @param value Estado para preencher o display (true para ligado, false para desligado).
 */
void ssd1306_fill(ssd1306_t *ssd, bool value) {
    // itera por todas as posições do display
    for (uint8_t y = 0; y < ssd->height; ++y) {
        for (uint8_t x = 0; x < ssd->width; ++x) {
            ssd1306_pixel(ssd, x, y, value);
        }
    }
}

/**
 * @brief Desenha um retângulo no display.
 *
 * @param ssd Ponteiro para a estrutura do display.
 * @param top Coordenada Y do canto superior do retângulo.
 * @param left Coordenada X do canto esquerdo do retângulo.
 * @param width Largura do retângulo.
 * @param height Altura do retângulo.
 * @param value Estado do pixel (true para ligado, false para desligado).
 * @param fill Se true, preenche o retângulo; se false, desenha apenas as bordas.
 */
void ssd1306_rect(ssd1306_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value) {
    // Desenha a borda superior e inferior
    for (uint8_t x = left; x < left + width; ++x) {
        ssd1306_pixel(ssd, x, top, value);                    // Linha superior
        ssd1306_pixel(ssd, x, top + height - 1, value);       // Linha inferior
    }

    // Desenha a borda esquerda e direita
    for (uint8_t y = top; y < top + height; ++y) {
        ssd1306_pixel(ssd, left, y, value);                   // Linha esquerda
        ssd1306_pixel(ssd, left + width - 1, y, value);       // Linha direita
    }
}

// Função que desenha uma borda de corações
void ssd1306_rect_hearts(ssd1306_t *ssd, uint8_t top, uint8_t left, uint8_t width, uint8_t height, bool value) {
    const uint8_t spacing = 5; // Espaçamento entre os corações

    // Desenha a borda superior e inferior
    for (uint8_t x = left; x < left + width; x += spacing) {
        ssd1306_draw_heart(ssd, x, top, value);                    // Linha superior
        ssd1306_draw_heart(ssd, x, top + height - 1, value);       // Linha inferior
    }

    // Desenha a borda esquerda e direita
    for (uint8_t y = top; y < top + height; y += spacing) {
        ssd1306_draw_heart(ssd, left, y, value);                   // Linha esquerda
        ssd1306_draw_heart(ssd, left + width - 1, y, value);       // Linha direita
    }
}

// Função que desenha um coração 
void ssd1306_draw_heart(ssd1306_t *ssd, uint8_t x, uint8_t y, bool value) {
    /*
       Padrão do coração:
       . # . # .
       # # # # #
       # # # # #
       . # # # .
       . . # . .
    */
    ssd1306_pixel(ssd, x - 1, y, value);
    ssd1306_pixel(ssd, x + 1, y, value);
    
    ssd1306_pixel(ssd, x - 2, y + 1, value);
    ssd1306_pixel(ssd, x - 1, y + 1, value);
    ssd1306_pixel(ssd, x, y + 1, value);
    ssd1306_pixel(ssd, x + 1, y + 1, value);
    ssd1306_pixel(ssd, x + 2, y + 1, value);

    ssd1306_pixel(ssd, x - 2, y + 2, value);
    ssd1306_pixel(ssd, x - 1, y + 2, value);
    ssd1306_pixel(ssd, x, y + 2, value);
    ssd1306_pixel(ssd, x + 1, y + 2, value);
    ssd1306_pixel(ssd, x + 2, y + 2, value);

    ssd1306_pixel(ssd, x - 1, y + 3, value);
    ssd1306_pixel(ssd, x, y + 3, value);
    ssd1306_pixel(ssd, x + 1, y + 3, value);

    ssd1306_pixel(ssd, x, y + 4, value);
}


/**
 * @brief Desenha uma linha entre dois pontos no display.
 *
 * @param ssd Ponteiro para a estrutura do display.
 * @param x0 Coordenada X do ponto inicial.
 * @param y0 Coordenada Y do ponto inicial.
 * @param x1 Coordenada X do ponto final.
 * @param y1 Coordenada Y do ponto final.
 * @param value Estado do pixel (true para ligado, false para desligado).
 */
void ssd1306_line(ssd1306_t *ssd, uint8_t x0, uint8_t y0, uint8_t x1, uint8_t y1, bool value) {
    int dx = abs(x1 - x0);
    int dy = abs(y1 - y0);

    int sx = (x0 < x1) ? 1 : -1;
    int sy = (y0 < y1) ? 1 : -1;

    int err = dx - dy;

    while (true) {
        ssd1306_pixel(ssd, x0, y0, value); // Desenha o pixel atual

        if (x0 == x1 && y0 == y1)
            break; // Termina quando alcança o ponto final

        int e2 = err * 2;

        if (e2 > -dy) {
            err -= dy;
            x0 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y0 += sy;
        }
    }
}

/**
 * @brief Desenha uma linha horizontal no display.
 *
 * @param ssd Ponteiro para a estrutura do display.
 * @param x0 Coordenada X inicial.
 * @param x1 Coordenada X final.
 * @param y Coordenada Y da linha.
 * @param value Estado do pixel (true para ligado, false para desligado).
 */
void ssd1306_hline(ssd1306_t *ssd, uint8_t x0, uint8_t x1, uint8_t y, bool value) {
    for (uint8_t x = x0; x <= x1; ++x)
        ssd1306_pixel(ssd, x, y, value);
}

/**
 * @brief Desenha uma linha vertical no display.
 *
 * @param ssd Ponteiro para a estrutura do display.
 * @param x Coordenada X da linha.
 * @param y0 Coordenada Y inicial.
 * @param y1 Coordenada Y final.
 * @param value Estado do pixel (true para ligado, false para desligado).
 */
void ssd1306_vline(ssd1306_t *ssd, uint8_t x, uint8_t y0, uint8_t y1, bool value) {
    for (uint8_t y = y0; y <= y1; ++y)
        ssd1306_pixel(ssd, x, y, value);
}

/**
 * @brief Desenha uma string no display.
 *
 * @param ssd Ponteiro para a estrutura do display.
 * @param str String a ser desenhada.
 * @param x Coordenada X inicial da string.
 * @param y Coordenada Y inicial da string.
 */
void ssd1306_draw_string(ssd1306_t *ssd, const char *str, uint8_t x, uint8_t y) {
    while (*str) {
        ssd1306_draw_char(ssd, *str++, x, y);
        x += 8;
        if (x + 8 >= ssd->width) {
            x = 0;
            y += 8;
        }
        if (y + 8 >= ssd->height) {
            break;
        }
    }
}


// Função que atualiza a posição do quadrado 8x8 de acordo com a posição de x e y do joystick
void update_square_position(int *square_x, int *square_y, uint16_t vrx_value, uint16_t vry_value) {
    // Calcula a nova posição com base nos valores do joystick
    int delta_y = vrx_value / 32;
    int delta_x = (4095 - vry_value) / 64;

    // Atualiza as coordenadas do quadrado
    *square_x = delta_x;
    *square_y = delta_y;

    // Limite do movimento do quadrado dentro da área da borda
    if (*square_x < 8) {   // limite superior
        *square_x = 8;
    }
    if (*square_x > 56) {  // limite inferior
        *square_x = 56;
    }
    if (*square_y < 12) {  // limite da esquerda
        *square_y = 12;
    }
    if (*square_y > 124) { // limite da direita
        *square_y = 124;
    }
}
