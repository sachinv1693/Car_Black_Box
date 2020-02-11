#include "main.h"

/* Function to write events onto the internal eeprom */
void write_onto_eeprom(EVENTS store_event)
{
        unsigned char write_offset = (unsigned char) MAX_PASSCODE_LEN + 4;
        unsigned char rollover_addr = (unsigned char) 8 * MAX_LOG_COUNT + write_offset;
        if (store_event == CL)
        {
                write_addr = write_offset;
                rollover = OFF;
        }
        /*---------Writing 8 bytes onto eeprom from given address-----------*/
        //Write 6 bytes from time string (HH, MM & SS)
        for (unsigned char i = 0; i < 8; i++)
        {
                if (i != 2 && i != 5)//Skip ':' part
                        write_internal_eeprom(write_addr++, time[i]);//Address post-incremented
        }
        //Write 1 byte event value
        write_internal_eeprom(write_addr++, store_event);//Address post-incremented
        //Write 1 byte speed value
        write_internal_eeprom(write_addr++, speed);//Address post-incremented
        /*--------------8 bytes are written onto eeprom---------------------*/
        //Check for rollover of index since only MAX_LOG_COUNT event entries are allowed
        if (write_addr == rollover_addr)
        {
                write_addr = write_offset;
        }
        //Note that first & last index should be in the range from 0 to MAX_LOG_COUNT - 1
        if (rollover)//If rollover occured then, last index takes value of first index & first index points to new entry
        {
                last = first;
                first = (write_addr == write_offset) ? 0 : (write_addr - write_offset) / 8;
        }
        else//Otherwise first index remains at 0 & last index points to new entry
        {
                first = 0;
                last = (write_addr == write_offset) ? (unsigned char) MAX_LOG_COUNT - 1 : (write_addr - 2 * write_offset) / 8;
                if (write_addr == write_offset)
                        rollover = ON;
        }
        //Store initial storing address of next event onto eeprom address after password
        write_internal_eeprom((unsigned char) MAX_PASSCODE_LEN, write_addr);
        //Store rollover flag value onto next eeprom address
        write_internal_eeprom((unsigned char) MAX_PASSCODE_LEN + 1, rollover);
        //Store first & last flag values onto next consecutive eeprom addresses
        write_internal_eeprom((unsigned char) MAX_PASSCODE_LEN + 2, first);
        write_internal_eeprom((unsigned char) MAX_PASSCODE_LEN + 3, last);
}

