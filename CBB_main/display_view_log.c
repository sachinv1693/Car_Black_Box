#include "main.h"

/* Function to display view log */
void display_view_log(void)
{
		clcd_print("# TIME     E  SP", LINE1(0));
		//disp_num in the range from 0 to MAX_LOG_COUNT - 1
		(disp_num >= first) ? clcd_putch('0' + disp_num - first, LINE2(0)) : clcd_putch('0' + disp_num + MAX_LOG_COUNT - first, LINE2(0));
		//Display time values
		clcd_putch(log_reader[0], LINE2(2));
		clcd_putch(log_reader[1], LINE2(3));
		clcd_putch(':', LINE2(4));
		clcd_putch(log_reader[2], LINE2(5));
		clcd_putch(log_reader[3], LINE2(6));
		clcd_putch(':', LINE2(7));
		clcd_putch(log_reader[4], LINE2(8));
		clcd_putch(log_reader[5], LINE2(9));
		//Display event & speed values
		display_event(log_reader[6]);
		itoa_display(log_reader[7]);
}

