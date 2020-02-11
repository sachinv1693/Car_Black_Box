#include "main.h"

/* Function to display time */
void display_time(void)
{
        //Blink the clock hand values
        if (display == DISP_SET_TIME)
        {
                //SEC_ADDR, MIN_ADDR & HOUR_ADDR happens to be 0, 1 & 2 respectively
                unsigned char i = (clock_hand == SEC_ADDR) ? 10 : (clock_hand == MIN_ADDR) ? 7 : (clock_hand == HOUR_ADDR) ? 4 : 0;
                nb_delay = (nb_delay + 1) % 100;
                (nb_delay < 50) ? clcd_print(time, LINE2(4)) : clcd_print("  ", LINE2(i));
        }
        else
                clcd_print(time, LINE2(2));
}

