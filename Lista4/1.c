#include <avr/io.h>
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>
#include <util/delay.h>
#include <stdlib.h>

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

void timer1_init()
{
  TCCR1B = _BV(CS10);
}

void b8()
{
    //int 8
    printf("int8_t\r\n");
    volatile int8_t a8 = rand();
    volatile int8_t b8 = rand();
    uint16_t a;
    uint16_t b;
    a = TCNT1;
    volatile int8_t s8 = a8+b8;
    b = TCNT1;
    printf("%u\r\n",b-a);
    a = TCNT1;
    volatile int8_t m8 = a8*b8;
    b = TCNT1;
    printf("%u\r\n",b-a);
    a = TCNT1;
    volatile int8_t d8 = a8/b8;
    b = TCNT1;
    printf("%u\r\n",b-a);
}

void b16()
{
    //int 16
    printf("int16_t\r\n");
    volatile int16_t a16 = rand();
    volatile int16_t b16 = rand();
    uint16_t a;
    uint16_t b;
    a = TCNT1;
    volatile int16_t s16 = a16+b16;
    b = TCNT1;
    printf("%u\r\n",b-a);
    a = TCNT1;
    volatile int16_t m16 = a16*b16;
    b = TCNT1;
    printf("%u\r\n",b-a);
    a = TCNT1;
    volatile int16_t d16 = a16/b16;
    b = TCNT1;
    printf("%u\r\n",b-a);
}

void b32()
{
    //int 32
    printf("int32_t\r\n");
    volatile int32_t a32 = rand();
    volatile int32_t b32 = rand();
    uint16_t a;
    uint16_t b;
    a = TCNT1;
    volatile int32_t s32 = a32+b32;
    b = TCNT1;
    printf("%u\r\n",b-a);
    a = TCNT1;
    volatile int32_t m32 = a32*b32;
    b = TCNT1;
    printf("%u\r\n",b-a);
    a = TCNT1;
    volatile int32_t d32 = a32/b32;
    b = TCNT1;
    printf("%u\r\n",b-a);
}

void b64()
{
    //int 64
    printf("int64_t\r\n");
    volatile int64_t a64 = rand();
    volatile int64_t b64 = rand();
    uint16_t a;
    uint16_t b;
    a = TCNT1;
    volatile int64_t s64 = a64+b64;
    b = TCNT1;
    printf("%u\r\n",b-a);
    a = TCNT1;
    volatile int64_t m64 = a64*b64;
    b = TCNT1;
    printf("%u\r\n",b-a);
    a = TCNT1;
    volatile int64_t d64 = a64/b64;
    b = TCNT1;
    printf("%u\r\n",b-a);
}

void fl()
{
    //float
    printf("float\r\n");
    volatile float af = 1.1;
    volatile float bf = 1121.1;
    uint16_t a;
    uint16_t b;
    a = TCNT1;
    volatile float sf = af+bf;
    b = TCNT1;
    printf("%u\r\n",b-a);
    a = TCNT1;
    volatile float mf = af*bf;
    b = TCNT1;
    printf("%u\r\n",b-a);
    a = TCNT1;
    volatile float df = af/bf;
    b = TCNT1;
    printf("%u\r\n",b-a);
}

FILE uart_file;

int main()
{
  // zainicjalizuj UART
  uart_init();
  // skonfiguruj strumienie wejścia/wyjścia
  fdev_setup_stream(&uart_file, uart_transmit, uart_receive, _FDEV_SETUP_RW);
  stdin = stdout = stderr = &uart_file;
  // program testowy
  printf("Hello world!\r\n");
  timer1_init();
  while(1) {
    b8();
    _delay_ms(1000);
    b16();
    _delay_ms(1000);
    b32();
    _delay_ms(1000);
    b64();
    _delay_ms(1000);
    fl();
    _delay_ms(10000);
    printf("\r\n\r\n");
  }
}

