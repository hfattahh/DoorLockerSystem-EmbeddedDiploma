/******************************************************************************
 *
 * Module: UART
 *
 * File Name: uart.c
 *
 * Description: source file for the UART AVR driver
 *
 * Author: Hassan khodeir
 *
 * Created on: May 24, 2022
 *
 *******************************************************************************/

#include "uart.h"
#include "avr/io.h" /* To use the UART Registers */
#include "common_macros.h" /* To use the macros like SET_BIT */


/*
 * Description :
 * Functional responsible for Initialize the UART device by:
 * 1. Setup the Frame format like number of data bits, parity bit type and number of stop bits.
 * 2. Enable the UART.
 * 3. Setup the UART baud rate.
 */
void UART_init(uint32 baud_rate){
	uint16 a_ubrrValue = 0; /*to select the baud rate*/
	/*set registers UCSR1A, B, C*/

	/******** UCSRA ********************************************************
	 * 7    6   5   4   3  2   1   0
	 * RXC TXC UDRE FE DOR PE U2X MPCM
	 * most of those bits are flags
	 **********************************************************************/
	/* U2X = 1 for double transmission speed */
	UCSRA = (1<<U2X);


	/************************** UCSRB Description **************************
	 * 7       6     5    4    3     2    1    0
	 * RXCIE TXCIE UDRIE RXEN TXEN UCSZ2 RXB8 TXB8
	 *
	 * RXCIE = 0 Disable USART RX Complete Interrupt Enable
	 * TXCIE = 0 Disable USART TX Complete Interrupt Enable
	 * UDRIE = 0 Disable USART Data Register Empty Interrupt Enable
	 * RXEN  = 1 Receiver Enable
	 * RXEN  = 1 Transmitter Enable
	 * UCSZ2 = 0 For 8-bit data mode
	 * RXB8 & TXB8 not used for 8-bit data mode
	 ***********************************************************************/
	UCSRB = (1<<RXEN) | (1<<TXEN);

	/************************** UCSRC Description **************************
	 * URSEL   = 1 The URSEL must be one when writing the UCSRC
	 * UMSEL   = 0 Asynchronous Operation
	 * UPM1:0  = 00 Disable parity bit
	 * USBS    = 0 One stop bit
	 * UCSZ1:0 = 11 For 8-bit data mode
	 * UCPOL   = 0 Used with the Synchronous operation only
	 ***********************************************************************/
	UCSRC = (1<<URSEL) | (1<<UCSZ1) |(1<<UCSZ0);

	/* Calculate the UBRR register value */
	a_ubrrValue = F_CPU/16/baud_rate-1;
	/* First 8 bits from the BAUD_PRESCALE inside UBRRL and last 4 bits in UBRRH*/
	UBRRH = (unsigned char)(a_ubrrValue>>8);
	UBRRL = (unsigned char)a_ubrrValue;

}

/*
 * Description :
 * Functional responsible for send byte to another UART device.
 */
void UART_sendByte(const uint8 data){

	/* Wait for empty transmit buffer */
	while(BIT_IS_CLEAR(UCSRA, UDRE));
	/*another method found in ATMEGA16 data sheet example*/
	/*	while ( !( UCSRA & (1<<UDRE)) ); */
	/* Put data into buffer, sends the data */
	UDR = data;
}

/*
 * Description :
 * Functional responsible for receive byte from another UART device.
 */
uint8 UART_recieveByte(void){
	/* Wait for data to be received */
	/*busy loop*/
	while ( !(UCSRA & (1<<RXC)) )
		;
	/* Get and return received data from buffer */
	return UDR;

}

/*
 * Description :
 * Send the required string through UART to the other UART device.
 */
void UART_sendString(const uint8 *Str){

}
/*
 * Description :
 * Receive the required string until the '#' symbol through UART from the other UART device.
 */
void UART_receiveString(uint8 *Str){
	/*Receive until # */
}
