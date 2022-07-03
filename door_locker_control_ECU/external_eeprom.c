/******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the External EEPROM Memory
 *
 * Author: Hassan Khodeir
 *
 * Created on: May 31, 2022
 *
 *******************************************************************************/
#include "external_eeprom.h"
#include "twi.h"

/*******************************************************************************
 *                      Functions Definition                                   *
 *******************************************************************************/

uint8 EEPROM_writeByte(uint16 u16addr,uint8 u8data)
{
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write)
	 * EEPROM memory was set up as fixed address beginning with 0xA0
	 * 11 bit address for memory location divides 3bits and 8 to avoid sending 2bytes for memory addressing
	 * (Fixed 1010+ 3 MSB bits + R/W-)(the 8 LSB bits in memory address)
	 */
	TWI_writeByte(0xA0 | (uint8)((u16addr&0x0700)>>7));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;
	/* Send the required memory location address */
	TWI_writeByte((uint8)(u16addr & 0x00FF));
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* write byte to EEPROM */
	TWI_writeByte(u8data);
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;
	/* Send the Stop Bit */
	TWI_stop();
	/*return success*/
	return SUCCESS;
}



uint8 EEPROM_readByte(uint16 u16addr,uint8 *u8data)
{
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return ERROR;
	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=1 (write)
	 * EEPROM memory was set up as fixed address beginning with 0xA0
	 * 11 bit address for memory location divides 3bits and 8 to avoid sending 2bytes for memory addressing
	 * (Fixed 1010+ 3 MSB bits + R/W-)(the 8 LSB bits in memory address)
	 */
	TWI_writeByte((uint8)((0xA0) | ((u16addr & 0x0700)>>7) | 1));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;
	/* Send the required memory location address */
	TWI_writeByte((uint8)(u16addr & 0x00FF));
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;
	/***********************Repeat start*************************/
	TWI_start();
	if (TWI_getStatus() != TWI_REP_START)
		return ERROR;
	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write)
	 * EEPROM memory was set up as fixed address beginning with 0xA0
	 * 11 bit address for memory location divides 3bits and 8 to avoid sending 2bytes for memory addressing
	 * (Fixed 1010+ 3 MSB bits + R/W-)(the 8 LSB bits in memory address)
	 */

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=1 (Read) */

	TWI_writeByte(0xA0 | 0x01);
	if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
		return ERROR;

	/* Read Byte from Memory without send ACK */
	*u8data = TWI_readByteWithNACK();
	if (TWI_getStatus() != TWI_MR_DATA_NACK)
		return ERROR;

	/* Send the Stop Bit */
	TWI_stop();
	/*return success*/
	return SUCCESS;
}
