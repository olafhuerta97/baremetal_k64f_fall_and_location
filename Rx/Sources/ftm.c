/*
 * ftm.c
 *
 *  Created on: Oct 24, 2019
 *      Author: joseolafhuerta
 */
#include "derivative.h" 
#include "ftm.h"

static	uint16 la=440;
static	uint16 doo=261;
static	uint16 re=293;
static	uint16 mi=329;
static	uint16 fa=349;
static	uint16 sol=391;

 
void FTM3c4_init_outputcompare(void)
{
	SIM_SCGC3|=SIM_SCGC3_FTM3_MASK  ;               //Habilitar CLK para FTM3
	SIM_SCGC5|=SIM_SCGC5_PORTC_MASK;  
	PORTC_PCR8=PORT_PCR_MUX(3)   ; //Configuración como alternativa 3 p
	FTM3_SC=FTM_SC_CLKS(1);                     //Habilitar system clock para el FTM3
	//FTM3_C4SC=FTM_CnSC_ELSA_MASK|FTM_CnSC_MSA_MASK |FTM_CnSC_CHIE_MASK ;     //Habilitar Output Compare toggle (5<<2)
	FTM3_C4V= 20971520/(2*440);                   //Se lee el valor del FTM que equivale a la frecuencia de entrada
	NVICISER2|=(1<<(71%32));     //Rutina de interrupción para el FTM3 en el vector 71
}


uint16 martinillo(uint32 tiempo)
{
	static uint16 tono;
	static uint8 estado;
	PIT_LDVAL0 = (20971520/1000)*(1*300);//tiempo
	if (estado==0){
		estado++;
		tono=doo;
	}
	else if (estado==1) {
		estado++;
		tono=re;
	}
	else if (estado==2) {
		estado++;
		tono=mi;
	}
	else if (estado==3) {
		estado++;
		tono=doo;
	}
	else if (estado==4){
		estado++;
		tono=doo;
	}
	else if (estado==5) {
		estado++;
		tono=re;
	}
	else if (estado==6) {
			estado++;
			tono=mi;
		}
	else if (estado==7) {
			estado++;
			tono=doo;
		}
	else if (estado==8){
		estado++;
		tono=mi;
	}
	else if (estado==9) {
			estado++;
			tono=fa;
		}
	else if (estado==10) {
			estado++;
			tono=sol;
			PIT_LDVAL0 = (20971520/1000)*(2*300);//1us*tiempo
		}
	else if (estado==11) {
			estado++;
			tono=mi;
			PIT_LDVAL0 = (20971520/1000)*(1*300);//1us*tiempo
		}
	else if (estado==12) {
			estado++;
			tono=fa;
		}
	else if (estado==13){
		estado++;
		tono=sol;
		PIT_LDVAL0 = (20971520/1000)*(2*300);//1us*tiempo
	}
	else if (estado==14) {
			estado++;
			tono=sol;
			PIT_LDVAL0 = (20971520/2000)*(1*300);//1us*tiempo
		}
	else if (estado==15) {
			estado++;
			tono=la;
			PIT_LDVAL0 = (20971520/2000)*(1*300);//1us*tiempo
		}
	else if (estado==16) {
			estado++;
			tono=sol;
			PIT_LDVAL0 = (20971520/2000)*(1*300);//1us*tiempo
		}
	else if (estado==17) {
			estado++;
			tono=fa;
			PIT_LDVAL0 = (20971520/2000)*(1*300);//1us*tiempo
		}
	else if (estado==18){
		estado++;
		tono=mi;
		PIT_LDVAL0 = (20971520/1000)*(1*300);//1us*tiempo
	}
	else if (estado==19) {
			estado++;
			tono=doo;
		}
	else if (estado==20) {
			estado++;
			tono=sol;
			PIT_LDVAL0 = (20971520/2000)*(1*300);//1us*tiempo
		}
	else if (estado==21) {
			estado++;
			tono=la;
			PIT_LDVAL0 = (20971520/2000)*(1*300);//1us*tiempo
		}
	else if (estado==22){
		estado++;
		tono=sol;
		PIT_LDVAL0 = (20971520/2000)*(1*300);//1us*tiempo
	}
	else if (estado==23) {
			estado++;
			tono=fa;
			PIT_LDVAL0 = (20971520/2000)*(1*300);//1us*tiempo
		}
	else if (estado==24) {
			estado++;
			tono=mi;
			PIT_LDVAL0 = (20971520/1000)*(1*300);//1us*tiempo
		}
	else if (estado==25) {
			estado++;
			tono=doo;
		}
	else if (estado==26){
		estado++;
		tono=re;
	}
	else if (estado==27) {
			estado++;
			tono=sol;
		}
	else if (estado==28) {
			estado++;
			tono=doo;
			PIT_LDVAL0 = (20971520/1000)*(2*300);//1us*tiempo
		}
	else if (estado==29) {
			estado++;
			tono=re;
			PIT_LDVAL0 = (20971520/1000)*(1*300);//1us*tiempo
		}
	else if (estado==30){
		estado++;
		tono=sol;
	}
	else if (estado==31) {
			estado=0;
			tono=doo;
			PIT_LDVAL0 = (20971520/1000)*(2*300);//1us*tiempo
			FTM3_C4SC=0;
			PIT_TCTRL0 =0;
		}
	return tono;
}


