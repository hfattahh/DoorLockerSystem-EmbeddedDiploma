/******************************************************************************
 *
 * File Name: door_locker_protocol.h
 *
 * Description: Header file for Special characters used in communication between HMI and control ECUs
 *
 * Created on: Jun 24, 2022
 *
 * Author: Hassan Khodeir
 *
 *******************************************************************************/
#ifndef DOOR_LOCKER_PROTOCOL_H
#define DOOR_LOCKER_PROTOCOL_H

/*pass code size*/
#define PASSCODE_SIZE 6

/*check if the password is set special characters*/
#define CHECK_PASSWORD_SETTING 'S'
#define PASSWORD_SET 'Y'
#define PASSWORD_NOT_SET 'N'

/*set password Special characters*/
#define SET_PASSWORD '-'

/*Micro-controllers is ready*/
#define EUC1_READY '?'
#define EUC2_READY '@'


/*check if the passwords are identical special characters*/
#define PASSWORD_IDENTICAL ')'
#define PASSWORD_NOT_IDENTICAL '('

/*check if the password match special characters*/
#define CHECK_PASSWORD_MATCH 'M'
#define PASSWORD_MATCH 'y'
#define PASSWORD_NOT_MATCH 'n'

/*open the door Special characters*/
#define OPEN_DOOR '+'

/*dummy character used to assign to received byte, same case does not repeated*/
#define DUMMY_CHAR 'D'




#endif /*DOOR_LOCKER_PROTOCOL_H*/
