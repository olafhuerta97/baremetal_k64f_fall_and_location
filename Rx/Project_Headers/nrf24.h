/*
 * nrf24.h
 *
 *  Created on: Oct 16, 2019
 *      Author: joseolafhuerta
 */

#ifndef NRF24_H_
#define NRF24_H_

//declaracion var
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32; 

#define pin_CEHIGH()   	Pause(300);\
                       	GPIOB_PSOR = 1<<20

#define pin_CELOW()    	Pause(300);\
                       	GPIOB_PCOR = 1<<20
     
//registros del modulo de rf
#define CONFIG         	0x00 //!<Configuration Register
#define EN_AA         	0x01 //!<Enable 'Auto Acknowledgement' function; disable this functionality to be compatible with nRF24L01 (see page 72 of reference manual)
#define EN_RXADDR     	0x02 //!<Enabled RX Adresses
#define SETUP_AW     	0x03 //!<Setup of Adress Widths (common for all data pipes)
#define SETUP_RETR    	0x04 //!<Setup of Automatic Retransmission
#define RF_CH         	0x05 //!<RF Channel
#define RF_SETUP     	0x06 //!<RF Setup Register
#define STATUS1         	0x07 //!<Status Register (in parallel to the SPI command word applied on the MOSI pin, the STATUS register is shifted serially out on the MISO pin)
#define OBSERVE_TX     	0x08 //!<Transmit observe register
#define RPD         	0x09 //!<Received Power Detector
#define RX_ADDR_P0    	0x0A //!<Receive address data pipe 0; 5 bytes maximum length (LSByte is written first. Write the number of bytes defined by SETUP_AW)
#define RX_ADDR_P1    	0x0B //!<Receive address data pipe 1; 5 bytes maximum length (LSByte is written first. Write the number of bytes defined by SETUP_AW)
#define RX_ADDR_P2    	0x0C //!<Receive address data pipe 2; only LSByte. MSBytes are equal to RX_ADDR_P1[39:8]
#define RX_ADDR_P3    	0x0D //!<Receive address data pipe 3; only LSByte. MSBytes are equal to RX_ADDR_P1[39:8]
#define RX_ADDR_P4     	0x0E //!<Receive address data pipe 4; only LSByte. MSBytes are equal to RX_ADDR_P1[39:8]
#define RX_ADDR_P5     	0x0F //!<Receive address data pipe 5; only LSByte. MSBytes are equal to RX_ADDR_P1[39:8]
#define TX_ADDR     	0x10 //!<Transmit address; used for PTX device only (LSByte is written first); set RX_ADDR_P0 equal to this address to handle automatic acknowledge if this is a PTX device with Enhanced Shockburst enabled (see page 72 of reference manual)
#define RX_PW_P0     	0x11 //!<Number of bytes in RX Payload in data pipe 0 (1-32)
#define RX_PW_P1     	0x12 //!<Number of bytes in RX Payload in data pipe 1 (1-32)
#define RX_PW_P2     	0x13 //!<Number of bytes in RX Payload in data pipe 2 (1-32)
#define RX_PW_P3     	0x14 //!<Number of bytes in RX Payload in data pipe 3 (1-32)
#define RX_PW_P4     	0x15 //!<Number of bytes in RX Payload in data pipe 4 (1-32)
#define RX_PW_P5     	0x16 //!<Number of bytes in RX Payload in data pipe 5 (1-32)
#define FIFO_STATUS    	0x17 //!<FIFO Status Register
#define DYN_PD         	0x1C //!<Enable Dynamic Payload Length
#define FEATURE     	0x1D //!<Feature Register
//comandos para el rf
#define R_REGISTER        	0x00 //!<Read from register (1-5 bytes (LSByte first, from MSB to LSB)); use (R_REGISTER | register_address) to read data from the specific register;
#define W_REGISTER        	0x20 //!<Write to register (1-5 bytes (LSByte first, from MSB to LSB)); use (W_REGISTER | register_address) to write data to the specific register; executable in power down or standby modes only
#define R_RX_PAYLOAD    	0x61 //!<Read RX Payload (1-32 bytes); a read operation always starts at byte 0. Payload is deleted from the RX FIFO after it is read; used in RX mode
#define W_TX_PAYLOAD    	0xA0 //!<Write TX Payload (1-32 bytes); a write operation always starts at byte 0 used in TX Payload
#define FLUSH_TX        	0xE1 //!<Flush TX FIFO (0 bytes); used in TX mode
#define FLUSH_RX        	0xE2 //!<Flush RX FIFO (0 bytes); used in RX mode; should not be executed during transmission of acknowledge (ACK packet will not be completed)
#define REUSE_TX_PL        	0xE3 //!<Reuse last transmitted payload (0 bytes); TX Payload Reuse is active until W_TX_PAYLOAD or FLUSH_TX is executed; TX Payload Reuse must not be activated or deactivated during package transmission; used for a PTX device
#define R_RX_PL_WID        	0x60 //!<Read RX Payload width for the top R_RX_PAYLOAD in the RX FIFO (1 byte)
#define W_ACK_PAYLOAD    	0xA8 //!<Write payload to be transmitted together with ACK packet on the specific data pipe (1-32 bytes (LSByte first)); use (W_ACK_PAYLOAD | pipe_number) to store ACK payload for the specific data pipe; maximum 3 ACK payloads can be pending. Payloads with the same pipe_number are handled using FIFO principle;
#define W_TX_PAYLOAD_NOACK	0xB0 //!<Disable AUTO_ACK on this specific packet (1-32 bytes (LSByte first)); used in TX mode
#define NOP                	0xFF //!<No Operation (0 bytes); might be used to read the STATUS register
//definiciones para spi
#define SPI_PUSHR_PCS0_ON 	0x10000
#define SPI_PUSHR_PCS1_ON 	0x20000
#define SPI_PUSHR_PCS2_ON 	0x40000
#define SPI_PUSHR_PCS3_ON 	0x80000
#define SPI_PUSHR_PCS4_ON 	0x100000
#define SPI_PUSHR_PCS5_ON 	0x200000
#define SPI_CTAR_FMSZ_8BIT 	0x38000000