uint16 estrellita(uint32 tiempo)
{
	static uint8 estado;
	static uint16 tono;
	PIT_LDVAL0 = (20971520/1000)*(1*300);
	if (estado==0){
		estado++;
		PIT_LDVAL0 = (20971520/1000)*(1*300);
		tono=doo;
	}
	else if (estado==1) {
			estado++;
			tono=doo;
		}
	else if (estado==2) {
			estado++;
			tono=sol;
		}
	else if (estado==3) {
			estado++;
			tono=sol;
		}
	else if (estado==4){
		estado++;
		tono=la;
	}
	else if (estado==5) {
			estado++;
			tono=la;
		}
	else if (estado==6) {
			estado++;
			PIT_LDVAL0 = (20971520/1000)*(2*300);
			tono=sol;
		}
	else if (estado==7) {
			estado++;
			tono=fa;
			PIT_LDVAL0 = (20971520/1000)*(1*300);
		}
	else if (estado==8){
		estado++;
		tono=fa;
	}
	else if (estado==9) {
			estado++;
			tono=mi;
		}
	else if (estado==10) {
			estado++;
			tono=mi;
		}
	else if (estado==11) {
			estado++;
			tono=re;
		}
	else if (estado==12) {
			estado++;
			tono=re;
			PIT_LDVAL0 = (20971520/1000)*(1*300);
		}
	else if (estado==13){
		estado++;
		tono=doo;
		PIT_LDVAL0 = (20971520/1000)*(2*300);
	}
	else if (estado==14) {
			estado++;
			tono=sol;
			PIT_LDVAL0 = (20971520/1000)*(1*300);
		}
	else if (estado==15) {
			estado++;
			tono=sol;
		}
	else if (estado==16) {
			estado++;
			tono=fa;
		}
	else if (estado==17) {
			estado++;
			tono=fa;
		}
	else if (estado==18){
		estado++;
		tono=mi;
	}
	else if (estado==19) {
			estado++;
			tono=mi;
			PIT_LDVAL0 = (20971520/1000)*(1*300);
		}
	else if (estado==20) {
			estado++;
			tono=re;
			PIT_LDVAL0 = (20971520/1000)*(2*300);
		}
	else if (estado==21) {
				estado++;
				tono=sol;
				PIT_LDVAL0 = (20971520/1000)*(1*300);
			}
		else if (estado==22) {
				estado++;
				tono=sol;
			}
		else if (estado==23) {
				estado++;
				tono=fa;
			}
		else if (estado==24) {
				estado++;
				tono=fa;
			}
		else if (estado==25){
			estado++;
			tono=mi;
		}
		else if (estado==26) {
				estado++;
				tono=mi;
				PIT_LDVAL0 = (20971520/1000)*(1*300);
			}
		else if (estado==27) {
				estado=0;
				tono=re;
				PIT_LDVAL0 = (20971520/1000)*(2*300);
				FTM3_C4SC=0;
				PIT_TCTRL0 = 0;
			}
	return tono;
}
