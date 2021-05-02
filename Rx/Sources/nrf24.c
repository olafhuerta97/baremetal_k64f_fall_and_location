/*
 * nrf24.c
 *
 *  Created on: Oct 16, 2019
 *      Author: joseolafhuerta
 */
#include "nrf24.h"
#include "derivative.h" 


static uint8 CONFIG_INIT=        	(EN_CRC | CRC0 | PWR_UP|  PRIM_RX  ); //!< enabled RX_DR, TX_DS and MAX_RT interrupts; enabled CRC (2 bytes); device powered up and in RX mode
static uint8 CONFIG_INITtx=        	(EN_CRC | CRC0 | PWR_UP );
static uint8 EN_AA_INIT=        	(ENAA_P5 | ENAA_P4 | ENAA_P3 | ENAA_P2 | ENAA_P1 | ENAA_P0); //!< enabled Auto ACK on all pipes
static uint8 EN_RXADDR_INIT=		(ERX_P5 | ERX_P4 | ERX_P3 | ERX_P2 | ERX_P1 | ERX_P0); //!< enabled all data pipes    
static uint8 SETUP_RETR_INIT=		(ARD(2) | ARC(15)); //!< Automatic Retransmission Delay - 750us; Automatic Retransmission Count - up to 15 times (maximum available value)
static uint8 RF_CH_INIT=        	RF_CH_MASK(0); //!< RF Channel - 0 (2.4GHz)
static uint8 RF_SETUP_INIT=    		(RF_DR_LOW | RF_PWR(3)); //!< RF settings: data rate - 250Kbps, output power - 0dBm
static uint8 STATUS_INIT=        	(RX_DR | TX_DS | MAX_RT); //!< cleared interrupt flags
static uint8 RX_PW_P0_INIT=    		RX_PW(5); //!< default payload for data pipe 0 - 1 byte
static uint8 _TX_ADDR =0xD5; //!< TX Address (up to 5 bytes). The address bytes' order is from LSByte (_TX_ADDR[0]) to MSByte (_TX_ADDR[4]).                                                                                                	/*!< \note In order to receive ACK packet, set the same address for data pipe 0 before start of the transmission. */
static uint8 _RX_ADDR_P0=0xD5; //!< RX Address for data pipe 0 (up to 5 bytes). The address bytes' order is from LSByte (_RX_ADDR_P0[0]) to MSByte (_RX_ADDR_P0[4]).


void SPI1_Init(void)
{
	//configure ports
	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK; // Enable PORT clock gating ctrl    
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	SIM_SCGC6 |= SIM_SCGC6_SPI1_MASK;
    
	//PORT D
	PORTD_PCR4 = PORT_PCR_MUX(7)|PORT_PCR_DSE_MASK|PORT_PCR_PS_MASK|PORT_PCR_SRE_MASK ; //PCS0
	PORTD_PCR5 = PORT_PCR_MUX(7)|PORT_PCR_DSE_MASK|PORT_PCR_PS_MASK|PORT_PCR_SRE_MASK; //SCK
	PORTD_PCR6 = PORT_PCR_MUX(7)|PORT_PCR_PS_MASK; //SOUT /MOSI
	PORTD_PCR7 = PORT_PCR_MUX(7)|PORT_PCR_DSE_MASK|PORT_PCR_PS_MASK|PORT_PCR_SRE_MASK; //SIN/MISO
	//PORTBC
	PORTB_PCR20=PORT_PCR_MUX(1); //GPIO   CE
	PORTC_PCR18=PORT_PCR_MUX(1)|PORT_PCR_IRQC(10)  ; //GPIO   IRQ
	NVICISER1|=(1<<(61%32));
	GPIOB_PDDR|= (1<<20);
	pin_CELOW();
	// Clear all registers
	SPI1_SR = (SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK | SPI_SR_TFFF_MASK | SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK); //clear the status bits (write-1-to-clear)
	SPI1_TCR = 0;
	SPI1_RSER = 0;
	SPI1_PUSHR = 0; //Clear out PUSHR register. Since DSPI is halted, nothing should be transmitted
	SPI1_CTAR0 = 0;
    
	// configure registers
	SPI1_MCR |= SPI_MCR_MSTR_MASK | SPI_MCR_PCSIS_MASK |  SPI_MCR_CLR_RXF_MASK | SPI_MCR_CLR_TXF_MASK| SPI_MCR_ROOE_MASK ;
	SPI1_MCR &=  ~SPI_MCR_MDIS_MASK & ~ SPI_MCR_HALT_MASK; //enable SPI and start transfer
	SPI1_CTAR0 |=  SPI_CTAR_FMSZ_8BIT  | SPI_CTAR_BR(6) |SPI_CTAR_CSSCK(8); // 8 bits, 100khz
	SPI1_CTAR0 |= SPI_CTAR_ASC(8)  | SPI_CTAR_PBR(3)  |SPI_CTAR_PDT(7);
}

