 /******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dcmotor.h
 *
 * Description: Header file for the DC motor with AVR
 *
 * Author: Hassan Khodeir
 *
 * Created on: May 25, 2022
 *
 *******************************************************************************/
#ifndef DCMOTOR_H_
#define DCMOTOR_H_

#include"std_types.h"
#include"gpio.h"
#include"common_macros.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define DCMOTOR_PORT_ID PORTB_ID
#define DCMOTOR_IN1 PIN0_ID
#define DCMOTOR_IN2 PIN1_ID

/************************************************************************
 *						type definition
 ************************************************************************/
typedef enum{
	DC_STOP, DC_A_CW, DC_CW
}DcMotor_State;


/*
 * Function to initialize one specific motor
 */

void DcMotor_Init(void);


/*
 * Function to rotate DC motor CW or CCW with a given speed
 */
void DcMotor_Rotate(DcMotor_State state);
/*
 * function to return DC motor state
 */
DcMotor_State DcMotor_return_motor_state(void);


#endif /* DCMOTOR_H_ */


