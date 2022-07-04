/******************************************************************************
 *
 * Project: Door Locker System
 *
 * File Name: control_ecu.h
 *
 * Description: header file for the control UC
 *
 * Author: Hassan Khodeir
 *
 * Created on: Jun 25, 2022
 *
 *******************************************************************************/
#ifndef CONTROL_ECU_H_
#define CONTROL_ECU_H_

#include "std_types.h"

#define SET_PASSWORD_STATUS_ADDRESS 0x0311
#define PASSWORD_START_ADDRESS 0x0312

/***********************************************************************
 * [Function Name] : check_password_setting_status
 * [Description] : check on specific address if the password is set or not
 * [Args] : non
 * [in]: non
 * [Returns] :
 *                    Return 1 if password set
 *                    Return 0 if password is not set
 *************************************************************************/

uint8 check_password_setting_status(void);
/******************************************************************
 * [Function Name] : receivePass
 * [Description] : This function receive password from HMI by UART module
 * [Args] : non
 * [in] uint8 *pass: pointer to string to store the received password
 * [Returns] :
 ****************************************************/
void receive_password_from_HMI(uint8 *pass);

/******************************************************************
 * [Function Name] : check_password_match
 * [Description] : check if password match the saved password in eeprom
 * [Args] : non
 * [in] uint8 *pass: pointer to string of password that required to be checked
 * [Returns] : password matching status
 ****************************************************/
uint8 check_password_match(uint8 *pass);

/******************************************************************
 * [Function Name] : timer0_handler
 * [Description] : function to handle
 * [Args] : non
 * [in]: non
 * [Returns] : non
 ****************************************************/
void timer0_handler(void);

/******************************************************************
 * [Function Name] : open_door
 * [Description] : function to open the door using DC-motor
 * [Args] : non
 * [in]: non
 * [Returns] : non
 ****************************************************/
void open_door(void);


#endif /* CONTROL_ECU_H_ */