void RF_INITtx(void)
{
	Pause(200000);  // 80ms
	pin_CELOW();
	RF24_WriteRegister(RF_SETUP,RF_SETUP_INIT);
	RF24_WriteRegister(RX_PW_P0,RX_PW_P0_INIT);
	RF24_WriteRegister(RF_CH,RF_CH_INIT);
	Write5RegRf(RX_ADDR_P0,_RX_ADDR_P0,_RX_ADDR_P0,_RX_ADDR_P0,_RX_ADDR_P0,_RX_ADDR_P0);
	Write5RegRf(TX_ADDR,_TX_ADDR,_TX_ADDR,_TX_ADDR,_TX_ADDR,_TX_ADDR);
	RF24_WriteRegister(EN_RXADDR,EN_RXADDR_INIT);
	RF24_WriteRegister(STATUS1,STATUS_INIT);
	RF24_WriteRegister(EN_AA,EN_AA_INIT);  //sender
	RF24_WriteRegister(SETUP_RETR,SETUP_RETR_INIT); //sender
	RF24_WriteRegister(CONFIG,CONFIG_INITtx);  // TX
	Pause(125000);// 50ms
	pin_CEHIGH();
	SPI_SendByte(FLUSH_RX);
	SPI_SendByte(FLUSH_TX);
	pin_CELOW();
	Pause(25000);  // 10ms
}
uint8 RF24_ReadRegister(uint8 reg)
{
	uint8 read;
	read= SPI_ReadWriteByte(reg,NOP);
	return read;
}

void RF24_WriteRegister(uint8 u8Adress, uint8 u8Data)
{
	SPI_ReadWriteByte(W_REGISTER|u8Adress,u8Data);
}

uint8 SPI_SendByte(uint8 Data)
{
	SPI1_MCR |=  SPI_MCR_HALT_MASK;
	SPI1_MCR |= (SPI_MCR_CLR_RXF_MASK | SPI_MCR_CLR_TXF_MASK); //flush the fifos
	SPI1_SR |= (SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK | SPI_SR_TFFF_MASK | SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK); //clear the status bits (write-1-to-clear)
	SPI1_TCR |= SPI_TCR_SPI_TCNT_MASK;
	SPI1_MCR &=  ~SPI_MCR_HALT_MASK;
	SPI1_PUSHR = (SPI_PUSHR_EOQ_MASK | SPI_PUSHR_PCS0_ON | Data); //SPI_PUSHR_CONT_MASK  SPI_PUSHR_EOQ_MASK
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK; //clear the status bits (write-1-to-clear)
	Pause(250);  //100us
	return SPI1_POPR;
}


uint8 RF24_Status()
{
uint8 status;
status= SPI_SendByte(NOP);
	return status;
}

uint8 SPI_ReadWriteByte(uint8 Data, uint8 Data1){
	SPI1_MCR |=  SPI_MCR_HALT_MASK;
	SPI1_MCR |= (SPI_MCR_CLR_RXF_MASK | SPI_MCR_CLR_TXF_MASK); //flush the fifos
	SPI1_SR |= (SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK | SPI_SR_TFFF_MASK | SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK); //clear the status bits (write-1-to-clear)
	SPI1_TCR |= SPI_TCR_SPI_TCNT_MASK;
	SPI1_MCR &=  ~SPI_MCR_HALT_MASK;
	SPI1_PUSHR = (SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS0_ON | Data); //SPI_PUSHR_CONT_MASK  SPI_PUSHR_EOQ_MASK  
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK ;
	SPI1_PUSHR = (SPI_PUSHR_EOQ_MASK | SPI_PUSHR_PCS0_ON | Data1); //SPI_PUSHR_CONT_MASK  SPI_PUSHR_EOQ_MASK
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK ;
	Pause(250);  //100us
	(void)SPI1_POPR;
	return SPI1_POPR;
}


