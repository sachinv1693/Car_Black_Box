#include "main.h"

/* Gloabal variables */
unsigned short tim_count = 0,//Stores timer count value
			   sec = BLOCK_TIME_IN_SEC,//Holds password restriction block time
			   nb_delay = 0,//Holds non-blocking delay value
			   write_addr,//Holds eeprom write address
			   read_addr;//Holds eeprom read address

unsigned char try = ON,//A flag to avoid multiple iterations
			  no_of_tries = MAX_TRIES_ALLOWED,//No. of password tries
			  passcode_bit = 0,//Works as index of the strings
			  clock_reg[3],//Holds Hr-Min-Sec BCD values
			  time[9],//A string to keep time values
			  pswd[MAX_PASSCODE_LEN + 1],//String to store actual password
			  user_entry[MAX_PASSCODE_LEN + 1],//User entry string
			  pswd_reentry[MAX_PASSCODE_LEN + 1],//Password re-entry string
			  key_up = ON,//A flag to know if it is up key or down key
			  re_entry = OFF,//A flag to to when to re-enter string
			  clock_hand = SEC_ADDR,//Holds sec, min & hr address
			  disp_num = 0,//Display number for reading & downloading operation
			  rollover = OFF,//A flag to know rollover of eeprom address
			  speed,//Stores vehicle speed value
			  first = 0,//Holds first index value of an event
			  last = 0,//Holds last index value of an event
			  back_to_menu = OFF,//Flag to know when to go back to menu
			  log_reader[8],//An array to store bytes read from eeprom
			  long_press = OFF,//A flag to determine long press of a key
			  entry_not_full = ON,//Flag to know if passcode entry is completed
			  time_not_set = ON;//A flag to know if time value is set or not

//An array of strings having menu items
const unsigned char* menu_screen[6] = {"VIEW LOG", "SET TIME", "CHANGE PASSWD", "CLEAR LOG", "DOWNLOAD LOG", " "};//Note that last value is just dummy spaces

DISPLAY_MODE display = DISP_DASHBOARD,//Stores display type
			 arrow = 0;//to hold particular display type
EVENTS event = ON,//Stores event type
	   gear = GN;//Holds a gear event type

//Initialize all peripherals & respective values
static void init_config(void)
{
		init_clcd();
		init_matrix_keypad();
		init_timer0();
		init_i2c();
		init_ds1307();
		init_ADC_config();
		init_uart();
		get_initial_values_from_eeprom();
		get_time();
		speed = adc();
		//Store ON event into EEPROM
		write_onto_eeprom(ON);
		GIE = 1;
}

