#include <avr/pgmspace.h>
#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdint.h>
#include <util/delay.h>

//Michał Łukasik

const uint16_t exp_table[128] PROGMEM = {1,1,1,1,1,1,2,2,2,2,2,2,3,3,3,3,3,4,4,4,5,5,6,6,7,7,8,8,9,10,10,11,12,13,14,15,17,18,19,21,23,25,27,29,31,34,36,39,43,46,50,54,58,63,68,73,79,86,93,100,109,117,127,137,148,160,174,188,203,219,237,256,277,300,324,351,379,410,443,479,518,560,606,655,708,766,828,895,968,1046,1131,1223,1323,1430,1547,1672,1808,1955,2114,2286,2471,2672,2889,3124,3378,3652,3949,4270,4617,4992,5398,5836,6311,6823,7378,7977,8626,9327,10084,10904,11790,12748,13784,14904,16115,17424,18840,20371};

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

	return adc ;

}

void timer1_init()
{
  // ustaw tryb licznika
  // COM1A = 10   -- non-inverting mode
  // WGM1  = 1110 -- fast PWM top=ICR1
  // CS1   = 001  -- brak prescalera
  ICR1 = 1023;
  TCCR1A = _BV(COM1A1) | _BV(WGM11);
  TCCR1B = _BV(WGM12) | _BV(WGM13) | _BV(CS10);
  // ustaw pin OC1A (PB1) jako wyjście
  DDRB |= _BV(PB1);
}


int main(void)
{
    adc_init();
    timer1_init();
    while(1)
    {
		uint16_t vccx10 = readVoltage();
        OCR1A = ICR1/pgm_read_byte(&exp_table[vccx10>>3]);
        _delay_ms(50);
    }
}
