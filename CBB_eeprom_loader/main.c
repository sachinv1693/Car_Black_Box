#include "main.h"

/* Save initial configurations onto eeprom from main function */
void main()
{
		//Write initial passcode onto eeprom as "1010"
		write_internal_eeprom(0x00, '1');
		write_internal_eeprom(0x01, '0');
		write_internal_eeprom(0x02, '1');
		write_internal_eeprom(0x03, '0');
		//Write initial event storing address as 0x08 as we are using 8 bytes
		write_internal_eeprom(0x04, 0x08);
		//Write initial rollover flag value as 0
		write_internal_eeprom(0x05, 0x00);
		//Write first event index as 0
		write_internal_eeprom(0x06, 0x00);
		//Write last event index as 0
		write_internal_eeprom(0x07, 0x00);
}