//Function to check values entered from matrix keypad
static void check_matrix_keypad(void)
{
		//key to hold edge triggered switch value
		unsigned char key_edge_trig = read_switches(STATE_CHANGE);
		//Delay to handle bouncing effect
		for (unsigned short delay = 4000; delay--;);
		//key to hold level triggered switch value
		unsigned char key_level_trig = read_switches(LEVEL_CHANGE);
		//Collision, Gear Up and Down events can be traced in all display modes
		if (key_edge_trig == MK_SW1)//Collision
		{
				//Store event in EEPROM while updating event value to display
				write_onto_eeprom(event = CO);
				CLEAR_DISP_SCREEN;
		}
		else if (key_edge_trig == MK_SW2)//Move Gear Up
		{
				if (++gear > G4)//G4 is the top gear, cannot increase the gear further
						gear = G4;
				else
						//Store event in EEPROM while updating event value to display
						write_onto_eeprom(event = gear);
				CLEAR_DISP_SCREEN;
		}
		else if (key_edge_trig == MK_SW3)//Move Gear Down
		{
				if (--gear < GR)//GR is the lowest gear, cannot decrease the gear further
						gear = GR;
				else
						//Store event in EEPROM while updating event value to display
						write_onto_eeprom(event = gear);
				CLEAR_DISP_SCREEN;
		}
		//This key is used to change from Dashboard to Pswd screen/Encode '1'/Scroll up in Menu screen & view log and to change hr, min & sec field in set time mode
		else if (key_edge_trig == MK_SW11)
		{
				if (display == DISP_DASHBOARD)
				{
						//Change to password screen
						display = DISP_PASSWORD;
						CLEAR_DISP_SCREEN;
				}
				else if ((display == DISP_PASSWORD || display == DISP_CHG_PASSWD) && passcode_bit < MAX_PASSCODE_LEN)
				{
						//Bring timer count back to 0
						tim_count = 0;
						//Encode '1' into pswd_reentry string if re_entry flag is ON, else into user_entry string
						(re_entry) ? pswd_reentry[passcode_bit] = '1': user_entry[passcode_bit] = '1';
						clcd_putch('*', LINE2(6 + passcode_bit++));
				}
				else if (display == DISP_VIEW_LOG)
				{
						if (disp_num != first && disp_num-- == 0)//Cannot scroll up beyond first index
								disp_num = MAX_LOG_COUNT - 1;
						//Read event log from eeprom
						read_from_eeprom();
						CLEAR_DISP_SCREEN;
				}
				else if (display == DISP_MENU)
				{
						key_up = OFF;//Up key is pressed
						if (arrow-- == 0)//Cannot move up beyond very first menu item
								arrow = 0;
						CLEAR_DISP_SCREEN;
				}
				else if (display == DISP_SET_TIME && time_not_set == ON)
				{
						if (clock_hand++ == HOUR_ADDR)
								clock_hand = SEC_ADDR;
				}
		}
		//This key is used to Encode '0'/Scroll down in Menu screen & view log and to change time values in set time mode
		else if (key_edge_trig == MK_SW12)
		{
				if ((display == DISP_PASSWORD || display == DISP_CHG_PASSWD) && passcode_bit < MAX_PASSCODE_LEN)
				{
						//Bring timer count back to 0
						tim_count = 0;
						//Encode '0' into pswd_reentry string if re_entry flag is ON, else into user_entry string
						(re_entry) ? pswd_reentry[passcode_bit] = '0': user_entry[passcode_bit] = '0';
						clcd_putch('*', LINE2(6 + passcode_bit++));
				}
				else if (display == DISP_VIEW_LOG)
				{
						if (disp_num != last)
								disp_num = (disp_num + 1) % MAX_LOG_COUNT;
						//Read event log from eeprom
						read_from_eeprom();
						CLEAR_DISP_SCREEN;
				}
				else if (display == DISP_MENU)
				{
						key_up = ON;//Down key is pressed
						if (arrow++ == 4)//Cannot scroll down beyond last menu item
								arrow = 4;
						CLEAR_DISP_SCREEN;
				}
				else if (display == DISP_SET_TIME)
				{
						set_time();
				}
		}
		//This key when long pressed, used for changing into screens
		else if (display == DISP_MENU && key_level_trig == MK_SW11)
		{
				long_press = TMR0ON = ON;//Turn ON long_press flag & timer
				if (tim_count >= CHG_DISP_ON_LONG_PRESS_IN_SEC * 10)
				{
						TMR0ON = OFF;//Turn OFF the timer & reset the timer count
						tim_count = 0;
						CLEAR_DISP_SCREEN;
						//Change to appropriate display
						display = arrow + DISP_VIEW_LOG;
						//Keep disp_num at first index in view log option
						if (display == DISP_VIEW_LOG)
								disp_num = first;
						else if (display == DISP_SET_TIME)
						{
								time_not_set = ON;//Allow the changes to be made in clock_reg values
								//Clock hand at Hour field. While releasing the key an interrupt is generated & it moves to Sec field as per requirement.
								clock_hand = HOUR_ADDR;
						}
						//Store Clear log event if it is clear log option
						else if (display == DISP_CLEAR_LOG)
								write_onto_eeprom(CL);//Log clearing is handled in this function itself
						else if (display == DISP_DOWNLOAD_LOG)
						{
								download_log();
								write_onto_eeprom(DL);//Write event onto eeprom
						}
				}
		}
		//This key when long pressed, used for coming out from screens
		else if ((display == DISP_MENU || display == DISP_VIEW_LOG || display == DISP_SET_TIME) && key_level_trig == MK_SW12)
		{
				long_press = TMR0ON = ON;//Turn ON long_press flag & timer
				if (tim_count >= CHG_DISP_ON_LONG_PRESS_IN_SEC * 10)
				{
						//Change to appropriate display
						if (display == DISP_VIEW_LOG)
								display = DISP_MENU;
						else if (display == DISP_MENU)
								display = DISP_DASHBOARD;
						else if (display == DISP_SET_TIME)
						{
								time_not_set = OFF;//Restrict the changes in clock_reg values 
								nb_delay = 0;//Reset nb_delay value which is used for blinking
								write_onto_eeprom(ST);//Store event in eeprom
								//Reduce time field by 1 unit since it is triggered again on last key press
								switch (clock_hand)
								{
										case SEC_ADDR://Reduce sec field
										case MIN_ADDR://Reduce min field
												if (clock_reg[HOUR_ADDR - clock_hand]-- == 0)
														clock_reg[HOUR_ADDR - clock_hand] = 0x59;
												break;
										case HOUR_ADDR://Reduce hr field
												if (clock_reg[HOUR_ADDR - clock_hand]-- == 0)
														clock_reg[HOUR_ADDR - clock_hand] = 0x23;
								}
								write_ds1307(clock_hand, clock_reg[HOUR_ADDR - clock_hand]);
								back_to_menu = ON;//New time is set, so turn ON back_to_menu flag
						}
						TMR0ON = OFF;
						tim_count = 0;
						CLEAR_DISP_SCREEN;
				}
		}
		else if (long_press)//With some long press, timer activates & tim_count changes, so must be reset. Note that this flag is set only in above 2 cases 
		{
				long_press = TMR0ON = OFF, tim_count = 0;
		}
		/* Below are the incidences when some action is to be taken without key pressing */
		//Once index reaches to MAX_PASSCODE_LEN, clear lcd display and put a terminating '\0' & turn OFF the timer
		if ((display == DISP_PASSWORD || display == DISP_CHG_PASSWD) && passcode_bit == MAX_PASSCODE_LEN && entry_not_full)
		{
				(re_entry) ? pswd_reentry[MAX_PASSCODE_LEN] = '\0' : user_entry[MAX_PASSCODE_LEN] = '\0';
				TMR0ON = entry_not_full = OFF;//Turn OFF the timer, reset timer count & note the entry full 
				tim_count = 0;
				CLEAR_DISP_SCREEN;
		}
		//Change back to Dashboard screen after inactivity time
		else if (display == DISP_PASSWORD && tim_count >= PASSCODE_ENT_TO_DASHBOARD_IN_SEC * 10)
		{
				display = DISP_DASHBOARD;
				TMR0ON = OFF;//Turn OFF the timer, reset timer count & passcode_bit index
				tim_count = passcode_bit = 0;
				CLEAR_DISP_SCREEN;
		}
		//Change back to Menu screen after inactivity time
		else if (display == DISP_CHG_PASSWD && tim_count >= CHG_PASSWD_TO_MENU_IN_SEC * 10)
		{
				display = DISP_MENU;
				DISP_ON_AND_CURSOR_OFF;
				TMR0ON = OFF;//Turn OFF the timer, reset timer count & passcode_bit index
				tim_count = passcode_bit = 0;
				CLEAR_DISP_SCREEN;
		}
}

