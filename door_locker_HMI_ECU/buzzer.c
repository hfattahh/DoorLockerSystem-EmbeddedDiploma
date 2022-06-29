/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.c
 *
 * Description: Source file for the Buzzer
 *
 * Author: Hassan Khodeir
 *
 * Created on: Jun 29, 2022
 *
 *******************************************************************************/
#include "std_types.h"
#include "buzzer.h"
#include "gpio.h"

/*******************************************************************************
 *							function Definition							   *
 ******************************************************************************/
void BUZZER_INIT(void)
{
	GPIO_setupPinDirection(BUZZER_PORT_ID, BUZZER_PIN, PIN_OUTPUT);
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN, LOGIC_LOW);

}
void BUZZER_on(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN, LOGIC_HIGH);
}
void BUZZER_off(void)
{
	GPIO_writePin(BUZZER_PORT_ID, BUZZER_PIN, LOGIC_LOW);
}
