#include "main.h"

void itoa_display(unsigned char data)
{
		unsigned char array[3];
		array[0] = '0' + data / 10;
		array[1] = '0' + data % 10;
		array[2] = '\0';//Terminate the string
		if (display == DISP_DOWNLOAD_LOG)
		{
				puts("            ");
				puts(array);
		}
		else
				clcd_print(array, LINE2(14));
}

