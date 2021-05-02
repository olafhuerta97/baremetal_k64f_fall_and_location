/*
 * i2c.h
 *
 *  Created on: Sep 29, 2019
 *      Author: joseolafhuerta
 */
#ifndef I2C_H_
#define I2C_H_ 

#define I2C_Start()            I2C0_C1 |= I2C_C1_TX_MASK;\
                               I2C0_C1 |= I2C_C1_MST_MASK
#define I2C_Stop()             I2C0_C1 &= ~I2C_C1_MST_MASK;\
                               I2C0_C1 &= ~I2C_C1_TX_MASK
#define I2C_Wait()             while((I2C0_S & I2C_S_IICIF_MASK)==0) {} \
                               I2C0_S |= I2C_S_IICIF_MASK; //(1<<1)
#define I2C_DisableAck()       I2C0_C1 |= I2C_C1_TXAK_MASK //(1<<3)
#define I2C_EnableAck()        I2C0_C1 &= ~I2C_C1_TXAK_MASK //(1<<3)
#define I2C_RepeatedStart()    I2C0_C1 |= I2C_C1_RSTA_MASK//(1<<2)
#define I2C_EnterRxMode()      I2C0_C1 &= ~I2C_C1_TX_MASK
#define I2C_write_byte(data)   I2C0_D = data

#define FXOS8700CQ_I2C_ADDRESS 	0x1D
#define WHO_AM_I_REG          	0x0D   
#define Memory_I2C_Adress        0x50 

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef signed long sint32;
typedef unsigned long long uint64;

//prototipado
void I2C_init(void);
void I2C_WriteRegister(unsigned char u8SlaveAddress, unsigned short u16Address, unsigned char u8Data);
unsigned char I2C_ReadRegister(unsigned char u8SlaveAddress, unsigned short u16Address);
void writeinMemoryCounter(unsigned short u16Adress);
uint16 ReadfromMemoryCounter(void);
uint16 Save_event_data(uint32 date, uint16 hour, uint8 event_type);
void Get_event_data(uint16 Address, uint8 *eventid, uint32 *date, uint16 *hour);
void vnew_Event(uint16 counter);

#endif /* I2C_H_ */
