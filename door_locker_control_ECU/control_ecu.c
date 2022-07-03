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
#include "dcmotor.h"
#include "external_eeprom.h"
#include "twi.h"

#define TESTING_PAHSE

/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/
uint8 g_timer0_ticks = 0;

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

	/*enable i-bit*/
	SREG |= (1<<7);
	/*Configuration structure for timer0 CTC mode*/
	TIMERS_ConfigType timer0_overflow_config = {F_CPU_1024, OVERFLOW_MODE, 5, NO_COMP_VALUE};
	/*initialize timer */
	TIMERS_init(&timer0_overflow_config, TIMER0_ID);
	/*set call back function for timer0*/
	TIMERS_setCallBack(timer0_handler, TIMER0_ID);

	/* Initialize the TWI/I2C Driver */
	TWI_init();

	/*initialize DC motor */
	DcMotor_Init();


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
			/*****store password to external EEPROM*****/
			 /* change password setting state to 1 in a specific address in the external EEPROM */
			 /* Write 0x0F in the external EEPROM */
			EEPROM_writeByte(0x0311, 0x01);

			break;
		case CHECK_PASSWORD_MATCH:
			UART_sendByte(EUC2_READY);
			receive_password_from_HMI(password);
			/*call match password function*/
			UART_sendByte(check_password_match(password));
			break;
		case OPEN_DOOR:
			/*open door for 3 seconds*/
			open_door();

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
	uint8 status = 1;

	/*write 1 just for test*/
	EEPROM_writeByte(0x0311, 0x01);
	_delay_ms(10);
	/* Read 0x0F from the external EEPROM */
	EEPROM_readByte(0x0311, &status);
	return status; /*just in testing phase*/
}

/******************************************************************
 * [Function Name] : receive_password_from_HMI
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
/******************************************************************
 * [Function Name] : check_password_match
 * [Description] : check if password match the saved password in eeprom
 * [Args] : non
 * [in] uint8 *pass: pointer to string of password that required to be checked
 * [Returns] : password matching status
 ****************************************************/
uint8 check_password_match(uint8 *pass)
{
#ifdef TESTING_PAHSE
	uint8 password [PASSCODE_SIZE]= {'2','2','2','2','2','\0'};
#elif
	uint8 password[PASSCODE_SIZE]; /*to get passord from eeprom*/
#endif
	/*
	 * password => get stored password from EEPROM
	 */
	uint8 index;
	for(index = 0; index < PASSCODE_SIZE ; index++)
	{
		if(password[index] != pass[index]){
			return PASSWORD_NOT_MATCH;
		}
	}
	return PASSWORD_MATCH;
}

/******************************************************************
 * [Function Name] : timer0_handler
 * [Description] : function to handle
 * [Args] : non
 * [in]: non
 * [Returns] : non
 ****************************************************/
void timer0_handler(void){
	setTimerValue(TIMER0_ID, 5);
	g_timer0_ticks++;
}
/******************************************************************
 * [Function Name] : open_door
 * [Description] : function to open the door using DC-motor
 * [Args] : non
 * [in]: non
 * [Returns] : non
 ****************************************************/
void open_door(void){
	/*rotate motor for 15 seconds CW*/
	g_timer0_ticks = 0;
	DcMotor_Rotate(DC_CW);
	while(g_timer0_ticks < 60);

	/*hold motor for 3 seconds */
	g_timer0_ticks = 0;
	DcMotor_Rotate(DC_STOP);
	while(g_timer0_ticks < 12);

	/*rotate motor for 15 seconds CCW*/
	g_timer0_ticks = 0;
	DcMotor_Rotate(DC_A_CW);
	while(g_timer0_ticks < 60);

	/*Stop Motor*/
	DcMotor_Rotate(DC_STOP);
}
