#include "libraries.h"
#include "system.h"

int main()
{
    power_on();
    led_init();
    while (1) {
        led_toggle();
        delay_ms(500);
    }
}