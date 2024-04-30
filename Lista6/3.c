#include <avr/io.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <util/delay.h>

//Michał Łukasik

unsigned const char dzwiek_raw[] PROGMEM = {

};
unsigned int dzwiek_raw_len = 0;

void spi_init()
{
    DDRB |= _BV(DDB3) | _BV(DDB5) | _BV(DDB2);
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR1) | _BV(SPR0);
}

void spi_transfer(uint8_t data)
{
    SPDR = 0b00110000 | data >> 4;
    while (!(SPSR & _BV(SPIF)));
    SPSR |= _BV(SPIF);
    SPDR = data << 4;
    while (!(SPSR & _BV(SPIF)));
    SPSR |= _BV(SPIF);
}

int main()
{
    spi_init();
    DDRD |= _BV(PORTD4);
    uint16_t i = 0;
    while(1) {
        PORTD &= ~_BV(PORTD4);
        spi_transfer(pgm_read_byte(&dzwiek_raw[++i]));
        PORTD |= _BV(PORTD4);
        i%=dzwiek_raw_len;
    }
}

