#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <util/delay.h>

//Michał Łukasik

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

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

void dash()
{
    LED_DDR |= _BV(LED);
    LED_PORT |= _BV(LED);
    _delay_ms(600);
    LED_PORT &= ~_BV(LED);
    _delay_ms(200);
}

void dot()
{
    LED_DDR |= _BV(LED);
    LED_PORT |= _BV(LED);
    _delay_ms(200);
    LED_PORT &= ~_BV(LED);
    _delay_ms(200);
}

FILE uart_file;

char *morse[]= { ".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // program testowy
  while(1) {
    char a = 1;
    scanf("%c", &a);
    char *command = morse[a-'a'];
    for (short i=0;i<strlen(command);i++)
    {
        if (command[i]=='.') dot();
        else dash();
    }
    printf("Wypisano: %c""\r\n", a);
  }
}

