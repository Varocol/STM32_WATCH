#include "libraries.h"
#include "system.h"

int main()
{
    power_on();
    led_init();
    uart_init();
    while (1)
    {
        led_toggle();
        uart_send_string("123123");
        delay_ms(500);
    }
}