# Car_Black_Box

Initially run the Makefile from CBB_eeprom_loader folder.
It stores the initial values in the internal eeprom.
After this, run the Makefile from CBB_main folder.
This starts executing the project onto PIC18F4580 micro-controller board. An on-board CLCD screen is used to create an interactive interface.

Initially a dashboard screen is displayed. It shows current time, last occurred event and vehicle speed. Vehicle speed can be varied using the on-board POT(potentiometer).
Matrix SW1 is used to register the Collision event.
Matrix SW2 is used to register the gear up event.
Matrix SW3 is used to register the gear down event.
These event types are stored onto internal eeprom at any time in any display mode. Note that other events are not displayed on dashboard. They are visible only in view log & download log operation.

Matrx SW11 when pressed once takes the user to password screen. Now the user has to provide the correct password. Initial password is "1010"(changeable). Same Matrix SW11 is used to write '1' while Matrix SW12 is used to write '0'. With every entry, user can just see a '*' on the screen. User has to provide the password within 5 sec(changeable). Otherwise dashboard screen will reopen. If password is correct then, a menu screen opens up. Here, user can scroll up and down using the same matrix SW11 & matrix SW12 key respectively.
If password does not match, 4(changeable) more tries are given to user. If user fails to provide the correct password after 5th try then, the screen will be locked for 30 sec(changeable). After 30 sec, user can retry again further 5 times.

Long pressing of matrix SW11 for 3 sec(changeable) performs the rsepective functionality.

In View log, one can see last 10 possible events stored in the data log.

If clear log is selected then a message appears on the screen saying that the log is cleared. Now, in the view log mode, only 1 'clear log' event is visble. Rest of the events will not be visible to user. Scrolling operation will be ineffective.

In Set time mode, one can set the time value. This is done using the same matrix SW11 & matrix SW12. Matrix SW11 is used to select Hr-Min-Sec field which keeps rotating when pressed again & again. The on-board RTC clock is configured in 24 Hr format. Matrix SW12 is used to increment the selected field value. 'Set time' operation is also considered as an event and stored into internal eeprom.

In 'change password' mode, one can set a new password. In this, after entering the password once, the user will be asked to re-enter the password. Now the user has to give the same pattern. Otherwise, it repeats enter & re-enter cycles. If user doesn't provide any input for 15 sec (changeable) then he/she will be taken back to menu screen. This operation is also considered as an event and stored into internal eeprom.

In 'download log', one has to open the serial terminal like minicom or PuTTY etc. on the PC and connect the board with it. After long pressing download log option, the data log will be printed on the serial monitor. This operation is also considered as an event and stored into internal eeprom.
