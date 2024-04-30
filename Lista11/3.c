#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <util/delay.h>
#include <avr/interrupt.h>

//Michał Łukasik

void adc_init()
{
  ADMUX   = _BV(REFS0); // referencja AVcc, wejście ADC0
  DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
  //ADCSRA |= _BV(ADATE);
}

uint16_t readVoltage(void) {
	ADCSRA |= _BV(ADSC);
	while( ADCSRA & _BV( ADSC) ) ;
	uint8_t low  = ADCL;
	uint8_t high = ADCH;
	uint16_t adc = (high << 8) | low;		// 0<= result <=1023
	//uint8_t vccx10 = (uint8_t) ( (11 * 1024) / adc);
	return adc;
}

void timer_init()
{
    TCCR1A = _BV(WGM11) | _BV(COM1B1);
    TCCR1B = _BV(CS11)| _BV(WGM12) | _BV(WGM13);
    OCR1B = 0;
    ICR1 = 3069;
}

int main()
{
    adc_init();
    timer_init();
    DDRB |= _BV(PB2);
    DDRB |= _BV(PB3);
    DDRB |= _BV(PB4);
    while(1) {
        int16_t a = readVoltage()-512;
        if (a >= 0)
        {
            PORTB |= _BV(PB3);
            PORTB &= ~_BV(PB4);
        }
        else
        {
            a = -a;
            a -= 1;
            PORTB &= ~_BV(PB3);
            PORTB |= _BV(PB4);
        }
        OCR1B = a*6;
        _delay_ms(50);
    }
}

