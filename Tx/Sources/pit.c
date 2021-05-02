/*
 * pit.c
 *
 *  Created on: Oct 24, 2019
 *      Author: joseolafhuerta
 */

#include "derivative.h" 
#include "pit.h" 


void pit_init(uint32 tiempo)
{
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;//Activar reloj PIT
	PIT_MCR&= ~PIT_MCR_MDIS_MASK;//Habilitar PIT0
	PIT_LDVAL0 = (20971520/1000)*tiempo;//1us*tiempo
	PIT_TCTRL0 = PIT_TCTRL_TEN_MASK|PIT_TCTRL_TIE_MASK;
	PIT_TFLG0= PIT_TFLG_TIF_MASK;
	NVICISER1|=(1<<(48%32));
}

void pit1_init(uint32 tiempo)
{
	SIM_SCGC6 |= SIM_SCGC6_PIT_MASK;//Activar reloj PIT
	PIT_MCR&= ~PIT_MCR_MDIS_MASK;//Habilitar PIT0
	PIT_LDVAL1 = (20971520/1000)*tiempo;//1us*tiempo
	PIT_TCTRL1 = PIT_TCTRL_TEN_MASK|PIT_TCTRL_TIE_MASK;
		PIT_TFLG1= PIT_TFLG_TIF_MASK;
	NVICISER1|=(1<<(49%32));
}
