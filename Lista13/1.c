#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <avr/pgmspace.h>
#include <stdint.h>
#include <stdio.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <math.h>

//Michał Łukasik

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

// inicjalizacja UART
void uart_init()
{
    // ustaw baudrate
    UBRR0 = UBRR_VALUE;
    // wyczyść rejestr UCSR0A
    UCSR0A = 0;
    // włącz odbiornik i nadajnik
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
    // ustaw format 8n1
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

// transmisja jednego znaku
int uart_transmit(char data, FILE *stream)
{
    // czekaj aż transmiter gotowy
    while(!(UCSR0A & _BV(UDRE0)));
    UDR0 = data;
    return 0;
}

// odczyt jednego znaku
int uart_receive(FILE *stream)
{
    // czekaj aż znak dostępny
    while (!(UCSR0A & _BV(RXC0)));
    return UDR0;
}


void adc_init()
{
    ADMUX   = _BV(REFS0);// referencja Vcc, wejście ADC0
    ADMUX   |= 0b0001;
    DIDR0   = _BV(ADC0D) | _BV(ADC1D) | _BV(ADC2D); // wyłącz wejście cyfrowe na ADC0, ADC1, ADC2
    // częstotliwość zegara ADC 125 kHz (16 MHz / 128)
    ADCSRA  = _BV(ADPS0) | _BV(ADPS1) | _BV(ADPS2); // preskaler 128
    ADCSRA |= _BV(ADEN); // włącz ADC
    ADCSRA |= _BV(ADIE);
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

FILE uart_file;

//-------------------------------------------------------------------------------------------------------------------------------------------------------

#define SHIFT 1
#define SIZE 1<<SHIFT

volatile int16_t read = 0;

volatile int16_t buf[SIZE];

volatile uint8_t index = 0;

ISR(TIMER1_COMPA_vect)
{
    ADCSRA |= _BV(ADSC);
}

ISR(ADC_vect)
{
    read -= buf[index];
    int16_t newvalue = ADC;
    newvalue -= 512;
    buf[index] = newvalue*newvalue;
    read += buf[index++];
    if(index >= SIZE) index = 0;
}

void timer_init()
{
    TCCR1B = _BV(CS11) | _BV(WGM12);
    OCR1A = 249;
    //OCR1A = 249;
    TIMSK1 = _BV(OCIE1A);
}


int main()
{
    timer_init();
    uart_init();
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    adc_init();
    sei();
    //DDRB |= _BV(PB5);
    while(1) {
        printf("\r%5d",(int16_t)sqrt(read>>SHIFT));
        //PORTB ^= _BV(PB5);
    }
}