void main(void)
{
		//Initialize configurations
		init_config();
		while (1)
		{
				get_time();//Keep reading the time
				speed = adc();//Keep reading the speed
				if (display == DISP_DASHBOARD)
				{
						clcd_print("  TIME     E  SP", LINE1(0));
						display_time();
						display_event(event);
						itoa_display(speed);
				}
				else if (display == DISP_PASSWORD || display == DISP_CHG_PASSWD)
				{
						//Accept passcode bits from 0 to MAX_PASSCODE_LEN - 1
						if (passcode_bit < MAX_PASSCODE_LEN)
						{
								//Turn ON the timer
								TMR0ON = entry_not_full = ON;
								if (!passcode_bit)
								{
										//Display to password entry message
										(re_entry) ? clcd_print("RE-ENTER PASSWD ", LINE1(1)) : clcd_print("ENTER PASSWORD  ", LINE1(1));
										//Blink the cursor
										MOV_CURSOR_ADDR(0xC6);
										DISP_ON_AND_CURSOR_BILNK_ON;
								}
						}
						else//Compare passcode strings and/or take further action
						{
								DISP_ON_AND_CURSOR_OFF;
								if (display == DISP_PASSWORD)
								{
										if (!my_strcmp(pswd, user_entry))//If password matches, display a message & switch to menu
										{
												clcd_print("PASSWORD SUCCESS", LINE1(0));
												clcd_print("..OPENING MENU..", LINE2(0));
												if (nb_delay++ == 300)
												{
														//Switch to Menu Screen & set arrow to VIEW LOG
														display = DISP_MENU;
														//Reset nb_delay, arrow & passcode_bit for next entries
														nb_delay = arrow = passcode_bit = 0;
														//Make all tries available for next entries
														no_of_tries = MAX_TRIES_ALLOWED;
														CLEAR_DISP_SCREEN;
												}

										}
										else//password does not match
										{
												//no. of tries should not be reduced on every iteration
												if (try)
												{
														//Reduce no. of tries & turn off the flag
														--no_of_tries;
														try = OFF;
												}
												//Display password failure message
												clcd_print("PASSWORD FAILURE", LINE1(0));
												//Display no. of remaining trials on next line
												if (no_of_tries)
												{
														clcd_putch('0' + no_of_tries, LINE2(0));
														clcd_print("tries remained", LINE2(2));
														//Show the message till some time
														if (nb_delay++ == 300)
														{
																//Turn on the flag & clear display screen
																try = ON;
																//Reset nb_delay, passcode bit & timer count
																nb_delay = tim_count = passcode_bit = 0;
																CLEAR_DISP_SCREEN;
														}
												}
												else//Lock screen
												{
														//Turn ON timer0
														TMR0ON = ON;
														//Display remaing sec
														clcd_print("Try in :", LINE2(0));
														clcd_putch('0' + sec % 10, LINE2(11));
														clcd_putch('0' + (sec / 10) % 10, LINE2(10));
														clcd_putch('0' + sec / 100, LINE2(9));
														clcd_print("sec", LINE2(13));
														if (tim_count >= 10)//For 1 sec, required timer count is 10
														{
																CLEAR_DISP_SCREEN;
																if (--sec == 0)
																{
																		//Turn OFF timer0 & turn ON try flag
																		TMR0ON = OFF;
																		try = ON;
																		//Reset passcode_bit
																		passcode_bit = 0;
																		//Make all tries available
																		no_of_tries = MAX_TRIES_ALLOWED;
																		//Reinitialize time in sec
																		sec = BLOCK_TIME_IN_SEC;
																}
																//Reset timer count
																tim_count = 0;
														}    
												}
										}
								}
								else if (display == DISP_CHG_PASSWD)
								{
										if (re_entry)//If password is re-entered then only verify
										{
												if (!my_strcmp(pswd_reentry, user_entry))//If password matches, store it & mark as event
												{
														clcd_print(" CHG SUCCESSFUL ", LINE1(0));//Display message
														clcd_print("..BACK TO MENU..", LINE2(0));
														if (nb_delay++ == 300)
														{
																nb_delay = passcode_bit = 0;
																re_entry = OFF;//Restrict password re-entry
																display = DISP_MENU;//Switch to menu screen
																//Store the event in eeprom
																write_onto_eeprom(CH);
																//Store the new password in eeprom & in pswd string
																for (unsigned char i = 0; i < MAX_PASSCODE_LEN; i++)
																{
																		pswd[i] = user_entry[i];
																		write_internal_eeprom(i, user_entry[i]);
																}
																CLEAR_DISP_SCREEN;
														}
												}
												else
												{
														clcd_print("PSWD NOT MATCHED", LINE1(0));
														clcd_print("   Try again    ", LINE2(0));
														if (nb_delay++ == 300)
														{
																nb_delay = passcode_bit = 0;
																re_entry = OFF;
																CLEAR_DISP_SCREEN;
														}
												}
										}
										else
										{
												passcode_bit = 0;//Go back to initial index
												re_entry = ON;//Allow to re-enter password
												CLEAR_DISP_SCREEN;
										}
								}
						}
				}
				else if (display == DISP_MENU)
				{
						display_menu(menu_screen);
				}
				else if (display == DISP_VIEW_LOG)
				{
						display_view_log();
				}
				else if (display == DISP_SET_TIME)
				{
						if (back_to_menu)
						{
								clcd_print("  NEW TIME SET  ", LINE1(0));
								clcd_print("..BACK TO MENU..", LINE2(0));
								if (nb_delay++ == 300)
								{
										display = DISP_MENU;//Switch to menu screen
										nb_delay = 0;//Reset nb_delay
										back_to_menu = OFF;//Switch the flag OFF for next entry
										CLEAR_DISP_SCREEN;
								}
						}
						else
						{
								clcd_print("HH:MM:SS", LINE1(4));//Time format on top line
								//Show changes being made in time setting
								display_time();
						}
				}
				else if (display == DISP_CLEAR_LOG)
				{
						//Just display a message for a short time & return to menu screen
						clcd_print("LOG CLEARED", LINE1(2));
						clcd_print("..BACK TO MENU..", LINE2(0));
						if (nb_delay++ == 300)
						{
								display = DISP_MENU;//Switch to menu screen
								nb_delay = 0;//Reset nb_delay
								CLEAR_DISP_SCREEN;
						}
				}
				else if (display == DISP_DOWNLOAD_LOG)
				{
						//Just display a message for a short time & return to menu screen
						clcd_print("DOWNLOADING DONE", LINE1(0));
						clcd_print("..BACK TO MENU..", LINE2(0));
						if (nb_delay++ == 300)
						{
								display = DISP_MENU;//Switch to menu screen
								nb_delay = 0;//Reset nb_delay
								CLEAR_DISP_SCREEN;
						}
				}
				//Keep checking for matrix key press
				check_matrix_keypad();
		}
}

