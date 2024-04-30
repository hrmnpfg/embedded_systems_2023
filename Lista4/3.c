#include <avr/pgmspace.h>
#include <avr/io.h>
#include <stdint.h>
#include <stdlib.h>
#include <util/delay.h>
#include <math.h>

//Michał Łukasik

const uint16_t exp_table[64] = {
  1,1,1,2,2,2,3,3,3,4,5,6,7,8,9,10,12,14,17,19,23,27,31,36,43,50,58,68,79,93,109,127,148,174,203,237,277,324,379,443,518,606,708,828,968,1131,1323,1547,1808,2114,2471,2889,3378,3949,4617,5398,6311,7378,8626,10084,11790,13784,16115,18840
};

const uint8_t sine_table[128] = {
    0,1,3,4,6,7,9,10,12,14,15,17,18,20,21,23,24,25,27,28,30,31,32,34,35,36,38,39,40,41,42,44,45,46,47,48,49,50,51,52,53,54,54,55,56,57,57,58,59,59,60,60,61,61,62,62,62,63,63,63,63,63,63,63,63,63,63,63,63,63,63,63,62,62,62,61,61,60,60,59,59,58,57,57,56,55,54,54,53,52,51,50,49,48,47,46,45,44,42,41,40,39,38,36,35,34,32,31,30,28,27,25,24,23,21,20,18,17,15,14,12,10,9,7,6,4,3,1
};

#define BLUE OCR0A
#define RED OCR1A
#define GREEN OCR2A

void timer_init()
{
    // Ustaw tryb licznika 0
    TCCR0A = _BV(COM0A1) | _BV(WGM01) | _BV(WGM00); // Tryb Fast PWM
    TCCR0B = _BV(CS00); // Brak prescalera

    // Ustaw tryb licznika 1
    ICR1 = 255;
    TCCR1A = _BV(COM1A1) | _BV(WGM11); // Tryb Fast PWM
    TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10); // Brak prescalera

    // Ustaw tryb licznika 2
    TCCR2A = _BV(COM2A1) | _BV(WGM01) | _BV(WGM00); // Tryb Fast PWM
    TCCR2B = _BV(CS00); // Brak prescalera

    // Ustaw piny wyjściowe jako wyjścia PWM
    DDRD |= _BV(PD6);  // OC0A (Timer 0)
    DDRB |= _BV(PB1);  // OC1A (Timer 1)
    DDRB |= _BV(PB3);  // OC2A (Timer 2)
}

uint32_t HsvToRgb(uint32_t hue) {
    int c = 255;
    int x = (c * (255 - abs((hue / 60) % 2 * 255))) / 255;
    int r, g, b;
    if (hue >= 0 && hue < 60) {
        r = c;
        g = x;
        b = 0;
    } else if (hue >= 60 && hue < 120) {
        r = x;
        g = c;
        b = 0;
    } else if (hue >= 120 && hue < 180) {
        r = 0;
        g = c;
        b = x;
    } else if (hue >= 180 && hue < 240) {
        r = 0;
        g = x;
        b = c;
    } else if (hue >= 240 && hue < 300) {
        r = x;
        g = 0;
        b = c;
    } else {
        r = c;
        g = 0;
        b = x;
    }

    return (uint32_t)(((uint32_t)r << 16) | (g << 8) | b);
}

uint8_t brightness(uint8_t color, uint8_t index)
{
    uint32_t a = (uint32_t)color * exp_table[index] / 18840;
    return 255 - (uint8_t)a;
}

void breathe(uint32_t color)
{
    for(uint8_t i = 0; i<128; i++)
    {
        uint8_t r = brightness((uint8_t)(color >> 16), sine_table[i]);
        uint8_t g = brightness((uint8_t)(color >> 8), sine_table[i]);
        uint8_t b = brightness((uint8_t)color, sine_table[i]);
        BLUE = b;
        RED = r;
        GREEN = g;
        _delay_ms(2000.0/128.0);
    }
    RED = 255;
    GREEN = 255;
    BLUE = 255;
    _delay_ms(50);
}

int main(void)
{
    timer_init();

    while(1)
    {
        uint16_t rand_number = rand() % 360;
        uint32_t color = HsvToRgb(rand_number);
        breathe(color);
    }
}
