/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: header file for the Buzzer
 *
 * Author: Hassan Khodeir
 *
 * Created on: Jun 29, 2022
 *
 *******************************************************************************/
#ifndef BUZZER_H_
#define BUZZER_H_

#include"std_types.h"
#include"gpio.h"
#include"common_macros.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/
#define BUZZER_PORT_ID PORTB_ID
#define BUZZER_PIN PIN7_ID



/*******************************************************************************
 *							function Declaration							   *
 ******************************************************************************/
void BUZZER_INIT(void);
void BUZZER_on(void);
void BUZZER_off(void);




#endif /* BUZZER_H_ */
