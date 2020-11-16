/*
 * lptmr.c

 *
 *  Created on: Sep 29, 2019
 *      Author: joseolafhuerta
 */
#include "lptmr.h"
#include "derivative.h" 

void LPTMR_init(uint16 tiempo)
{
	SIM_SCGC5|=SIM_SCGC5_LPTMR_MASK ;
	LPTMR0_CMR=tiempo-1;         //Tiempo de referencia 
	LPTMR0_PSR=LPTMR_PSR_PBYP_MASK |LPTMR_PSR_PCS(1)   ;        //Se configura el prescaler en bypass (bit 2), y el bit 0 para seleccionar LPO (1 kHz)
	LPTMR0_CSR= LPTMR_CSR_TIE_MASK|LPTMR_CSR_TEN_MASK ;        //Se prende el bit 6 (TIE) y el bit 0 (TEN)
	NVICISER1|=(1<<(58%32));     //Rutina de interrupción para LPTMR
}
