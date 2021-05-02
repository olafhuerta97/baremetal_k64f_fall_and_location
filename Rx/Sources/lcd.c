/*
 * lcd.c
 *
 *  Created on: Oct 25, 2019
 *      Author: joseolafhuerta
 */
#include "derivative.h" /* include peripheral declarations */
#include "lcd.h" /* include peripheral declarations */
#include "i2c.h"


static uint8 Line1[16] =  {"      Paciente  "};
static uint8 Line2[16] =  {"  /  /     :    "};

uint32 outputPorts(uint32 dato)
{
	uint32 exit=0;
	if(dato&(0x8)) exit+=D7;
	if(dato&(0x4)) exit+=D6;
	if(dato&(0x2)) exit+=D5;				
	if(dato&(0x1)) exit+=D4;
	return exit;
}

void rsIntruction(void)
{
	GPIOC_PCOR=RS;
	enable();
}

void data()
{
	GPIOC_PSOR=RS;
	enable();
	GPIOC_PCOR=RS;
}

void enable(void)
{
	GPIOC_PSOR=E;
	delay(20);
	GPIOC_PCOR=E;
}


void rst(void)
{
	GPIOD_PDOR=outputPorts(0x0);
	rsIntruction();
	GPIOD_PDOR=outputPorts(0x1);
	rsIntruction();
}


void pass(void)
{
	GPIOD_PDOR=outputPorts(0xC);
	rsIntruction();
	GPIOD_PDOR=outputPorts(0x0);
	rsIntruction();
}

void message(uint8 *cadena)
{
	uint8 i;
	uint8 z;
	GPIOC_PSOR=RS;
	for(i=0;i<16;i++){
		z=((*(cadena+i))&0xF0)>>4;
		GPIOD_PDOR=outputPorts(z);
		enable();
		z=((*(cadena+i))&0x0F);
		GPIOD_PDOR=outputPorts(z);
		enable();
	}
	GPIOC_PCOR=RS;
}

void LCD_init(void){
	delay(100);
	uint8 cadena[17]={0x3,0x3,0x3,0x2,0x2,0x8,0x0,0x8,0x0,0x1,0x0,0x4,0x0,0xF,0x0,0x1,0xFF};
	uint8 *i;
	i=&cadena[0];
	GPIOC_PCOR =E;
	GPIOC_PCOR =RS;

	while(*i!=0xFF){
		GPIOD_PDOR=outputPorts(*i);
		rsIntruction();
		i++;
	}
	LPTMR0_CSR |=  LPTMR_CSR_TIE_MASK ;
}

void delay(uint16 tiempo_ms)
{
	LPTMR0_PSR= LPTMR_PSR_PBYP_MASK |LPTMR_PSR_PCS(1) ;	//Prescaler bypass y LPO de CLK
	LPTMR0_CMR=tiempo_ms;	//Asignar tiempo_ms a registro con valor a comparar
	LPTMR0_CSR= LPTMR_CSR_TEN_MASK;	//Timer Enable 
	do{}
	while(!(LPTMR0_CSR & LPTMR_CSR_TCF_MASK));				//Esperar la bandera TCF
	LPTMR0_CSR |= LPTMR_CSR_TCF_MASK;
}

void LPTMR_init(void)
{
	SIM_SCGC5|= SIM_SCGC5_LPTMR_MASK ;
	LPTMR0_PSR= LPTMR_PSR_PBYP_MASK |LPTMR_PSR_PCS(1) ;	//Prescaler bypass y LPO de CLK
	LPTMR0_CSR= LPTMR_CSR_TEN_MASK;	//Timer Enable 
	LPTMR0_CSR |=  LPTMR_CSR_TIE_MASK ;
}

