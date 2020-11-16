/*
 * adc.h
 *
 *  Created on: Oct 31, 2019
 *      Author: joseolafhuerta
 */

#ifndef ADC_H_
#define ADC_H_

#define  GreenLOW()   GPIOC_PSOR|=(1<<2);GPIOA_PSOR|=(1<<2)
#define  GreenHIGH()  GPIOC_PCOR|=(1<<2);GPIOA_PCOR|=(1<<2)
#define  YellowLOW()   GPIOB_PSOR|=(1<<23)
#define  YellowHIGH()  GPIOB_PCOR|=(1<<23)
#define  RedLOW()      GPIOA_PSOR|=(1<<1)
#define  RedHIGH()     GPIOA_PCOR|=(1<<1)

void ADC0_SE18_init(void);
void movmean50(unsigned short datain,unsigned short *result);
void leds_init(void);

#endif /* ADC_H_ */
