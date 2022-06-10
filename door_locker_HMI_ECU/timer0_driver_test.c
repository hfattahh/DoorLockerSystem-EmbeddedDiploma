/******************************************************************************
 *
 * Module: AtMega16_timers
 *
 * File Name: timer0_driver_test.c
 *
 * Description: 1st test file to test timer driver by applying delay using over flow mode, and CTC mode
 *
 * Created on: Jun 10, 2022
 *
 * Author: Hassan Khodeir
 *
 *******************************************************************************/

#include <avr/io.h> /*to use register such as SREG*/
#include "timers.h"
#include "std_types.h"
#include "common_macros.h"
#include "gpio.h"

#define OVERFLOW_TEST
#define no_tics_per_second_in_overflow 4

uint8 g_overflowCounter = 0;

void wait_using_overflow(void){
	/* assign TCNT0 to 5 after every overflow, so overflow = 250msec */
	TCNT0 = 5;
	if(g_overflowCounter >= no_tics_per_second_in_overflow)
		g_overflowCounter = 0;

	g_overflowCounter++;
}





int main(){

	/*set PC0 pin as output and initialize it with logic low*/
	GPIO_setupPinDirection(PORTC_ID, PIN0_ID, PIN_OUTPUT);
	GPIO_writePin(PORTC_ID, PIN0_ID, LOGIC_LOW);

	/*enable i-bit*/
	SREG |= (1<<7);
	/*Configuration structure for timer0 overflow mode*/
	TIMERS_ConfigType timer0_overflow_config = {F_CPU_1024, OVERFLOW_MODE, 5, NO_COMP_VALUE};
	/*Configuration structure for timer0 CTC mode*/
	TIMERS_ConfigType timer0_CTC_config = {F_CPU_1024, CTC_MODE, ZERO_INIT_VALUE, 1000};
#ifdef OVERFLOW_TEST
	TIMERS_init(&timer0_overflow_config, TIMER0_ID);
	TIMERS_setCallBack(wait_using_overflow, TIMER0_ID);
#elif CTC_TEST

#endif


	for(;;){
#ifdef OVERFLOW_TEST
		if(g_overflowCounter >= no_tics_per_second_in_overflow){
			TOGGLE_BIT(PORTC, 0);
		}
#elif CTC_TEST

#endif

	}

}
