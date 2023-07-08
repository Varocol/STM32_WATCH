#include "libraries.h"

int main()
{
    led_init();
    while (1)
    {
        // test 1
        led_toggle();
        delay_ms(500);
    }
}