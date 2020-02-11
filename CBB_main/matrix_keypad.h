#ifndef MATRIX_KEYPAD_H
#define MATRIX_KEYPAD_H

/* Refer pic_specific.h for port specific definations */

#define MAX_ROW			4
#define MAX_COL			3

#define STATE_CHANGE	1
#define LEVEL_CHANGE	0

#define MK_SW1			1
#define MK_SW2			2
#define MK_SW3			3
#define MK_SW4			4
#define MK_SW5			5
#define MK_SW6			6
#define MK_SW7			7
#define MK_SW8			8
#define MK_SW9			9
#define MK_SW10			10
#define MK_SW11			11
#define MK_SW12			12

#define ALL_RELEASED	0xFF

#define HI				1
#define LO				0

void init_matrix_keypad(void);
unsigned char scan_key(void);
unsigned char read_switches(unsigned char detection_type);

#endif
