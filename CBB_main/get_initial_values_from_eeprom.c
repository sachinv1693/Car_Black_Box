#include "main.h"

/* Function to read the initial values from the eeprom */
void get_initial_values_from_eeprom(void)
{
        //Get original password from eeprom storage (0x00)
        for (unsigned char i = 0; i < MAX_PASSCODE_LEN; i++)
        {
                pswd[i] = (read_internal_eeprom(i) - '0') ? '1' : '0';
        }
        pswd[MAX_PASSCODE_LEN] = '\0';

        //Get eeprom write address from the next address
        write_addr = read_internal_eeprom((unsigned char) MAX_PASSCODE_LEN);

        //Get rollover flag value from the next address 
        rollover = read_internal_eeprom((unsigned char) MAX_PASSCODE_LEN + 1);
        //Get value of first & last flag from next consecutive addresses
        first = read_internal_eeprom((unsigned char) MAX_PASSCODE_LEN + 2);
        last = read_internal_eeprom((unsigned char) MAX_PASSCODE_LEN + 3);
}

