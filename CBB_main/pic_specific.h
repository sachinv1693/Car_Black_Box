#ifndef PIC_SPECIFIC_H
#define PIC_SPECIFIC_H

/* Please use this file to use all architecture dependent contents like Port Informations, system specific header files etc. */

#include <xc.h>

#define CLCD_PORT				PORTD
#define CLCD_EN					PORTCbits.RC2
#define CLCD_RS					PORTCbits.RC1
#define CLCD_RW					PORTCbits.RC0
#define CLCD_BUSY				PORTDbits.RD7
#define PORT_DIR				TRISD7
#define MATRIX_KEYPAD_PORT	    PORTB
#define ROW3					PORTBbits.RB7
#define ROW2					PORTBbits.RB6
#define ROW1					PORTBbits.RB5
#define COL4					PORTBbits.RB4
#define COL3					PORTBbits.RB3
#define COL2					PORTBbits.RB2
#define COL1					PORTBbits.RB1

#endif

