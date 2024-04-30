#include <avr/io.h>
#include <stdio.h>
#include <inttypes.h>

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

void b8()
{
    //int 8
    printf("int8_t\r\n");
    int8_t a8 = 1;
    int8_t b8 = 1;
    scanf("%"SCNd8, &a8);
    printf("odczytano %"SCNd8"\r\n", a8);
    scanf("%"SCNd8, &b8);
    printf("odczytano %"SCNd8"\r\n", b8);
    int8_t s8 = a8+b8;
    printf("Suma: %"PRId8"\r\n", s8);
    int8_t m8 = a8*b8;
    printf("Mnożenie: %"PRId8"\r\n", m8);
    int8_t d8 = a8/b8;
    printf("Dzielenie: %"PRId8"\r\n", d8);
}

void b16()
{
    //int 16
    printf("int16_t\r\n");
    int16_t a16 = 1;
    int16_t b16 = 1;
    scanf("%"SCNd16, &a16);
    printf("odczytano %"SCNd16"\r\n", a16);
    scanf("%"SCNd16, &b16);
    printf("odczytano %"SCNd16"\r\n", b16);
    int16_t s16 = a16+b16;
    printf("Suma: %"PRId16"\r\n", s16);
    int16_t m16 = a16*b16;
    printf("Mnożenie: %"PRId16"\r\n", m16);
    int16_t d16 = a16/b16;
    printf("Dzielenie: %"PRId16"\r\n", d16);
}

void b32()
{
    //int 32
    printf("int32_t\r\n");
    int32_t a32 = 1;
    int32_t b32 = 1;
    scanf("%"SCNd32, &a32);
    printf("odczytano %"SCNd32"\r\n", a32);
    scanf("%"SCNd32, &b32);
    printf("odczytano %"SCNd32"\r\n", b32);
    int32_t s32 = a32+b32;
    printf("Suma: %"PRId32"\r\n", s32);
    int32_t m32 = a32*b32;
    printf("Mnożenie: %"PRId32"\r\n", m32);
    int32_t d32 = a32/b32;
    printf("Dzielenie: %"PRId32"\r\n", d32);
}

void b64()
{
    //int 64
    printf("int64_t\r\n");
    int64_t a64 = 1;
    int64_t b64 = 1;
    scanf("%"SCNd32, &a64);
    printf("odczytano %"PRId32"\r\n", a64);
    scanf("%"SCNd32, &b64);
    printf("odczytano %"SCNd32"\r\n", b64);
    int64_t s64 = a64+b64;
    printf("Suma: %"PRId32"\r\n", s64);
    int64_t m64 = a64*b64;
    printf("Mnożenie: %"PRId32"\r\n", m64);
    int64_t d64 = a64/b64;
    printf("Dzielenie: %"PRId32"\r\n", d64);
}

void fl()
{
    //float
    printf("float\r\n");
    float af = 1;
    float bf = 1;
    scanf("%f", &af);
    printf("odczytano %f""\r\n", af);
    scanf("%f", &bf);
    printf("odczytano %f""\r\n", bf);
    float fs = af+bf;
    printf("Suma: %f""\r\n", fs);
    float fm = af*bf;
    printf("Mnożenie: %f""\r\n", fm);
    float fd = af/bf;
    printf("Dzielenie: %f""\r\n", fd);
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
  while(1) {
    b8();
    b16();
    b32();
    b64();
    fl();
  }
}

