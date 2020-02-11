#include "main.h"

/* Function to download the log */
void download_log(void)
{
		//Store view number to adjust the scale from 1 to MAX_LOG_COUNT
		unsigned char view_number;
		puts("----------------Car Black Box Data------------------\n\r");
		puts("Sr. No.       TIME STAMP          EVENT        SPEED\n\r");           
		//Read each byte from eeprom first index to last index & send to PC via UART
		//disp_num is in the range from 0 to MAX_LOG_COUNT - 1
		//Read all values from eeprom and transmit via UART
		disp_num = first;
		while ((!rollover && (disp_num <= last)) || rollover)
		{       
				read_from_eeprom();
				view_number = (disp_num >= first) ? '1' + disp_num - first : '1' + disp_num + MAX_LOG_COUNT - first;
				if (view_number <= '9')
				{
						putch(' ');
						putch(view_number);
				}
				else    
						puts("10");
				puts(".            ");
				//Display time values
				putch(log_reader[0]);
				putch(log_reader[1]);
				putch(':');
				putch(log_reader[2]);
				putch(log_reader[3]);
				putch(':');
				putch(log_reader[4]);
				putch(log_reader[5]);
				puts("            ");
				//Display event & speed values
				display_event(log_reader[6]);
				itoa_display(log_reader[7]);
				puts("\n\r");
				disp_num = (disp_num + 1) % MAX_LOG_COUNT;
				if (disp_num == first)
						break;
		}
		puts("----------------*-*-*-*-*-*-*-*-*-*-----------------\n\n\r");
}

