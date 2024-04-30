#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <inttypes.h>

//Michał Łukasik

int main()
{
  UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
  // program testowy
  DDRD=0xFF;
  while(1) {
    uint8_t a=0b10000000;

        for(uint8_t i=0;i<8;i++){
            PORTD= a;
            _delay_ms(1000);
            a >>= 1;
        }
  }
}

