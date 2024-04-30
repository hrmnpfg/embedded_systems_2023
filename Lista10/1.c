#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include "hd44780.c"

//Michał Łukasik

#define MT "                \0"
#define BAUD 9600                          // baudrate
#define UBRR_VALUE ((F_CPU)/16/(BAUD)-1)   // zgodnie ze wzorem

void uart_init()
{
    UBRR0 = UBRR_VALUE;
    UCSR0A = 0;
    UCSR0B = _BV(RXEN0) | _BV(TXEN0);
    UCSR0C = _BV(UCSZ00) | _BV(UCSZ01);
}

int uart_receive(FILE *stream)
{
    while (!(UCSR0A & _BV(RXC0)));
    return UDR0;
}

int hd44780_transmit(char data, FILE *stream)
{
    LCD_WriteData(data);
    return 0;
}

FILE hd44780_file;

int main()
{
    uart_init();
    LCD_Initialize();
    LCD_Clear();
    fdev_setup_stream(&hd44780_file, hd44780_transmit, uart_receive, _FDEV_SETUP_RW);
    stdin = stdout = stderr = &hd44780_file;
    printf("Hello world!");
    char v = 1;
    LCD_GoTo(0, 1);
    LCD_WriteCommand(HD44780_DISPLAY_ONOFF | HD44780_DISPLAY_ON | HD44780_CURSOR_ON | HD44780_CURSOR_BLINK);
    uint8_t index = 0;
    char array[17] = MT;
    while(1) {
        scanf("%c",&v);
        if (v == '\r')
        {
            LCD_Clear();
            LCD_GoTo(0, 0);
            printf("%s", array);
            LCD_GoTo(0, 1);
            index = 0;
            strncpy(array,MT,17);
            continue;
        }
        printf("%c", v);
        array[index] = v;
        index++;
        if (index == 16)
        {
            LCD_GoTo(0, 1);
            index = 0;
        }
    }
}

