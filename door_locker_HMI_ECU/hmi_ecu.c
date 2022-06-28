/******************************************************************************
 *
 * Project: Door Locker System
 *
 * File Name: hmi_ecu.c
 *
 * Description: Source file for the hmi_uc that responsible to interface with user
 *
 * Author: Hassan Khodeir
 *
 * Created on: Jun 10, 2022
 *
 *******************************************************************************/
#include "hmi_ecu.h"
#include "std_types.h" /*to use standard types like uint8*/
#include "common_macros.h" /* To use the macros like SET_BIT */
#include "timers.h"
#include <avr/io.h> /*for AVR registers*/
#include "keypad.h"
#include "lcd.h"
#include "util/delay.h"
#include "door_locker_protocol.h"
#include "uart.h"

#define TESTING_PAHSE



/*******************************************************************************
 *                      Global Variables                                       *
 *******************************************************************************/
/*
 * password -> array to hold password
 * g_ticks	-> global variable to conunt timer ticks
 *
 */
uint8 password[PASSCODE_SIZE] = {'\0'};
uint8 g_ticks = 0;



int main(void)
{
	/* password[]  		-> array holds the entered password by user
	 * confirmPassword[]-> array holds the confirmation password that entered by user
	 * receivedByte     -> variable to hold byte that received from master throw UART
	 * userInput		-> variable to get user input + or -
	 * passwordCounter	-> variable to store numbers of wrong entered
	 *
	 */
	uint8 password[PASSCODE_SIZE] = {'\0'};
	uint8 confirmPassword[PASSCODE_SIZE] = {'\0'};
	uint8 receivedByte = DUMMY_CHAR;
	uint8 userInput = DUMMY_CHAR;
	uint8 passwordCounter = 0;

	/*initialize the LCD*/
	LCD_init();
	/*initialize UART with baud rate 9600*/
	UART_init(9600);

	/*send check setting status command*/
	UART_sendByte(CHECK_PASSWORD_SETTING);

	/*receive if password set or not*/
	receivedByte = UART_recieveByte();
	/*if not set then, set password*/
	if(receivedByte == PASSWORD_NOT_SET)
	{
		/*repeat entering password until user enter 2 identical passwords*/
		receivedByte = PASSWORD_NOT_IDENTICAL;

		do{
			/*********************setting password*********************/
			/*enter password by user and store it in an array*/
			enter_passcode(password);
			/*confirm password by user and store it in an array*/
			enter_passcode(confirmPassword);
			/******* SEND passwords to control ECU *******/
			/*send set password command to control ECU*/
			UART_sendByte(SET_PASSWORD);
			/***wait to control ECU to be ready***/
			while(!(UART_recieveByte() == EUC2_READY));
			/*send password to control ECU*/
			send_password_to_control_ECU(password);
			/***wait to control ECU to be ready***/
			while(!(UART_recieveByte() == EUC2_READY));
			/*send password to control ECU*/
			send_password_to_control_ECU(confirmPassword);
			receivedByte = UART_recieveByte();
#ifdef TESTING_PAHSE
			LCD_clearScreen();
			LCD_displayString("PASSWORD NOT IDENTICAL!");
#endif
		}
		while(receivedByte == PASSWORD_NOT_IDENTICAL);
	}
#ifdef TESTING_PAHSE
	LCD_clearScreen();
	LCD_displayString("hello visitor!");
#endif


	while(1)
	{
		/*clear receivedByte variable*/
		receivedByte = DUMMY_CHAR;
		/*
		 * display +, and -
		 * wait for user input
		 * check the password
		 * count wrong entered password
		 * serve the user if correct password was entered
		 * **** the user if wrong password was entered
		 */
		LCD_clearScreen();
		LCD_displayString("+Open Door");
		LCD_moveCursor(1, 0);
		LCD_displayString("-Change Password");
		/*get user input using
		 *KEYPAD_getPressedKey() function containing a tight polling
		 */
		userInput = KEYPAD_getPressedKey();

		/************************* serve user request ******************************/
		if(userInput == OPEN_DOOR || userInput == SET_PASSWORD)
		{
			while(passwordCounter < 3)
			{
				/*enter password by user and store it in an array*/
				enter_passcode(password);
				UART_sendByte(CHECK_PASSWORD_MATCH);
				/***wait to control ECU to be ready***/
				while(!(UART_recieveByte() == EUC2_READY));
				/*send password to control ECU*/
				send_password_to_control_ECU(password);
				/*receive byte */
				receivedByte = UART_recieveByte();
				/*if password match serve the user*/
				if(receivedByte == PASSWORD_MATCH)
				{

					switch(userInput)
					{
					case OPEN_DOOR:
						/*send open_door command throw UART*/
						UART_sendByte(OPEN_DOOR);
						LCD_clearScreen();
						LCD_displayString("Opening the door");
						_delay_ms(30000);
						break;
					case SET_PASSWORD:
						/*dispaly setup new pass*/
						LCD_clearScreen();
						LCD_displayString("Set new pass");
						_delay_ms(50);
						/*repeat entering password until user enter 2 identical passwords*/
						receivedByte = PASSWORD_NOT_IDENTICAL;

						do
						{
							/*********************setting password*********************/
							/*enter password by user and store it in an array*/
							enter_passcode(password);
							/*confirm password by user and store it in an array*/
							enter_passcode(confirmPassword);
							/******* SEND passwords to control ECU *******/
							/*send set password command to control ECU*/
							UART_sendByte(SET_PASSWORD);
							/***wait to control ECU to be ready***/
							while(!(UART_recieveByte() == EUC2_READY));
							/*send password to control ECU*/
							send_password_to_control_ECU(password);
							/***wait to control ECU to be ready***/
							while(!(UART_recieveByte() == EUC2_READY));
							/*send password to control ECU*/
							send_password_to_control_ECU(confirmPassword);
							/*receive password IDENTICAL status from control ECU*/
							receivedByte = UART_recieveByte();
#ifdef TESTING_PAHSE
							LCD_clearScreen();
							LCD_displayString("PASSWORD NOT IDENTICAL!");
#endif
						}
						while(receivedByte == PASSWORD_NOT_IDENTICAL);
						break;
					default:
						break;
					}/*end switch-case*/
					break; /*break while loop if password match*/
				}/*end if password match*/
				/*else if password does not match increment password counter until 3*/
				else
				{
					passwordCounter++;
				}
			}/*end while password counter <=3 */
		}
		/*warning if wrong password entered for 3 times*/
		if(passwordCounter >= 3){
			/*Warning for 1 minute*/
			/*initialize timer
			 * wait for one minute
			 */
			while(g_ticks < 4 ){
				LCD_clearScreen();
				LCD_displayString("#####################");
				_delay_ms(1000);
			}
		}

	}/*end super loop*/
}



