#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sfr_defs.h>
#include <avr/sleep.h>
#include <stdio.h>

//Michał Łukasik

#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

#define BUFFER_SIZE 64
volatile char tx_buffer[BUFFER_SIZE];
volatile uint8_t tx_head = 0;
volatile uint8_t tx_tail = 0;

volatile char rx_buffer[BUFFER_SIZE];
volatile uint8_t rx_head = 0;
volatile uint8_t rx_tail = 0;

void uart_init()
{
    UBRR0 = UBRR_VALUE;
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}


ISR(USART_UDRE_vect)
{
    if (tx_head != tx_tail)
    {
        UDR0 = tx_buffer[tx_tail];
        tx_tail = (tx_tail + 1) % BUFFER_SIZE;
    }
    else
    {
        UCSR0B &= ~_BV(UDRIE0);
    }
}

ISR(USART_RX_vect)
{
    char received = UDR0;
    if (rx_head + 1 != rx_tail)
    {
        rx_buffer[rx_head] = received;
        rx_head = (rx_head + 1) % BUFFER_SIZE;
    }
}

int uart_transmit(char data, FILE *stream)
{
  while (tx_head + 1 == tx_tail);
  tx_buffer[tx_head++] = data;
  UCSR0B |= _BV(UDRIE0);
  return 0;
}

int uart_receive(FILE *stream)
{
  while (rx_head == rx_tail);
  return rx_buffer[rx_tail++];
}

void interrupt_init()
{
    UCSR0B |= _BV(RXCIE0);
    UCSR0B |= _BV(UDRIE0);
}

FILE uart_file;

int main(void)
{
    uart_init();
    interrupt_init();

    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &uart_file;

    while(1)
    {
        // printf("aaaaa");
        int a;
        scanf("%d", &a);
        printf("%d\r\n",a+2);
	}
}
