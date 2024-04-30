#include <avr/io.h>
#include <stdint.h>
#include <avr/pgmspace.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Michał Łukasik

static unsigned const char volatile dzwiek_raw[] PROGMEM = {

};
unsigned int dzwiek_raw_len = 0;

void spi_init()
{
    DDRB |= _BV(DDB3) | _BV(DDB5) | _BV(DDB2);
    SPCR = _BV(SPE) | _BV(MSTR) | _BV(SPR0) | _BV(SPR1);
    //SPSR |= 1;
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


volatile uint16_t i = 0;
ISR(TIMER1_COMPA_vect)
{
    PORTB &= ~_BV(PB2);
    spi_transfer(pgm_read_byte(&dzwiek_raw[++i]));
    PORTB |= _BV(PB2);
    i%=dzwiek_raw_len;
}

void timer_init()
{
    TCCR1B = _BV(CS11) | _BV(WGM12);
    OCR1A = 249;
    TIMSK1 = _BV(OCIE1A);
}

int main()
{
    timer_init();
    spi_init();
    DDRB |= _BV(PB2);
    sei();
    while(1) {

    }
}

