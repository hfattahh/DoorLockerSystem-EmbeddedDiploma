/******************************************************************************
 *
 * Module: DC Motor
 *
 * File Name: dcmotor.c
 *
 * Description: Source file for the DC motor with AVR
 *
 * Author: Hassan Khodeir
 *
 * Created on: May 25, 2022
 *
 *******************************************************************************/
#include"dcmotor.h"
#include"gpio.h"
/*******************************************************************************
 *                                Global Variables                             *
 *******************************************************************************/
static DcMotor_State g_DcMotor_state = 0;
/*
 *
 */

/*******************************************************************************
 *                                Function Definitions                         *
 *******************************************************************************/
/*
 * Function to initialize one specific motor
 */

void DcMotor_Init(void){
	GPIO_setupPinDirection(DCMOTOR_PORT_ID, DCMOTOR_IN1, PIN_OUTPUT);
	GPIO_setupPinDirection(DCMOTOR_PORT_ID, DCMOTOR_IN2, PIN_OUTPUT);

	GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN1, LOGIC_LOW);
	GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN2, LOGIC_LOW);
}


/*
 * Function to rotate DC motor CW or CCW with a given speed
 */
void DcMotor_Rotate(DcMotor_State state){
	switch(state)
	{
	case DC_STOP:
		GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN1, LOGIC_LOW);
		GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN2, LOGIC_LOW);

		break;
	case DC_CW:
		GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN1, LOGIC_LOW);
		GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN2, LOGIC_HIGH);

		break;
	case DC_A_CW:
		GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN1, LOGIC_HIGH);
		GPIO_writePin(DCMOTOR_PORT_ID, DCMOTOR_IN2, LOGIC_LOW);
		break;
	}
	g_DcMotor_state = state;

}
DcMotor_State DcMotor_return_motor_state(void){
	return g_DcMotor_state;

}
