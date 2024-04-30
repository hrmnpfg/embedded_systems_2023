#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include <util/delay.h>

//Michał Łukasik

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB

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

FILE uart_file;

char buf[4] = "    ";

char *morse[]= { ".-","-...","-.-.","-..",".","..-.","--.","....","..",".---","-.-",".-..","--","-.","---",".--.","--.-",".-.","...","-","..-","...-",".--","-..-","-.--","--.."};

uint8_t lengths[]= {2,4,4,3,1,4,3,4,2,4,3,4,2,2,3,4,4,3,3,1,3,4,3,4,4,4};

char translate(uint8_t len)
{
    uint8_t valid = 1;
    for (uint8_t i=0; i<26; i++)
    {
        if (len != lengths[i]) continue;
        for (uint8_t j=0; j<len; j++)
        {
            if (buf[j] == morse[i][j])
            {
                continue;
            }
            else valid = 0;
        }
        if (valid) return 'a'+i;
        valid = 1;
    }
    return ' ';
}

uint32_t count[2] = {0,0};
uint8_t button;

uint8_t index = 0;
uint8_t printed = 0;
uint8_t space = 1;

int main()
{
    BTN_PORT |= _BV(BTN);
    LED_DDR |= _BV(LED);
    // zainicjalizuj UART
    uart_init();
    // skonfiguruj strumienie wejścia/wyjścia
    fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
    stdout = stderr = &uart_file;

    BTN_PORT |= _BV(BTN);
    _delay_ms(100);
    while(1) {
        if (button)
        {
            count[0]++;
            count[1]=0;
        }
        else
        {
            count[0]=0;
            count[1]++;
            printed = 0;
        }

        if (count[1] > 40) LED_PORT |= _BV(LED);
        else LED_PORT &= ~_BV(LED);
        button = BTN_PIN & _BV(BTN);

        if(button > 0 && count[1] > 2)
        {
            if (count[1]<40)
            {
                buf[index] = '.';
            }
            else
            {
                buf[index] = '-';
            }
            index++;
        }
        else if ((button > 0 && count[0] > 100 && printed == 0) || index == 4)
        {
            printed = 1;
            space = 0;
            char a = translate(index);
            printf("%c", a);
            //for (uint8_t i = 4; i>=index; i--) buf[i] = ' ';
            //printf("%s \r\n",buf);
            index = 0;
        }
        if (button > 0 && count[0] > 500 && space == 0)
        {
            printf(" ");
            space = 1;
        }
        _delay_ms(10);
    }
}

