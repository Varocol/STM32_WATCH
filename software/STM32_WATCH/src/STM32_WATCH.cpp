#include "libraries.h"

int main()
{
    led_init();
    while (1) {
        // test 1
        // led_on();
        // test 2
        // led_off();
        // test 3
        led_toggle();
        delay_ms(500);
    }
}