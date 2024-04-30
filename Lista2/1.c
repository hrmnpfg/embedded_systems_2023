#include <avr/io.h>
#include <util/delay.h>

//Michał Łukasik

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB

//access time szybszy niż tablica bitów
int8_t buf[100];


int main() {
  BTN_PORT |= _BV(BTN);
  LED_DDR |= _BV(LED);
  int8_t i=0;
  while (1) {
    if (buf[i] == 0) LED_PORT |= _BV(LED);
    else LED_PORT &= ~_BV(LED);
    buf[i] = BTN_PIN & _BV(BTN);
    _delay_ms(10);
    i++;
    if(i>100) i=0;
  }
}
