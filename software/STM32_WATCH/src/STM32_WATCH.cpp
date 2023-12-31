#include "system.h"

struct beep_song test_song =
    {
        .volume = 90,
        .Basicspeed = 50,
        .tone = B,
        .octachord = 1,
        .name = "两只老虎",
        .data = {0x15, 0x02, 0x16, 0x02, 0x17, 0x02, 0x15, 0x02, 0x15, 0x02,
                 0x16, 0x02, 0x17, 0x02, 0x15, 0x02, 0x17, 0x02, 0x18, 0x02,
                 0x19, 0x01, 0x17, 0x02, 0x18, 0x02, 0x19, 0x01, 0x19, 0x03,
                 0x1A, 0x03, 0x19, 0x03, 0x18, 0x03, 0x17, 0x02, 0x15, 0x16,
                 0x19, 0x03, 0x1A, 0x03, 0x19, 0x03, 0x18, 0x03, 0x17, 0x02,
                 0x15, 0x16, 0x15, 0x02, 0x0F, 0x02, 0x15, 0x01, 0x15, 0x02,
                 0x0F, 0x02, 0x15, 0x01, 0x00, 0x00},
};

int main()
{
    system_init();
    // beep_music(&test_song);
    while (1)
    {
        if (key4_status == Pressed && key4_status_time >= 1000)
        {
            led_toggle();
            // beep_music(&test_song);
            beep_tip();
            delay_ms(500);
        }
    }
}