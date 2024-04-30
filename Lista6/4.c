#include <avr/io.h>
#include <util/delay.h>

//Michał Łukasik

inline void spi_init()
{
  SPCR  = _BV(SPE) | _BV(MSTR) | _BV(SPR1) | _BV(SPR0) | _BV(DORD);
  DDRB |= _BV(PB1) | _BV(PB2) | _BV(PB3) | _BV(PB5);
}

inline void load(const uint8_t data)
{
    PORTB &= ~_BV(PB2);
    PORTB &= ~_BV(PB1);

    SPDR = data;
    while (!(SPSR & _BV(SPIF)));
    SPSR &= ~_BV(SPIF);

    PORTB |=  _BV(PB1);
    PORTB &= ~_BV(PB2);
}

uint8_t digits[] = {
  0b11111100,
  0b01100000,
  0b11011010,
  0b11110010,
  0b01100110,
  0b10110110,
  0b10111110,
  0b11100000,
  0b11111110,
  0b11110110,
};

int main()
{
  spi_init();

  uint8_t i = 0;
  while (1)
  {
    load(digits[i]);
    _delay_ms(1000);
    if (++i == 10) i >>= 4;
  }
}
