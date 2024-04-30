#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <inttypes.h>

//Michał Łukasik

#define s9 144
#define s8 128
#define s7 248
#define s6 130
#define s5 146
#define s4 153
#define s3 176
#define s2 164
#define s1 249
#define s0 192

uint8_t nums[10] = {s0,s1,s2,s3,s4,s5,s6,s7,s8,s9};

int main()
{
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    DDRD=0xFF;
    DDRC=0xFF;
    while(1)
    {
        for (uint8_t j = 0; j < 6; j++)
        {
            for (uint8_t i = 0; i < 10; i++)
            {
                for(int16_t t = 0; t<500; t++)
                {
                    PORTD = nums[i];
                    PORTC = 1;
                    _delay_ms(1);
                    PORTC = 0;
                    PORTD = nums[j];
                    PORTC = 2;
                    _delay_ms(1);
                    PORTC = 0;
                }
            }
        }
    }
}

