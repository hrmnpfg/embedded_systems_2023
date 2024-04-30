#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//Michał Łukasik

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB

//access time szybszy niż tablica bitów
volatile int8_t buf[100];
volatile int8_t i=0;

ISR(TIMER1_COMPA_vect)
{
    if (buf[i] == 0) LED_PORT |= _BV(LED);
    else LED_PORT &= ~_BV(LED);
    buf[i] = BTN_PIN & _BV(BTN);
    i++;
    if(i>100) i=0;
}

void timer_init()
{
    TCCR1B = _BV(CS11) | _BV(WGM12);
    OCR1A = 19999;
    TIMSK1 = _BV(OCIE1A);
}

int main() {
    timer_init();
    BTN_PORT |= _BV(BTN);
    LED_DDR |= _BV(LED);
    sei();
    set_sleep_mode(SLEEP_MODE_IDLE);
    while (1) {
        sleep_mode();
    }
}
