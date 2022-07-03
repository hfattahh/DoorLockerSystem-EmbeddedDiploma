/******************************************************************************
 *
 * Module		: TWI(I2C)
 *
 * File Name	: twi.c
 *
 * Description	: Source file for the TWI(I2C) AVR driver
 *
 * Author		: Hassan Khdoeir
 *
 * Created on	: May 30, 2022
 *
 *******************************************************************************/
#include"twi.h"
#include<avr/io.h>
#include"common_macros.h"


void TWI_init(void){



    /* Bit Rate: 400.000 kbps using zero pre-scaler TWPS=00 and F_CPU=8Mhz */
    TWBR = 0x02;

	/*TWSR
	 * TWI Status Register – TWSR
	 *7     6    5 	   4   3   2   1    0
	 *TWS7 TWS6 TWS5 TWS4 TWS3 – TWPS1 TWPS0
	 */
	TWSR = 0x00; /*TWI Bit Rate Prescaler*/

	/*TWCR
	 * TWI Control Register – TWCR
	 *   7    6 	5 	  4 	3 	2	 1	 0
	 * TWINT TWEA TWSTA TWSTO TWWC TWEN  –  TWIE
	 */
	TWCR = (1<<TWEN); /*enable TWI*/

	/*TWAR
	 * TWI (Slave) Address Register – TWAR
	 */
	TWAR = 0x02; /*set my address as address 1 (0b00000010)*/

 }
void TWI_start(void)
{
	/*TWCR
	 * TWI Control Register – TWCR
	 *   7    6 	5 	  4 	3 	2	 1	 0
	 * TWINT TWEA TWSTA TWSTO TWWC TWEN  –  TWIE
	 */
    /*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the start bit by TWSTA=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1<<TWINT) | (1<<TWEN) |  (1<<TWSTA);

    /* Wait for TWINT flag set in TWCR Register (start bit is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));

}
void TWI_stop(void)
{
    /*
	 * Clear the TWINT flag before sending the start bit TWINT=1
	 * send the stop bit by TWSTO=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1<<TWINT) | (1<<TWEN) |  (1<<TWSTO);
}
void TWI_writeByte(uint8 data){

    /* Put data On TWI data Register */
    TWDR = data;
    /*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
    TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register(data is send successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
}
uint8 TWI_readByteWithACK(void){

    /*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 * Enable ACK bit TWEA = 1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN) | (1<< TWEA);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
	return TWDR;

}
uint8 TWI_readByteWithNACK(void)
{
    /*
	 * Clear the TWINT flag before sending the data TWINT=1
	 * Enable TWI Module TWEN=1
	 */
	TWCR = (1 << TWINT) | (1 << TWEN);
    /* Wait for TWINT flag set in TWCR Register (data received successfully) */
    while(BIT_IS_CLEAR(TWCR,TWINT));
    /* Read Data */
	return TWDR;

}
uint8 TWI_getStatus(void){
	uint8 status;
	/* masking to eliminate first 3 bits and get the last 5 bits (status bits) */
	status = (TWSR & 0xF8);

	return status;
}
