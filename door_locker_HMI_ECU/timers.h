/******************************************************************************
 *
 * Module: AtMega16_timers
 *
 * File Name: ATMEGA16_timers.h
 *
 * Description: Header file for the AVR ATMega16 timers driver
 *
 * Created on: Jun 9, 2022
 *
 * Author: Hassan Khodeir
 *
 * Note: This driver works only with Timer0 and as future plan it will handle All ATMega16 Timers
 *
 *******************************************************************************/

#ifndef TIMERS_H_
#define TIMERS_H_

#include "std_types.h"

#define ZERO_INIT_VALUE 0
#define NO_COMP_VALUE 0

/*******************************************************************************
 *                         Types Declaration                                   *
 *******************************************************************************/
typedef enum
{
	/*
	 * timer1 clock prescaler Bit 2:0 – CS12:0: Clock Select
	 * 0 => stop , 1 => F_CPU, 2, 3, 4, 5, 6
	 */
	NO_CLOCK,F_CPU_CLOCK,F_CPU_8,F_CPU_64,F_CPU_256,F_CPU_1024
}TIMERS_Clock;

typedef enum{
	TIMER0_ID, TIMER1_ID, TIMER2_ID
}TIMERS_Select;

typedef enum{
	OVERFLOW_MODE, CTC_MODE
}TIMERS_Mode;

typedef struct
{
	TIMERS_Clock clock;
	TIMERS_Mode mode;
	uint16 timer_init_value;
	uint16 timer_Compare_value;
}TIMERS_ConfigType;



/*******************************************************************************
 *                      Functions Prototypes                                   *
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
void TIMERS_init(const TIMERS_ConfigType * Timer_Config_Ptr, TIMERS_Select timer_no);

/*
 * Description: Function to set the Call Back function address for each timer.
 */
void TIMERS_setCallBack(void(*a_ptr)(void), TIMERS_Select timer_no);

/*
 * Description: Function to clear a timer Value to start count from ZERO
 */
void clearTimerValue(TIMERS_Select timer_no);

/*
 * Description: Function to disable a timer
 */
void TIMERS_DeInit(TIMERS_Select timer_no);

/*
 * Description: Function to set initial value for TIMER/COUNTER Register
 */
void setTimerValue(TIMERS_Select timer_no, uint16 a_value);


#endif /* TIMERS_H_ */
