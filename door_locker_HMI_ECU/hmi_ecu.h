/******************************************************************************
 *
 * Module: Door Locker System
 *
 * File Name: hmi_ecu_fuuctions.h
 *
 * Description: Header file for HMI ECU functions
 *
 * Author: Hassan Khodeir
 *
 * Created on: Jun 10, 2022
 *
 *******************************************************************************/
#ifndef HMI_ECU_FUCTIONS_H_
#define HMI_ECU_FUCTIONS_H_

#include "std_types.h"


/*******************************************************************************
 *                      Functions Prototypes                                   *
 *******************************************************************************/
void enter_passcode(uint8*);
void test_enter_passcode(uint8* passCodeString);

void send_password_to_control_ECU(uint8* pass);



#endif /* HMI_ECU_FUUCTIONS_H_ */
