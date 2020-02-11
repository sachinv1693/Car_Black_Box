#include "main.h"

void init_ADC_config(void)
{
    //Config RA5 pin as i/p pin
    AN4 = 1;
    
    //Config ADCON2 ADCON1 ADCON0
    ADCON0 = 0x11;
    ADCON1 = 0x0A;
    ADCON2 = 0x92;
}
