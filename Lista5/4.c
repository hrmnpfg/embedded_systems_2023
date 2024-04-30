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

volatile uint32_t vcc10;

FILE uart_file;

void timer_init()
{
    TCCR1B = _BV(CS12) | _BV(CS10);
}

volatile uint16_t last = 0;

ISR(TIMER1_CAPT_vect)
{
    uint16_t temp = ICR1;
    uint16_t l = temp - last;
    //printf("%u\r\n",15625/l);
    printf("%u\r\n",l);
    last = temp;
}

void interrupt_init()
{
    TIMSK1 = _BV(ICIE1);
}

int main(void)
{
    timer_init();
    uart_init();
    interrupt_init();
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;
    sei();
    set_sleep_mode(SLEEP_MODE_IDLE);
    while(1)
    {
        sleep_mode();
	}
}
