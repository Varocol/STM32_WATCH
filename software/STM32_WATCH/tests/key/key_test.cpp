#include "libraries.h"
#include "system.h"

int main()
{
    power_on();
    led_init();
    keys_init();
    keys_intterupt_init();
    while (1)
    {
        if (key4_status == Pressed && key4_status_time >= 1000)
        {
            led_toggle();
        }
        delay_ms(500);
    }
}