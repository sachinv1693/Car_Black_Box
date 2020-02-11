#include "main.h"

/* Function to read 8 bytes from the internal eeprom */
void read_from_eeprom(void)
{
        //Base address to start reading from eeprom
        read_addr = disp_num * 8 + MAX_PASSCODE_LEN + 4;
        //6 bytes clock value 
        log_reader[0] = read_internal_eeprom(read_addr);
        log_reader[1] = read_internal_eeprom(read_addr + 1);
        log_reader[2] = read_internal_eeprom(read_addr + 2);
        log_reader[3] = read_internal_eeprom(read_addr + 3);
        log_reader[4] = read_internal_eeprom(read_addr + 4);
        log_reader[5] = read_internal_eeprom(read_addr + 5);
        //1 byte event value
        log_reader[6] = read_internal_eeprom(read_addr + 6);
        //1 byte speed value
        log_reader[7] = read_internal_eeprom(read_addr + 7);
}