/*******************************************************************************
 *                      Functions Definition                                   *
 *******************************************************************************/
/************************************************************************************************
 * [Function Name] : enter_passcode
 * [Description] : get Password from user
 * [Args] :
 * [in]       : uint8* passCodeString , pointer to string to store entered password
 * [Returns] : No Returns
 *
 *
 **************************************************************************************************/
void enter_passcode(uint8* passCodeString){
	int key;
	LCD_clearScreen();
	LCD_displayString("Enter Pass pls");
	LCD_moveCursor(1, 0);
	for(key = 0; key < PASSCODE_SIZE-1; key++)
	{
		/*save the pressed button on keypad to pass code array*/
		passCodeString[key] = KEYPAD_getPressedKey();
		LCD_displayCharacter(passCodeString[key]);
		_delay_ms(220);
	}
	LCD_clearScreen();
	passCodeString[PASSCODE_SIZE-1] = '\0';

}

/*
 * function to check if the password was set or not
 * if set => continue
 * not set => set the password
 * function to
 */

/************************************************************************************************
 * [Function Name] 	: send_password_to_control_ECU
 * [Description] 	: function to send string to control ECU
 * [Args] 			:
 * [in]        		: pointer to string (password)
 * [Returns] 		: No Returns
 *
 **************************************************************************************************/
void send_password_to_control_ECU(uint8* pass)
{
	uint8 i;
	for(i = 0; i<PASSCODE_SIZE;i++){
		UART_sendByte(*(pass+i));
		_delay_us(10);
	}
}
void test_enter_passcode(uint8* passCodeString){
	LCD_clearScreen();
	LCD_displayString("Entered PassCode:");
	LCD_moveCursor(1, 0);
	LCD_displayString(passCodeString);
}





