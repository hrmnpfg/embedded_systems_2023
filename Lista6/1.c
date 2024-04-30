#include <avr/io.h>
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

void interrupt_init()
{
    UCSR0B |= _BV(RXCIE0);
}

ISR(USART_RX_vect){
    UDR0 = UDR0;
}

int main(void)
{
    uart_init();
    interrupt_init();

    sei();
    set_sleep_mode(SLEEP_MODE_IDLE);
    while(1)
    {
        sleep_mode();
	}
}
