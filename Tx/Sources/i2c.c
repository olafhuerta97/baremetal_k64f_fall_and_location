/*
 * i2c.c
 *
 *  Created on: Sep 29, 2019
 *      Author: joseolafhuerta
 */
#include "i2c.h"
#include "derivative.h" 
#include "FXOS8700CQ.h"
#include "nrf24.h"

void I2C_WriteRegister(unsigned char u8SlaveAddress, unsigned char u8RegisterAddress, unsigned char u8Data)
{
	I2C_Start();      // start bit 
	I2C0_D = u8SlaveAddress << 1;				/* Send I2C device address with W/R bit = 0 */
	I2C_Wait();   // espera

	I2C0_D = u8RegisterAddress;					/* Send register address */
	I2C_Wait();   // espera
	
	I2C0_D = u8Data ;   // manda datos a escribir
	I2C_Wait();   //espera
	
	I2C_Stop();  //stop bit
	Pause(5000);  // pausa	
}

unsigned char I2C_ReadRegister(unsigned char u8SlaveAddress, unsigned char u8RegisterAddress)
{
	uint8 result;

	I2C_Start();   // inicia star bit
	I2C0_D = u8SlaveAddress << 1;									/* Send I2C device address with W/R bit = 0 */
	I2C_Wait();    // espera

	I2C0_D = u8RegisterAddress;										/* Send register address */
	I2C_Wait();   // espera

	I2C_RepeatedStart();   //repite start

	I2C0_D = (u8SlaveAddress << 1) + 1;							/* Send I2C device address this time with W/R bit = 1 */
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
	SIM_SCGC5 |= SIM_SCGC5_PORTE_MASK;		// Turn on clock to Port E module
	PORTE_PCR24 |= PORT_PCR_MUX(0x5);		// PTE24 pin is I2C0 SCL line
	PORTE_PCR25 |= PORT_PCR_MUX(0x5);		// PTE25 pin is I2C0 SDA line
	I2C0_F  |= I2C_F_ICR(0x14); 			// SDA hold time = 2.125us, SCL start hold time = 4.25us, SCL stop hold time = 5.125us
	I2C0_C1 |= I2C_C1_IICEN_MASK;           // Enable I2C0 module
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;     // prender el puerto c para la interrupcion del accelerometro
	PORTC_PCR6 |= (0|PORT_PCR_ISF_MASK|		// Clear the interrupt flag
					PORT_PCR_MUX(0x1)|		// PTC6 is configured as GPIO
					PORT_PCR_IRQC(0xA));	// PTC6 is configured for falling edge interrupts
	NVICISER1|=1<<(61%32);                  //interrupcion gpioC
}
void ACCEL_init(void)
{
	I2C_WriteRegister(FXOS8700CQ_I2C_ADDRESS, CTRL_REG2, 0x40);   // reset por software del accelerometro
	Pause(30000);  
	I2C_WriteRegister(FXOS8700CQ_I2C_ADDRESS, XYZ_DATA_CFG_REG, 0x02); 	 // prender resolution 8G
	I2C_WriteRegister(FXOS8700CQ_I2C_ADDRESS, M_CTRL_REG1, 0x00);     // apagar magnetometro
	I2C_WriteRegister(FXOS8700CQ_I2C_ADDRESS, CTRL_REG2, 0x00);	 		// High Resolution off
	I2C_WriteRegister(FXOS8700CQ_I2C_ADDRESS, CTRL_REG3, 0x00);	       //no interrupciones en bajo consumo
	I2C_WriteRegister(FXOS8700CQ_I2C_ADDRESS, CTRL_REG4, 0x02); //1 for normal  cambiar este bit a unoo para lectura continua interrupcion mode
	I2C_WriteRegister(FXOS8700CQ_I2C_ADDRESS, CTRL_REG5, 0x02);	// 1 for normal cambiar este bit a unoo para lectura continua	DRDY interrupt routed to INT1 - PTC6
	I2C_WriteRegister(FXOS8700CQ_I2C_ADDRESS, A_VECM_CFG_REG , 0x08);  //enable magnitude para lectura de mgnitud
	I2C_WriteRegister(FXOS8700CQ_I2C_ADDRESS, A_VECM_THS_MSB_REG, 0x13); //13  // registro para ingresar magnitud a ser comparada
	I2C_WriteRegister(FXOS8700CQ_I2C_ADDRESS, A_VECM_THS_LSB_REG, 0x88);  //88
	I2C_WriteRegister(FXOS8700CQ_I2C_ADDRESS, A_VECM_CNT_REG,0);   //  80ms  (4/50hz)
	I2C_WriteRegister(FXOS8700CQ_I2C_ADDRESS, CTRL_REG1, 0x11);   //decimal  2D-12.5hz 21-50hz 0x19-100hz  0x11-200hz   31 6hz low noise y enable accel
	
}

unsigned long I2C_MagDataRead(void)
{
	// variables locales
	uint8 lectura[6];
	uint8 control=1;
	static uint32 mag = 0;
	sint32 x=0;
	sint32 y=0;
	sint32 z=0;
	//leer datos de aaccel
	do{	
		lectura[(control-1)] = I2C_ReadRegister(FXOS8700CQ_I2C_ADDRESS,control);
		control++;
	}
	while(control<=6);
	control=1;
	// calcular magnitud nota: combianacion de signos y tamaños
	x = ((((lectura[0] << 8) | lectura[1]))  ); 
	y = ((((lectura[2] << 8) | lectura[3])) );
	z = ((((lectura[4] << 8) | lectura[5])) );	
	z = (z*8000)/32768;
	y = (y*8000)/32768;
	x = (x*8000)/32768;
	mag = x*x+y*y+z*z ; 
	// regresa magnitud^2
return mag;
}
