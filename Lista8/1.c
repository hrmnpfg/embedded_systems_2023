#include "FreeRTOS.h"
#include "task.h"
#include <avr/io.h>

//Michał Łukasik

#define mainLED_TASK_PRIORITY   1
#define mainBUTTON_TASK_PRIORITY 1

static void vBlinkLed(void* pvParameters);
static void vButton(void* pvParameters);

int main(void)
{
    // Create task.
    xTaskHandle blink_handle;
    xTaskHandle button_handle;

    xTaskCreate
        (
         vBlinkLed,
         "zad",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainLED_TASK_PRIORITY,
         &blink_handle
        );

    xTaskCreate
        (
         vButton,
         "serial",
         configMINIMAL_STACK_SIZE,
         NULL,
         mainBUTTON_TASK_PRIORITY,
         &button_handle
        );

    // Start scheduler.
    vTaskStartScheduler();

    return 0;
}


void vApplicationIdleHook(void)
{

}

static void vBlinkLed(void* pvParameters)
{
    UCSR0B &= ~_BV(RXEN0) & ~_BV(TXEN0);

    vTaskDelay(7);
    DDRD=0xFF;
    uint8_t a;
    while(1) {
        a = 0b10000000;
        for(uint8_t i=0;i<8;i++){
            PORTD= a;
            vTaskDelay(100);
            a >>= 1;
        }
  }
}

#define LED PB5
#define LED_DDR DDRB
#define LED_PORT PORTB

#define BTN PB4
#define BTN_PIN PINB
#define BTN_PORT PORTB

uint8_t buf[100];

static void vButton(void* pvParameters)
{
    BTN_PORT |= _BV(BTN);
    LED_DDR |= _BV(LED);
    uint8_t i=0;
    while (1) {
        if (buf[i] == 0) LED_PORT |= _BV(LED);
        else LED_PORT &= ~_BV(LED);
        buf[i] = BTN_PIN & _BV(BTN);
        i++;
        if(i>=100) i=0;
        vTaskDelay(10);
    }
}
