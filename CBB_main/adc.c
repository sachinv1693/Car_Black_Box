#include "main.h"

unsigned char adc(void)
{
	unsigned short data;

	//Start ADC conversion
	ADCON0bits.GO = 1;

	//wait till conversion is complete
	while(ADCON0bits.GO);

	//Store the result
	data = ADRESH;
	data = (data << 8) | ADRESL;

	//Change the data onto the scale of 0-90 from 0-1023 and return
	return (data = (unsigned char)((float) data / 11.3)) ;

}
