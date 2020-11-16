/*
 * adc.c
 *
 *  Created on: Oct 31, 2019
 *      Author: joseolafhuerta
 */

#include "derivative.h" /* include peripheral declarations */
#include "adc.h" 


static unsigned short u16mov_mean_ADC[50]={0};
static unsigned char i=0;
static unsigned char firstround=0;

void ADC0_SE18_init(void)
{
	SIM_SCGC5|=SIM_SCGC5_PORTE_MASK ;//ENABLE PTE
	SIM_SCGC3|=SIM_SCGC3_ADC1_MASK ;// ENABLE ADC 1 
	ADC1_CFG1|= ADC_CFG1_MODE(3);
	ADC1_SC1A|= (ADC_SC1_ADCH(18)|ADC_SC1_AIEN_MASK);//Canal y enable adc
	NVICISER2|=(1<<(73%32)); //encender interrupcion ADC
}

void movmean50(unsigned short datain,unsigned short *result)
{
	unsigned long u32Average_ADC=0;
	unsigned char j=0;
	u16mov_mean_ADC[(i++%50)]=datain;
	if (i>=50)
	{
		firstround=1;	
	}
	if(firstround)
	{
		for(j=0;j<50;j++){
			u32Average_ADC=u32Average_ADC+u16mov_mean_ADC[j];
		}
		*result=u32Average_ADC/50;
	}
}

void leds_init(void)
{
	SIM_SCGC5|=SIM_SCGC5_PORTB_MASK ;//ENABLE PTE
	SIM_SCGC5|=SIM_SCGC5_PORTC_MASK ;//ENABLE PTE
	SIM_SCGC5|=SIM_SCGC5_PORTA_MASK ;//ENABLE PTE
	PORTC_PCR2 = PORT_PCR_MUX(1); //Green
	PORTA_PCR2 = PORT_PCR_MUX(1); //Green
	PORTB_PCR23 = PORT_PCR_MUX(1); //Yellow
	PORTA_PCR1 = PORT_PCR_MUX(1); //Red
	GPIOA_PDDR |= (1<<2)|(1<<1); 
	GPIOB_PDDR |= (1<<23); 
	GPIOC_PDDR |= (1<<2); 
	GreenLOW();
	YellowLOW(); 
	RedLOW();
}
