#ifndef MAIN_H
#define MAIN_H

#include "pic_specific.h"
#include "clcd.h"
#include "init_ADC_config.h"
#include "timer0.h"
#include "ds1307.h"
#include "i2c.h"
#include "eeprom.h"
#include "uart.h"
#include "matrix_keypad.h"

#define OFF				                    0
#define MAX_PASSCODE_LEN                    4
#define MAX_TRIES_ALLOWED                   5
#define BLOCK_TIME_IN_SEC                   30
#define MAX_LOG_COUNT                       10
#define PASSCODE_ENT_TO_DASHBOARD_IN_SEC    5
#define CHG_PASSWD_TO_MENU_IN_SEC           15
#define CHG_DISP_ON_LONG_PRESS_IN_SEC       3

typedef enum
{
		CO,//Collision
		ON,//Power ON and flag ON
		GR,//Reverse gear
		GN,//Neutral gear
		G1,//1st gear
		G2,//2nd gear
		G3,//3rd gear
		G4,//4th gear
		ST,//Set Time
		CH,//Change Password
		CL,//Clear Log
		DL//Download Log
}EVENTS;

typedef enum
{
		DISP_DASHBOARD,
		DISP_PASSWORD,
		DISP_MENU,
		DISP_VIEW_LOG,
		DISP_SET_TIME,
		DISP_CHG_PASSWD,
		DISP_CLEAR_LOG,
		DISP_DOWNLOAD_LOG
}DISPLAY_MODE;

/* Extern unsigned short variables */
extern unsigned short write_addr,//eeprom write address
	   read_addr,//eeprom read address
	   nb_delay;//Non-blocking delay

/* Extern unsigned char variables */
extern unsigned char clock_reg[3],//Holds BCD values of Hr-Min-Sec
	   time[9],//Time string holding time values
	   pswd[MAX_PASSCODE_LEN + 1],//Original password storing string
	   key_up,//Flag to know up key & down key press
	   clock_hand,//Holds sec, min & hr address
	   disp_num,//Display number for reading & downloading operation
	   rollover,//Flag to detect rollover of eeprom address
	   first,//First index value of an event
	   last,//Last index of value of an event
	   log_reader[8],//Array for storing eeprom read values
	   speed;//Vehicle speed

/* Extern display type & the arrow to specific menu */
extern DISPLAY_MODE display, arrow;

/* Function prototypes */
void get_initial_values_from_eeprom(void);
void write_onto_eeprom(EVENTS store_event);
void read_from_eeprom(void);
void display_time(void);
void set_time(void);
void get_time(void);
void display_event(EVENTS event);
void itoa_display(unsigned char data);
void display_menu(const unsigned char* menu_screen[]);
unsigned char my_strcmp(const char* str1, const char* str2);
void display_view_log(void);
void download_log(void);
unsigned char adc(void);

#endif