uint8 Write5RegRf(uint8 Data, uint8 Data1, uint8 Data2, uint8 Data3, uint8 Data4, uint8 Data5 )
{
	uint8 reg = Data|W_REGISTER;
	SPI1_MCR |=  SPI_MCR_HALT_MASK;
	SPI1_MCR |= (SPI_MCR_CLR_RXF_MASK | SPI_MCR_CLR_TXF_MASK); //flush the fifos
	SPI1_SR |= (SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK | SPI_SR_TFFF_MASK | SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK); //clear the status bits (write-1-to-clear)
	SPI1_TCR |= SPI_TCR_SPI_TCNT_MASK;
	SPI1_MCR &=  ~SPI_MCR_HALT_MASK;
    
	SPI1_PUSHR = (SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS0_ON | reg); //SPI_PUSHR_CONT_MASK  SPI_PUSHR_EOQ_MASK  
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK; //clear the status bits (write-1-to-clear)
	SPI1_PUSHR = (SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS0_ON | Data1); //  
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK; //clear the status bits (write-1-to-clear)
	SPI1_PUSHR = (SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS0_ON | Data2); //  
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK; //clear the status bits (write-1-to-clear)
	SPI1_PUSHR = (SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS0_ON | Data3); //  
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK; //clear the status bits (write-1-to-clear)
	SPI1_PUSHR = (SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS0_ON | Data4); //  
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK; //clear the status bits (write-1-to-clear)
	SPI1_PUSHR = (SPI_PUSHR_EOQ_MASK | SPI_PUSHR_PCS0_ON | Data5); //last byte
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK; //clear the status bits (write-1-to-clear)
	(void)SPI1_POPR;
	Pause(800);  //200us
	return SPI1_POPR;
}

void Pause(uint32 MaxValue)
{
	//pausa comun con un for
	uint32 Counter;
	for(Counter = 0; Counter < MaxValue; Counter++)
    	{
        	asm("nop");
    	}
}
void RF24_Initrx(void)
{
	Pause(200000);  // 80ms
	pin_CELOW();
	RF24_WriteRegister(RF_SETUP,RF_SETUP_INIT);
	RF24_WriteRegister(RX_PW_P0,RX_PW_P0_INIT);
	RF24_WriteRegister(RF_CH,RF_CH_INIT);
	Write5RegRf(RX_ADDR_P0,_RX_ADDR_P0,_RX_ADDR_P0,_RX_ADDR_P0,_RX_ADDR_P0,_RX_ADDR_P0);
	Write5RegRf(TX_ADDR,_TX_ADDR,_TX_ADDR,_TX_ADDR,_TX_ADDR,_TX_ADDR);
	RF24_WriteRegister(EN_RXADDR,EN_RXADDR_INIT);
	RF24_WriteRegister(STATUS1,STATUS_INIT);
	RF24_WriteRegister(CONFIG,CONFIG_INIT);  // rx
	Pause(125000);// 50ms
	pin_CEHIGH();
	SPI_SendByte(FLUSH_RX);
	SPI_SendByte(FLUSH_TX);
	pin_CELOW();
	Pause(25000);  // 10ms
	pin_CEHIGH();
}

void Read5RegRf (uint8 Data, uint8 *buf){
	uint8 reg = Data;
	SPI1_MCR |=  SPI_MCR_HALT_MASK;
	SPI1_MCR |= (SPI_MCR_CLR_RXF_MASK | SPI_MCR_CLR_TXF_MASK); //flush the fifos
	SPI1_SR |= (SPI_SR_TCF_MASK | SPI_SR_EOQF_MASK | SPI_SR_TFUF_MASK | SPI_SR_TFFF_MASK | SPI_SR_RFOF_MASK | SPI_SR_RFDF_MASK); //clear the status bits (write-1-to-clear)
	SPI1_TCR |= SPI_TCR_SPI_TCNT_MASK;
	SPI1_MCR &=  ~SPI_MCR_HALT_MASK;	
	SPI1_PUSHR = (SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS0_ON | reg); //SPI_PUSHR_CONT_MASK  SPI_PUSHR_EOQ_MASK  
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK ;
	*buf=SPI1_POPR; 
	SPI1_PUSHR = (SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS0_ON | NOP); //  
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK ;
	*buf=SPI1_POPR; 
	buf++;
	SPI1_PUSHR = (SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS0_ON | NOP); //  
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK ;
	*buf=SPI1_POPR; 
	buf++;
	SPI1_PUSHR = (SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS0_ON |NOP); //  
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK ;
	*buf=SPI1_POPR; 
	buf++;
	SPI1_PUSHR = (SPI_PUSHR_CONT_MASK | SPI_PUSHR_PCS0_ON |NOP); //  
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK ;
	*buf=SPI1_POPR; 
	buf++;
	SPI1_PUSHR = (SPI_PUSHR_EOQ_MASK | SPI_PUSHR_PCS0_ON | NOP); //last byte
	while(!(SPI1_SR & SPI_SR_TCF_MASK));
	SPI1_SR |= SPI_SR_TFFF_MASK | SPI_SR_TCF_MASK ;
	*buf=SPI1_POPR; 
	(void)SPI1_POPR;
	Pause(500);  //200us
}


