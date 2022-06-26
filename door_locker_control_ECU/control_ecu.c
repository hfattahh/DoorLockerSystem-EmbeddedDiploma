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
	/* password[]  			-> array holds the entered password by user
	 * confirmPassword[] 	-> array holds the confirmation password that entered by user
	 * receivedByte      	-> variable to hold byte that received from master throw UART
	 * passwordSettingState	-> uint8 variable to store if password is setting before or not
	 * index				-> index used as for loop counter
	 *
	 */
	uint8 password[PASSCODE_SIZE] = {'\0'};
	uint8 confirmPassword[PASSCODE_SIZE] = {'\0'};
	uint8 receivedByte = DUMMY_CHAR;
	uint8 passwordSettingState;
	uint8 index;

	/*initialize UART with baud rate 9600*/
	UART_init(9600);


	while(1)
	{
		/*receive byte from HMI ECU*/
		receivedByte = UART_recieveByte();

		/*switch case on received byte*/
		switch(receivedByte)
		{
		case CHECK_PASSWORD_SETTING:
			/*call check password setting function*/
			passwordSettingState = check_password_setting_status();
			/*if password setting state = 1 > send(set)
			 * else if = 0 > send(not set)
			 */
			if(passwordSettingState){
				UART_sendByte(PASSWORD_SET);
			}
			else if (!passwordSettingState){
				UART_sendByte(PASSWORD_NOT_SET);
			}
			break;
		case SET_PASSWORD:
			/*receive two passwords form HMI*/
			UART_sendByte(EUC2_READY);
			receive_password_from_HMI(password);
			UART_sendByte(EUC2_READY);
			receive_password_from_HMI(confirmPassword);
			for(index = 0; index < PASSCODE_SIZE ; index++)
			{
				if(password[index] != confirmPassword[index]){
					UART_sendByte(PASSWORD_NOT_IDENTICAL);
					break;
				}
			}
			UART_sendByte(PASSWORD_IDENTICAL);
			break;
		case CHECK_PASSWORD_MATCH:
			/*call match password function*/
			break;
		case OPEN_DOOR:
			/*open door for 1 minutes*/
			break;
		default:
			break;
		}
		receivedByte  = DUMMY_CHAR;


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
	return 0; /*just in testing phase*/
}

/******************************************************************
 * [Function Name] : receivePass
 * [Description] : This function receive password from HMI by UART module
 * [Args] : non
 * [in] uint8 *pass: pointer to string to store the received password
 * [Returns] :
 ****************************************************/
void receive_password_from_HMI(uint8 *pass)
{
	uint8 i;
	for(i = 0; i<PASSCODE_SIZE;i++){
		pass[i] = UART_recieveByte();
	}
}
