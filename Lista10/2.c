#include <avr/io.h>
#include <stdint.h>
#include <util/delay.h>
#include <stdio.h>
#include "hd44780.c"

//Michał Łukasik

int hd44780_transmit(char data, FILE *stream)
{
    LCD_WriteData(data);
    return 0;
}

FILE hd44780_file;

uint8_t tab[5] = {0b10000, 0b11000, 0b11100, 0b11110, 0b11111};

int main()
{
    LCD_Initialize();
    LCD_Clear();
    fdev_setup_stream(&hd44780_file, hd44780_transmit, NULL, _FDEV_SETUP_WRITE);
    stdin = stdout = stderr = &hd44780_file;
    printf("Hello world!");
    LCD_GoTo(0, 1);

    for(uint8_t i=0; i<5; i++)
    {
        LCD_WriteCommand(HD44780_CGRAM_SET | (i<<3));
        for(uint8_t j=0;j<7;j++){
            LCD_WriteData(tab[i]);
        }
    }
    _delay_ms(1000);
    for(uint8_t i=0;i<16;i++)
    {
        for(uint8_t j=0; j<5; j++)
        {
            LCD_GoTo(i, 1);
            printf("%c",j);
            _delay_ms(1000);
        }
    }
    while(1);
}

