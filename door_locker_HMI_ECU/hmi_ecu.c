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
uint8 password[PASSCODE_SIZE] = {'\0'};


int main(void){
	/*password[]  -> array holds the entered password by user
	 *confirmPassword[] -> array holds the confirmation password that entered by user
	 *receivedByte     -> variable to hold byte that received from master throw UART
	 *
	 */
	uint8 password[PASSCODE_SIZE] = {'\0'};
	uint8 confirmPassword[PASSCODE_SIZE] = {'\0'};
	uint8 receivedByte = DUMMY_CHAR;

	/*initialize the LCD*/
	LCD_init();
	/*initialize UART with baud rate 9600*/
	UART_init(9600);

	/*send check setting status command*/
	UART_sendByte(CHECK_PASSWORD_SETTING);

	/*receive if password set or not*/
	receivedByte = UART_recieveByte();
	/*if not set  call function set password*/
	switch(receivedByte){
	/*********************setting password*********************/
	case PASSWORD_NOT_SET:
		/*enter password by user and store it in an array*/
		enter_passcode(password);
		/*confirm password by user and store it in an array*/
		enter_passcode(confirmPassword);
		/*send set password command to control ECU*/
		UART_sendByte(SET_PASSWORD);
		/*wait to control ECU to be ready*/
		while(!(UART_recieveByte() == EUC2_READY));
		/*send password to control ECU*/
		send_password_to_control_ECU(password);
		while(!(UART_recieveByte() == EUC2_READY));
		/*send password to control ECU*/
		send_password_to_control_ECU(confirmPassword);
		break;
	default:
		break;
	}
#ifdef TESTING_PAHSE
	LCD_clearScreen();
	LCD_displayString("hello visitor!");
#endif


	while(1)
	{
	}
}



/*******************************************************************************
 *                      Functions Definition                                   *
 *******************************************************************************/
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
 * [Function Name] : set_password
 * [Description] : get password, confirm password, check if 2 password is identical
 * 				   if identical => continue
 * 				   else => return
 * [Args] :
 * [in]        No Input
 * [Returns] : No Returns
 *
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





