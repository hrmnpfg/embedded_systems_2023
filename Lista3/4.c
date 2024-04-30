#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>

//Michał Łukasik

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

void uart_init()
{
  UBRR0 = UBRR_VALUE;
  UCSR0B = _BV(RXEN0) | _BV(TXEN0);
  UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

int uart_transmit(char data, FILE *stream)
{
  while(!(UCSR0A & _BV(UDRE0)));
  UDR0 = data;
  return 0;
}

int uart_receive(FILE *stream)
{
  while (!(UCSR0A & _BV(RXC0)));
  return UDR0;
}

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

FILE uart_file;

int main(void)
{
    adc_init();
    uart_init();
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    while(1)
    {
		float vccx10 = readVoltage();
        float resistance = 10230000.0/vccx10 - 10000.0; // przekształcony wzór, użyty opornik 10kOhm
        float R0 = 4700.0;
        float B= 2675.2;
        float T0= 298.15;
        float T = (R0*T0)/(T0*log((resistance/R0))+B);
        int t2 = (int)(T-273.15);
        int resistance2 = (int)resistance;
        printf("Temperatura: %d.%d, opór: %d\n\r",t2/10,t2%10, resistance2);
    }
}
