#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

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
  ADMUX  = _BV(REFS0); // referencja AVcc, wejście ADC0
  ADMUX   |= 0b1110; // wybieramy 1.1V
  //DIDR0   = _BV(ADC0D); // wyłącz wejście cyfrowe na ADC0
  // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
  ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
  ADCSRA |= _BV(ADEN); // włącz ADC
  ADCSRA |= _BV(ADIE); // włącza przerwanie ADC
}


volatile uint32_t vcc10;

void readVoltage(void) {
	ADCSRA |= _BV(ADSC);				// Start a conversion
	while( ADCSRA & _BV( ADSC) ) ;
	uint8_t low  = ADCL;
	uint8_t high = ADCH;
	uint16_t adc = (high << 8) | low;		// 0<= result <=1023
	vcc10 = (uint32_t) ( (11 * 10240000) / adc);
}

FILE uart_file;

ISR(ADC_vect)
{
    uint16_t adc = ADC;
    vcc10 = (uint32_t) ( (11 * 10240000) / adc);
}

int main(void)
{
    adc_init();
	DDRB |= _BV(PORTB5);
    uart_init();
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    set_sleep_mode(SLEEP_MODE_ADC);
    while(1)
    {
		readVoltage();
        printf("Z czekaniem: %lu\r\n",vcc10);
        _delay_ms(1000);

        sei();
        _delay_ms(1000);
        ADCSRA |= _BV(ADSC);
        sleep_mode();
        printf("Z przerwaniem: %lu\r\n",vcc10);
        cli();
        _delay_ms(1000);
	}
}
