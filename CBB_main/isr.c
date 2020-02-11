#include "main.h"

//Extern timer count value
extern unsigned short tim_count;

void interrupt isr(void)
{
		//Check if timer0 interrupt flag is set
		if (TMR0IF)
		{
				//Add 3036 with 2 extra cycle
				TMR0 = TMR0 + 3038;
				//Increment timer count
				tim_count++;
				//Clear the interrupt flag
				TMR0IF = 0;
		}
}

