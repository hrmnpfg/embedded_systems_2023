#include <avr/io.h>
#include <avr/sfr_defs.h>
#include <stdint.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>

//Michał Łukasik

#define SLEEP_MODE_IDK SLEEP_MODE_IDLE

#define LED PD3
#define LED_DDR DDRD
#define LED_PORT PORTD

#define BTN PD4
#define BTN_PIN PIND
#define BTN_PORT PORTD

void spi_slave_init()
{
    DDRB |= _BV(DDB4);
    SPCR |= _BV(SPE) | _BV(SPIE);
}

ISR(SPI_STC_vect)
{
    uint8_t a = SPDR;
    if (a == 0) LED_PORT |= _BV(LED);
    else LED_PORT &= ~_BV(LED);

    SPDR = (BTN_PIN & _BV(BTN))!=0;
}

FILE uart_file;

int main(void)
{
    spi_slave_init();

    BTN_PORT |= _BV(BTN);
    LED_DDR |= _BV(LED);

    sei();
    set_sleep_mode(SLEEP_MODE_IDK);
    while (1) {
        sleep_mode();
    }
}
