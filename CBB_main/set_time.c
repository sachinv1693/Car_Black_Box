#include "main.h"

void set_time(void)
{
		if ((++clock_reg[HOUR_ADDR - clock_hand] > 9) && ((clock_reg[HOUR_ADDR - clock_hand] - 10) % 0x10 == 0))
				clock_reg[HOUR_ADDR - clock_hand] += 6;
		//Increment selected field value
		switch (clock_hand)
		{
				case SEC_ADDR://Change sec field
				case MIN_ADDR://Change min field
						if (clock_reg[HOUR_ADDR - clock_hand] == 0x60)
								clock_reg[HOUR_ADDR - clock_hand] = 0;
						break;
				case HOUR_ADDR://Change hr field
						if (clock_reg[0] == 0x24)
								clock_reg[0] = 0;
						break;
		}
		//Note that SEC_ADDR, MIN_ADDR & HOUR_ADDR are 0, 1 & 2 respectively. So we will use the same as index values in time string
		for (unsigned char i = SEC_ADDR; i <= HOUR_ADDR; i++)
				write_ds1307(i, clock_reg[HOUR_ADDR - i]);
		/* Clearing the CH bit of the RTC to Start the Clock */
		//write_ds1307(SEC_ADDR, read_ds1307(SEC_ADDR) | 0x7F);
}
