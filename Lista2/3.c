#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>

//Michał Łukasik

#define BTN1 PB4
#define BTN2 PB3
#define BTN3 PB2
#define BTN_PIN PINB
#define BTN_PORT PORTB

uint8_t gray = 0;
uint8_t out = 0;
uint8_t buttonN[2] = {0,0};
uint8_t buttonP[2] = {0,0};
uint8_t buttonR = 0;
uint8_t changedN = 0;
uint8_t changedP = 0;

int main() {
    BTN_PORT |= _BV(BTN1);
    BTN_PORT |= _BV(BTN2);
    BTN_PORT |= _BV(BTN3);

    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);
    DDRD=0xFF;
    while (1) {
        // if (changed) LED_PORT &= ~_BV(LED);
        // else LED_PORT &= _BV(LED);

        buttonN[0] = BTN_PIN & _BV(BTN1);
        buttonP[0] = BTN_PIN & _BV(BTN2);
        buttonR = BTN_PIN & _BV(BTN3);
        _delay_ms(20);
        buttonN[1] = BTN_PIN & _BV(BTN1);
        buttonP[1] = BTN_PIN & _BV(BTN2);
        if (buttonN[0] == 0 && buttonN[1] == 0 && changedN == 0)
        {
            gray += 1;
            PORTD = gray^(gray >>1);
            changedN = 0xFF;
        }
        else if (buttonN[1] != 0)
        {
            changedN = 0;
        }
        if (buttonP[0] == 0 && buttonP[1] == 0 && changedP == 0)
        {
            gray -= 1;
            PORTD = gray^(gray >>1);
            changedP = 0xFF;
        }
        else if (buttonP[1] != 0)
        {
            changedP = 0;
        }

        if (buttonR == 0)
        {
            gray = 0;
            PORTD = 0;
            changedN = 0xFF;
            changedP = 0xFF;
        }

    }
}
