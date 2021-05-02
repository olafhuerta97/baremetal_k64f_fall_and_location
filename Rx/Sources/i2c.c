/*
 * i2c.c
 *
 *  Created on: Sep 29, 2019
 *      Author: joseolafhuerta
 */
#include "i2c.h"
#include "derivative.h" 
#include "nrf24.h"
#include "lcd.h"

void I2C_WriteRegister(unsigned char u8SlaveAddress, unsigned short u16Address, unsigned char u8Data)
{
	unsigned char u8AddressMSB=0;
	unsigned char u8AddressLSB=0;
	u8AddressMSB=(unsigned char)((u16Address &(0xFF00))>>8);
	u8AddressLSB=(unsigned char)(u16Address &(0x00FF));
	I2C_Start();      // start bit 
	I2C0_D = u8SlaveAddress << 1;				/* Send I2C device address with W/R bit = 0 */
	I2C_Wait();   // espera

	I2C0_D = u8AddressMSB;					/* Send register address */
	I2C_Wait();   // espera
	
	I2C0_D = u8AddressLSB;					/* Send register address */
	I2C_Wait();   // espera
	
	I2C0_D = u8Data ;   // manda datos a escribir
	I2C_Wait();   //espera
	
	I2C_Stop();  //stop bit
	Pause(4000);  // pausa	
}

unsigned char I2C_ReadRegister(unsigned char u8SlaveAddress, unsigned short u16Address)
{
	unsigned char u8AddressMSB=0;
	unsigned char u8AddressLSB=0;
	u8AddressMSB=(unsigned char)((u16Address &(0xFF00))>>8);
	u8AddressLSB=(unsigned char)(u16Address &(0x00FF));
	unsigned char result;

	I2C_Start();   // inicia star bit
	I2C0_D = u8SlaveAddress << 1;									/* Send I2C device address with W/R bit = 0 */
	I2C_Wait();    // espera

	I2C0_D = u8AddressMSB;					/* Send register address */
	I2C_Wait();   // espera
	
	I2C0_D = u8AddressLSB;					/* Send register address */
	I2C_Wait();   // espera

	I2C_RepeatedStart();   //repite start

	I2C0_D = (u8SlaveAddress << 1) + 1;			/* Send I2C device address this time with W/R bit = 1 */
	I2C_Wait();//espera
	
	I2C_EnterRxMode();   //entra en modo rx
	result = I2C0_D;   // resultado 
	I2C_DisableAck();  // no ack

	I2C_Wait();   // espera
	I2C_Stop();   // stop bit
	result = I2C0_D; // resultado
	Pause(400);  //pausa
	return result;
}

void I2C_init(void)
{
	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;		// Turn on clock to I2C0 module
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK ;
	PORTE_PCR24 |= PORT_PCR_MUX(0x5);		// PTE24 pin is I2C0 SCL line
	PORTE_PCR25 |= PORT_PCR_MUX(0x5);		// PTE25 pin is I2C0 SDA line
	I2C0_F  |= I2C_F_ICR(0x14); 			// SDA hold time = 2.125us, SCL start hold time = 4.25us, SCL stop hold time = 5.125us
	I2C0_C1 |= I2C_C1_IICEN_MASK;           // Enable I2C0 module
}

void writeinMemoryCounter(unsigned short u16Address)
	{
	unsigned char u8AddressMSB=0;
	unsigned char u8AddressLSB=0;
	u8AddressMSB=(unsigned char)((u16Address &(0xFF00))>>8);
	u8AddressLSB=(unsigned char)(u16Address &(0x00FF));
	I2C_WriteRegister(Memory_I2C_Adress, 0x00,u8AddressMSB );
	I2C_WriteRegister(Memory_I2C_Adress, 0x01,u8AddressLSB);
	}
uint16 ReadfromMemoryCounter(void)
	{
	uint16 u16Address;
	uint8 u8AddressMSB=0;
	uint8 u8AddressLSB=0;
	u8AddressMSB= I2C_ReadRegister(Memory_I2C_Adress,0x00);
	u8AddressLSB= I2C_ReadRegister(Memory_I2C_Adress,0x01);
	u16Address=(uint16)((u8AddressMSB<<8)|(u8AddressLSB));
	return u16Address;
	}

uint16 Save_event_data(uint32 date, uint16 hour, uint8 event_type)
{
	uint8 i=0;
	uint32 Save_date_hour;
	uint8 Saveinmemory[5]={0};
	uint16 Address=0;
	Address=ReadfromMemoryCounter();
	Save_date_hour=(date<<12)|(hour);
	Saveinmemory[0] = event_type;
	Saveinmemory[1] = (uint8)(Save_date_hour);
	Saveinmemory[2] = (uint8)(Save_date_hour >> 8);
	Saveinmemory[3] = (uint8)(Save_date_hour >> 16);
	Saveinmemory[4] = (uint8)(Save_date_hour >> 24);
	for(i=0; i<5; i++)
	{
	I2C_WriteRegister(Memory_I2C_Adress ,(Address+i), Saveinmemory[i]);
	}
	writeinMemoryCounter((Address+5));
	return (Address+5);
}
void Get_event_data(uint16 Address, uint8 *eventid, uint32 *date, uint16 *hour)
{
	uint8 i=0;
	uint8 Savefrommemory[5]={0};
	for(i=0; i<5; i++)
	{
		Savefrommemory[i] =I2C_ReadRegister(Memory_I2C_Adress ,(Address+i));
	}
	*eventid=Savefrommemory[0];
	*hour=((((0x0F)&(Savefrommemory[2]))<<8)|Savefrommemory[1]);
	*date=(( Savefrommemory[4]<<12)|(Savefrommemory[3]<<4)|(((0xF0)&(Savefrommemory[2]))>>4));
}

