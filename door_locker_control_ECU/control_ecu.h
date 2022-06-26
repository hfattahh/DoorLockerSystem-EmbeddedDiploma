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

#endif /* CONTROL_ECU_H_ */
