/******************************************************************************
 *
 * Module: AtMega16_timers
 *
 * File Name: ATMEGA16_timers.c
 *
 * Description: Source file for the AVR ATMega16 timers driver
 * 				that file includes functions to operate atmega16 timers0
 * Future plan: add timer1, and timer2
 *
 * Note: enable global interrupt enable bit in application code
 *
 * Created on: Jun 9, 2022
 *
 * Author: Hassan Khodeir
 *
 *******************************************************************************/
#include "timers.h"

#include "std_types.h" /*to use standard types like uint8*/
#include "common_macros.h" /* To use the macros like SET_BIT */
#include <avr/io.h> /*for Timers register*/
#include <avr/interrupt.h>/*for Timers ISR*/
/*******************************************************************************
 *                           Global Variables                                  *
 *******************************************************************************/

/* Global variables to hold the address of the call back function in the application */
static volatile void (*g_timer0CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_timer1CallBackPtr)(void) = NULL_PTR;
static volatile void (*g_timer2CallBackPtr)(void) = NULL_PTR;

/*******************************************************************************
 *                      Interrupt handler                                      *
 *******************************************************************************/
ISR(TIMER0_OVF_vect){
	if(g_timer0CallBackPtr != NULL_PTR){
		(*g_timer0CallBackPtr)();
	}
}
ISR(TIMER0_COMP_vect){
	if(g_timer0CallBackPtr != NULL_PTR){
		(*g_timer0CallBackPtr)();
	}
}
/*future_plan => complete Timer1,2 handler*/



/*******************************************************************************
 *                      Functions Definition                                   *
 *******************************************************************************/
/*
 * Description : Function to initialize the Timer driver
 * 	1. Set the required clock.
 * 	2. Set the required Mode.
 * 	3. Set timer initial Value.
 * 	4. Set timer compare value
 * 	Function Input: timer_no => select between timers0,1,2
 * 					Timer configType => setup timer configuration by passing a structure's address
 * 	Note that Timer/Counter1 and Timer/Counter0 share the same prescaler
 */
void TIMERS_init(const TIMERS_ConfigType * Timer_Config_Ptr, TIMERS_Select timer_no){
	switch(timer_no)
	{
	case TIMER0_ID:
		/*
		 * TCCR0:
		 * BIT7 FOC0 => 1 non_PWM
		 * BIT3,6 WGM01:0 => Normal mode => 0,0
		 * BIT3,6 WGM01:0 => CTC mode => 1,0
		 * Bit 5:4 ??? COM01:0 => 0,0 non_PWM
		 * Bit 2:0 ??? CS02:0: Clock Select "prescalers"
		 */
		/*set timer0 to run on the required clock select*/
		TCCR0 = (0x07 & Timer_Config_Ptr->clock);
		/*set timer0 initial value*/
		TCNT0 = Timer_Config_Ptr->timer_init_value;
		switch(Timer_Config_Ptr->mode)
		{
		case OVERFLOW_MODE:
			/*set pin7 FOC0*/
			TCCR0 |= (1<<FOC0);
//			TCCR0 = (1<<FOC0) | (1<<CS02) | (1<<CS00);
			/*enable timer overflow interrupt and disable CTC interrupt*/
			TIMSK = (TIMSK & 0xFC) | (1<<TOIE0);
			break;
		case CTC_MODE:
			/*assign a compare match value to Output Compare Register ??? OCR0*/
			OCR0 = Timer_Config_Ptr->timer_Compare_value;
			/*set timer0 to operate on CTC mode*/
			TCCR0 |= (1<<FOC0) | (1<<WGM01);
			/*disable overflow interrupt and enable CTC interrupt*/
			TIMSK = (TIMSK & 0xFC) | (1<<OCIE0);
			break;
		}
		break;
	case TIMER1_ID:
		break;
	case TIMER2_ID:
		break;
	}
}

/*
 * Description: Function to set the Call Back function address for each timer.
 */
void TIMERS_setCallBack(void(*a_ptr)(void), TIMERS_Select timer_no){
	if (timer_no == TIMER0_ID)
		g_timer0CallBackPtr = a_ptr;
	else if(timer_no == TIMER1_ID)
		g_timer1CallBackPtr = a_ptr;
	else if(timer_no == TIMER2_ID)
		g_timer2CallBackPtr = a_ptr;
}

/*
 * Description: Function to clear a timer Value to start count from ZERO
 */
void clearTimerValue(TIMERS_Select timer_no){
	if (timer_no == TIMER0_ID)
		TCNT0 = 0;
	else if(timer_no == TIMER1_ID)
		TCNT1 = 0;
	else if(timer_no == TIMER2_ID)
		TCNT2 = 0;

}

/*
 * Description: Function to disable a timer
 */
void TIMERS_DeInit(TIMERS_Select timer_no){
	if (timer_no == TIMER0_ID){
		TCCR0 = 0;
		TIMSK &= 0xFC;
	}
	else if(timer_no == TIMER1_ID){
		TCCR1A = 0;
		TCCR1B = 0;
		TIMSK &= 0xC3;
	}
	else if(timer_no == TIMER2_ID)
	{
		TCCR2 = 0;
		TIMSK &= 0x3F;
	}

}


/*
 * Description: Function to set initial value for TIMER/COUNTER Register
 */
void setTimerValue(TIMERS_Select timer_no, uint16 a_value){
	if (timer_no == TIMER0_ID)
		TCNT0 = a_value;
	else if(timer_no == TIMER1_ID)
		TCNT1 = a_value;
	else if(timer_no == TIMER2_ID)
		TCNT2 = a_value;
}