void PORT_init(void)
{
	//ENABLE PORT C AND B
	SIM_SCGC5|=  SIM_SCGC5_PORTD_MASK |SIM_SCGC5_PORTC_MASK ;
	
	//ENABLE PORTS AS GPIO
	PORTD_PCR0 = PORT_PCR_MUX(1);  
	PORTD_PCR1 = PORT_PCR_MUX(1);
	PORTD_PCR2 = PORT_PCR_MUX(1);
	PORTD_PCR3 = PORT_PCR_MUX(1);
	PORTC_PCR3 = PORT_PCR_MUX(1);
	PORTC_PCR2 = PORT_PCR_MUX(1);
	
	//ENABLE PORTS AS OUTPUTS
	GPIOD_PDDR |= 0xF;
	GPIOC_PDDR |= (1<<3)|(1<<2);
}

void detect_event_type (uint8 event_type, uint8 *linetochange)
{
	uint8 number;
	if((event_type&0xF0)==0xC0)
	{
		*linetochange='C';
		*(linetochange+1)='a';
		*(linetochange+2)='i';
		*(linetochange+3)='d';
		*(linetochange+4)='a';
	}
	else if((event_type&0xF0)==0xF0)
	{
		*linetochange='F';
		*(linetochange+1)='u';
		*(linetochange+2)='g';
		*(linetochange+3)='a';
		*(linetochange+4)=' ';	
	}
	number=(event_type&0x0F)+'0';
	*(linetochange+15)=number;	
}
void detect_datenhour(uint32 fecha,uint16 hora,uint8 *linetochange){
	uint8 u8day;
	uint8 u8month;
	uint8 u8year;
	uint8 i=0;
	if (fecha == 0 && hora == 0)
	{
		for(i=0; i<16; i++)
		{
			*(linetochange+i)=' ';
		}
	}
	else
	{
	u8year=fecha%100;
	u8month=(fecha/100)%100;
	u8day=(uint8)(fecha/10000);	
	*(linetochange+1)=(u8day%10)+'0';
	u8day/=10;
	*(linetochange)=u8day+'0';
	*(linetochange+4)=(u8month%10)+'0';
	u8month/=10;
	*(linetochange+3)=u8month+'0';
	*(linetochange+7)=(u8year%10)+'0';
	u8year/=10;
	*(linetochange+6)=u8year+'0';
	*(linetochange+13)=(hora%10)+'0';
	hora/=10;
	*(linetochange+12)=(hora%10)+'0';
	hora/=10;
	*(linetochange+10)=(hora%10)+'0';
	hora/=10;
	*(linetochange+9)=(hora%10)+'0';
	hora/=10;
	*(linetochange+11)=':';
	*(linetochange+2)='/';
	*(linetochange+5)='/';
	}
}

void vEvent_init(void)
{
	uint16 counter=0;
	uint8 eventid=0;
	uint32 fecha=0;
	uint16 hora=0;
	counter= ReadfromMemoryCounter();
	Get_event_data((counter-5), &eventid, &fecha, &hora);
	detect_event_type (eventid, &Line1[0]);
	detect_datenhour (fecha, hora, &Line2[0]);
	rst();
	message(&Line1[0]);
	pass();
	message(&Line2[0]);
}

void vnew_Event(uint16 counter)
{
	uint8 eventid=0;
	uint32 fecha=0;
	uint16 hora=0;
	Get_event_data((counter-5), &eventid, &fecha, &hora);
	detect_event_type (eventid, &Line1[0]);
	detect_datenhour (fecha, hora, &Line2[0]);
	rst();
	message(&Line1[0]);
	pass();
	message(&Line2[0]);
}

void vSearch_Event(uint16 counter)
{
	uint8 eventid=0;
	uint32 fecha=0;
	uint16 hora=0;
	Get_event_data(counter, &eventid, &fecha, &hora);
	detect_event_type (eventid, &Line1[0]);
	detect_datenhour (fecha, hora, &Line2[0]);
	rst();
	message(&Line1[0]);
	pass();
	message(&Line2[0]);
}