#define SPI_Wait()	while(!(SPI1_SR & SPI_SR_TCF_MASK));SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK //clear the status bits (write-1-to-clear)

	/* config */
#define MASK_RX_DR 	(1<<6) //!<Mask interrupt caused by RX_DR; '1': Interrupt not reflected on the IRQ pin, '0': Reflect RX_DR as active low interrupt on the IRQ pin
#define MASK_TX_DS 	(1<<5) //!<Mask interrupt caused by TX_DS; '1': Interrupt not reflected on the IRQ pin, '0': Reflect TX_DS as active low interrupt on the IRQ pin
#define MASK_MAX_RT	(1<<4) //!<Mask interrupt called by MAX_RT; '1': Interrupt not reflected on the IRQ pin, '0': Reflect MAX_RT as active low interrupt on the IRQ pin
#define EN_CRC         	(1<<3) //!<Enable CRC; forced high if one of the bits in the EN_AA is high
#define CRC0             	(1<<2) //!<CRC encoding scheme; '1': 2 bytes, '0': 1 byte
#define PWR_UP         	(1<<1) //!<'1': power up, '0': power down
#define PRIM_RX     	(1<<0) //!<RX/TX control; '1': PRX, '0': PTX

	/* EN_AA */
#define ENAA_P5     	(1<<5) //!<Enable auto acknowledgement data pipe 5
#define ENAA_P4     	(1<<4) //!<Enable auto acknowledgement data pipe 4
#define ENAA_P3     	(1<<3) //!<Enable auto acknowledgement data pipe 3
#define ENAA_P2     	(1<<2) //!<Enable auto acknowledgement data pipe 2
#define ENAA_P1     	(1<<1) //!<Enable auto acknowledgement data pipe 1
#define ENAA_P0        	(1<<0) //!<Enable auto acknowledgement data pipe 0

	/* EN_RXADDR */
#define ERX_P5         	(1<<5) //!<Enable data pipe 5
#define ERX_P4         	(1<<4) //!<Enable data pipe 4
#define ERX_P3         	(1<<3) //!<Enable data pipe 3
#define ERX_P2         	(1<<2) //!<Enable data pipe 2
#define ERX_P1         	(1<<1) //!<Enable data pipe 1
#define ERX_P0         	(1<<0) //!<Enable data pipe 0

	/* SETUP_AW */
#define AW(x)            	((x)<<0) //!<//bits 1:0// RX/TX Address field width; '11': 5 bytes, '10': 4 bytes, '01': 3 bytes, '00': illegal; LSByte is used if address width is below 5 bytes

	/* SETUP_RETR */
