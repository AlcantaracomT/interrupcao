#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "ws2812.pio.h"
#include "pico/bootrom.h"

#define IS_RGBW false
#define NUM_PIXELS 25
#define LED_RED 13
#define WS2812_PIN 7

const uint botao1 = 5;
const uint botao2 = 6;
int numeral = 0;

const uint red_pin = 13;   
const uint green_pin = 11; 
const uint blue_pin = 12; 

// Variável global para a cor
uint8_t led_r = 10;
uint8_t led_g = 0;
uint8_t led_b = 10;

// LEDs 1 estão ligados na matriz
bool led_buffer[NUM_PIXELS];

// Variáveis para debouncing
static absolute_time_t last_interrupt_time_botao1 = 0;
static absolute_time_t last_interrupt_time_botao2 = 0;
static const uint64_t debounce_time_ms = 200; 


static void interrupcao(uint gpio, uint32_t events);
void pisca();
void set_one_led(uint8_t r, uint8_t g, uint8_t b, double inten);
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b);
void contador(int contar);




int main()
{
    stdio_init_all();
    PIO pio = pio0;
    int sm = 0;
    uint offset = pio_add_program(pio, &ws2812_program);

    ws2812_program_init(pio, sm, offset, WS2812_PIN, 800000, IS_RGBW);
    set_one_led(led_r, led_g, led_b, 0.1);

    //entrada e saida
    gpio_init(red_pin);
    gpio_init(green_pin);
    gpio_init(blue_pin);

    gpio_set_dir(red_pin, GPIO_OUT);
    gpio_set_dir(green_pin, GPIO_OUT);
    gpio_set_dir(blue_pin, GPIO_OUT);



    gpio_init(botao1);
    gpio_init(botao2);

    gpio_pull_up(botao1);
    gpio_pull_up(botao2);


    gpio_set_dir(botao1, GPIO_IN);
    gpio_set_dir(botao2, GPIO_IN);

    gpio_set_irq_enabled_with_callback(botao1, GPIO_IRQ_EDGE_FALL, true, &interrupcao);
    gpio_set_irq_enabled_with_callback(botao2, GPIO_IRQ_EDGE_FALL, true, &interrupcao);


   
    while (1)
    {
        pisca();
        sleep_ms(10);
    }
    return 0;
}

//configurar o estado dos leds
void set_led_color(uint red_pin, uint green_pin, uint blue_pin, bool R, bool G, bool B) {
    gpio_put(red_pin, R);   
    gpio_put(green_pin, G); 
    gpio_put(blue_pin, B); 
}

// Enviar dados para o LED
static inline void put_pixel(uint32_t pixel_grb)
{
    pio_sm_put_blocking(pio0, 0, pixel_grb << 8u);
}

// Reorganizar RGB para GRB no WS2812
static inline uint32_t urgb_u32(uint8_t r, uint8_t g, uint8_t b)
{
    return ((uint32_t)(r) << 8) | ((uint32_t)(g) << 16) | (uint32_t)(b);
}

// Percorre o buffer e marca 1 ligado e 0 desligado
void set_one_led(uint8_t r, uint8_t g, uint8_t b, double inten)
{
    // Intensidade 
    r *= inten;
    g *= inten;
    b *= inten;

    // Define a cor com base nos parâmetros fornecidos
    uint32_t color = urgb_u32(r, g, b);

    for (int i = 0; i < NUM_PIXELS; i++)
    {
        if (led_buffer[i])
        {
            put_pixel(color); 
        }
        else
        {
            put_pixel(0); 
        }
    }
}

// Função única para tratar ambos os botões
static void interrupcao(uint gpio, uint32_t events) {
    absolute_time_t current_time = get_absolute_time();

    // Verifica qual botão foi pressionado
    if (gpio == botao1) { // Botão 1 (Incrementa)
        if (absolute_time_diff_us(last_interrupt_time_botao1, current_time) < debounce_time_ms * 1000) {
            return;
        }
        last_interrupt_time_botao1 = current_time;

        numeral = (numeral + 1) % 10;
        printf("Botão 1 pressionado. Numeral: %d\n", numeral);
    } 
    else if (gpio == botao2) { // Botão 2 (Decrementa)
        if (absolute_time_diff_us(last_interrupt_time_botao2, current_time) < debounce_time_ms * 1000) {
            return;
        }
        last_interrupt_time_botao2 = current_time;

        numeral = (numeral - 1 + 10) % 10;
        printf("Botão 2 pressionado. Numeral: %d\n", numeral);
    }

    // Atualiza a matriz de LEDs
    contador(numeral);
    set_one_led(led_r, led_g, led_b, 0.5);
}


//piscar o led em 5 vezes por segundo
void pisca() {
    static bool estado = false;
    static absolute_time_t ultimo_tempo = {0};
    const uint64_t intervalo = 100 * 1000;

    if (absolute_time_diff_us(ultimo_tempo, get_absolute_time()) > intervalo) {
        estado = !estado;
        gpio_put(red_pin, estado);
        ultimo_tempo = get_absolute_time();
    }
}



//função que printa os numeros na matriz de led
void contador(int contar)
{
    bool numeros[10][NUM_PIXELS] = {
        {   0, 1, 1, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 1, 1, 0}, // 0

        {   0, 0, 1, 0, 0, 
            0, 0, 1, 0, 0, 
            0, 0, 1, 0, 0, 
            0, 1, 1, 0, 0, 
            0, 0, 1, 0, 0}, // 1

        {   0, 1, 1, 1, 0, 
            0, 1, 0, 0, 0, 
            0, 1, 1, 1, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 1, 1, 0},  // 2

        {   0, 1, 1, 1, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 1, 1, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 1, 1, 0}, //3

        {   0, 1, 0, 0, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 1, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 0, 1, 0}, //4

        {   0, 1, 1, 1, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 1, 1, 0, 
            0, 1, 0, 0, 0, 
            0, 1, 1, 1, 0}, //5
        
        {   0, 1, 1, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 1, 1, 0, 
            0, 1, 0, 0, 0, 
            0, 1, 1, 1, 0}, //6

        {   0, 1, 0, 0, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 0, 0, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 1, 1, 0}, //7

        {   0, 1, 1, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 1, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 1, 1, 0}, //8

        {   0, 1, 0, 0, 0, 
            0, 0, 0, 1, 0, 
            0, 1, 1, 1, 0, 
            0, 1, 0, 1, 0, 
            0, 1, 1, 1, 0} //9   
    };

    memcpy(led_buffer, numeros[contar], NUM_PIXELS * sizeof(bool));//copia no array global
    set_one_led(led_r, led_g, led_b, 0.5);
}
