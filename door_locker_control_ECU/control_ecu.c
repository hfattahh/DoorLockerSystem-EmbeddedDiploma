/******************************************************************************
 *
 * Project: Door Locker System
 *
 * File Name: control_ecu.c
 *
 * Description: Source file for the control ECU that plays as back-end server in door locker system
 *
 * Author: Hassan Khodeir
 *
 * Created on: Jun 10, 2022
 *
 *******************************************************************************/
#include "control_ecu.h"
#include "std_types.h" /*to use standard types like uint8*/
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "timers.h"
#include <avr/io.h> /*for AVR registers*/
#include "util/delay.h"
#include "uart.h"
#include "door_locker_protocol.h"

#define DEVELOPMENT_PHASE

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/
uint8 password[PASSCODE_SIZE] = {'\0'};


int main(void){
	uint8 receviedByte, passwordSettingState;
	UART_init(9600);


	while(1)
	{
		/*receive byte from HMI ECU*/
		receviedByte = UART_recieveByte();

		/*switch case on received byte*/
		switch(receviedByte)
		{
		case CHECK_PASSWORD_SETTING:
			/*call check password setting function*/
			passwordSettingState = check_password_setting_status();
			/*if password setting state = 1 > send(set)
			 * else if = 0 > send(not set)
			 */
			if(passwordSettingState == 1){
				UART_sendByte(PASSWORD_SET);
			}
			else if (passwordSettingState){
				UART_sendByte(PASSWORD_NOT_SET);
			}
			break;
		case CHECK_PASSWORD_MATCH:
			/*call match password function*/
			break;
		case SET_PASSWORD:
			/*receive to passwords form HMI,*/
		case OPEN_DOOR:
			/*open door for 1 minutes*/
			break;
		default:
			break;
		}
		receviedByte  = DUMMY_CHAR;


	}
}



/*******************************************************************************
 *                      Functions Definition                                   *
 *******************************************************************************/
/*------------------------------------------------------------------------------------------------------------------
 * [Function Name] : check_password_setting_status
 * [Description] : check on specific address if the password is set or not
 * [Args] : non
 * [in]: non
 * [Returns] :
 *                    Return 1 if password set
 *                    Return 0 if password is not set
 *
 *
 */
uint8 check_password_setting_status(void)
{
	/*check EEPROM Specific Address*/
	/*if there is one => return 1; (set)
	 *if there is 0 => return 0 (not set)
	 */
	return 1; /*just in testing phase*/
}