#define ARD(x)        	((x)<<4) //!<//bits 7:4// Automatic Retransmission Delay; '1111': Wait 4000us, '1110': Wait 3750us, ... '0001': Wait 500us, '0000': Wait 250us; delay defined from the end of the transmission to the start of the next transmission
                                            	/*!
                                            	* Please take care when setting this parameter. If the ACK Payload is more than 15 bytes in 2Mbps or 5 bytes in 1Mbps, the ARD must be 500us or more. In 250Kbps (even if there is no ACK Payload) the ARD must be 500us or more
                                            	*/
#define ARC(x)        	((x)<<0) //!<//bits 3:0// Auto Retransmit Count; '1111': Up to 15 retransmits on fail of AA, '1110': Up to 14 retransmits on fail of AA, ... '0001': Up to 1 retransmit on fail of AA, '0000': Retransmits disabled

	/* RF_CH */
#define RF_CH_MASK(x)	((x)<<0) //!<//bits 6:0// Sets the frequency channel nRF24L01+ operates on (2.400GHz - 2.525GHz); default = 2 (2.4GHz (?))

	/* RF_SETUP */
#define CONT_WAVE    	(1<<7) //!<Enables continuous carrier transmit when high
#define RF_DR_LOW    	(1<<5) //!<Set RF Data Rate to 250Kbps (see RF_DR_HIGH for encoding)
#define PLL_LOCK    	(1<<4) //!<Force PLL lock signal; only used in test
#define RF_DR_HIGH	(1<<3) //!<Select between the high speed Data Rates; this bit does not matter if RF_DR_LOW is high; encoding [RF_DR_HIGH:RF_DR_LOW]: '11': Reserved, '10': 2Mbps, '01': 250Kbps, '00': 1Mbps
#define RF_PWR(x)    	((x)<<1) //!<//bits 2:1// Set RF output power in TX mode; '11': 0dBm, '10': -6dBm, '01': -12dBm, '00': -18dBm

	/* STATUS */
#define RX_DR            	(1<<6) //!<Data Ready RX FIFO interrupt; asserted when new data arrives RX FIFO; write '1' to clear bit
#define TX_DS            	(1<<5) //!<Data Sent TX FIFO interrupt; asserted when packet transmitted on TX. If AUTO_ACK is activated, this bit is set high only when ACK is received; write '1' to clear bit
#define MAX_RT        	(1<<4) //!<Maximum number of TX retransmits interrupt; if MAX_RT is asserted, it must be cleared to enable further communication; write '1' to clear bit
#define RX_P_NO(x)	((x)<<1) //!<//bits 3:1// Data pipe number for the payload available for reading from RX_FIFO; '111': RX FIFO Empty, '110': Not used, '101'-'000': Data pipe number
#define TX_FULL        	(1<<0) //!<TX FIFO Full flag; '1': TX FIFO full, '0': Available locations in TX FIFO

  /* RX_PW_P0-5 */
#define RX_PW(x)     	((x)<<0) //!<//bits 5:0// Number of bytes in RX Payload in data pipe 0-5; 0: Pipe not used, 1-32: 1-32 bytes


	/* FEATURE */
#define EN_DPL        	(1<<2) //!<Enable Dynamic Payload Length
#define EN_ACK_PAY    	(1<<1) //!<Enable ACK Payload
                             	/*!<
                              	* If ACK packet payload is activated, ACK packets have dynamic payload length and Dynamic Payload Length feature should be enabled for data pipe 0 on the PTX and PRX devices (EN_DPL,ENAA_P0,DPL_P0). This is to ensure that they receive ACK packets with payloads. Also set the correct ARD value
                              	*/
#define EN_DYN_ACK    	(1<<0) //!<Enable W_TX_PAYLOAD_NOACK command
	//!@}

void RF_INITtx(void);
void SPI1_Init(void);
void Pause(uint32 MaxValue);
uint8 SPI_ReadWriteByte(uint8 Data,uint8 Data1);
uint8 RF24_ReadRegister(uint8 reg);
uint8 RF24_Status();
uint8 SPI_ReadByte();
uint8 SPI_SendByte(uint8 Data);
void RF24_WriteRegister(uint8 u8Adress, uint8 u8Data);
uint8 Write5RegRf(uint8 Data, uint8 Data1, uint8 Data2, uint8 Data3, uint8 Data4, uint8 Data5);
void RF24_Initrx(void);
void Read5RegRf (uint8 Data, uint8 *buf);

#endif /* NRF24_H_ */
