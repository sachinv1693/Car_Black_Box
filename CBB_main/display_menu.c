#include "main.h"

/* Function to display menu */
void display_menu(const unsigned char* menu_screen[])
{
        if (key_up && !(arrow % 2) || !key_up && !(arrow % 2))
        {
                clcd_putch(RIGHT_ARROW, LINE1(0));
                clcd_print(menu_screen[arrow], LINE1(2));
                clcd_print(menu_screen[arrow + 1], LINE2(2));
        }
        else
        {
                clcd_putch(RIGHT_ARROW, LINE2(0));
                clcd_print(menu_screen[arrow], LINE2(2));
                clcd_print(menu_screen[arrow - 1], LINE1(2));
        }
}

