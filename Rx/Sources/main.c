/* main implementation: use this 'C' sample to create your own application
 *
 */
#include "derivative.h" /* include peripheral declarations */
#include "nrf24.h"
#include "pit.h"
#include "lcd.h"
#include "ftm.h"
#include "i2c.h"
#include "uart.h"

//declaracion var
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef signed short sint16;

//prototipado de funciones
void dataprocess(void);
void enter_wait(void);
void sleep (void);
void vButton_Init(void);

//variables
uint8 buf[5]={0};
uint16 hora;
uint32 fecha;
uint32 tono=0;
uint32 frecuencia=440;
uint8 song=0;

int main(void){
	I2C_init();//For Memory EEprom
	LPTMR_init(); // For LCD delay
	PORT_init();   // For LCD
	LCD_init();  // LCD init
	pit_init(300); // Periodic Interrupt Timer
	FTM3c4_init_outputcompare(); //Flextimer
	SPI1_Init();
	RF24_Initrx();
	vButton_Init();
	vEvent_init();
	while(1)
	{
		enter_wait();
	}
	return 0;
}

void FTM3_IRQHandler(){ 
	uint32 frecuencia=440;//Función de interrupción para el FT3
	if (((FTM3_C4SC) & (FTM_CnSC_CHF_MASK) ) && ((FTM3_C4SC) & (FTM_CnSC_CHIE_MASK)))
	{      //Se condiciona que las banderas estén prendidas (se prenden cada que se hace un toggle)
		frecuencia =(20971520)/(2*(tono));                           //Si se cumple, se calcula la frecuencia
		FTM3_C4SC&=~(FTM_CnSC_CHF_MASK);                       //Apaga la bandera
		FTM3_C4V+=frecuencia;                     //Escribe la nueva frecuencia
	}
}

void PIT0_IRQHandler()
{
	PIT_TFLG0= PIT_TFLG_TIF_MASK;
	if (song==1)
		tono=martinillo(200);
	else if (song==2)
		tono=estrellita(200);	
}

void PORTC_IRQHandler()
{
	uint8 status;
	uint16 counter;
	PORTC_PCR18|=PORT_PCR_ISF_MASK;  //Turn down flag
	status =RF24_Status();
	if (status&RX_DR) /* data received interrupt */
	{
		pin_CELOW(); /* need to disable rx mode during reading RX data */
		RF24_WriteRegister(STATUS1,RX_DR); /* clear bit */
		Read5RegRf(R_RX_PAYLOAD,&buf[0]);
		hora=((((0x0F)&(buf[3]))<<8)|buf[4]);
		fecha=(( buf[1]<<12)|( buf[2]<<4)|(((0xF0)&(buf[3]))>>4));
		if(hora && fecha)
		{
			transformdate(&hora, &fecha);
		}
		counter=Save_event_data(fecha, hora, buf[0]);
		vSearch_Event((counter-5));
		dataprocess();
		pin_CEHIGH(); /* re-enable rx mode */
	}
	if (status&TX_DS) /* data sent interrupt */
	{ 
		pin_CELOW(); /* need to disable rx mode during reading RX data */
		RF24_WriteRegister(STATUS1,TX_DS); /* clear bit */
		pin_CEHIGH(); /* re-enable rx mode */  
	}
	if (status&MAX_RT) /* retry timeout interrupt */
	{ 
		pin_CELOW(); /* need to disable rx mode during reading RX data */  
		RF24_WriteRegister(STATUS1,MAX_RT); /* clear bit */
		pin_CEHIGH(); /* re-enable rx mode */  
	}
}

void PORTB_IRQHandler(){
	static sint16 i;
	static uint16 counterb;
	signed long sum =0;
	song=0;
	PIT_TCTRL0 =0;
	FTM3_C4SC=0;
	counterb= (ReadfromMemoryCounter())-5;
	if(PORTB_PCR2&PORT_PCR_ISF_MASK)
	{
		PORTB_PCR2|=PORT_PCR_ISF_MASK;
		i=i+5;
		sum=counterb+i;
		if ((sum) <= (counterb))
		{
			vSearch_Event(counterb+i);
		}
		else
		{
			i=i-5;	
		}
	}
	else if(PORTB_PCR3&PORT_PCR_ISF_MASK)
	{
		PORTB_PCR3|=PORT_PCR_ISF_MASK;
		i=i-5;
			if ((counterb+i)>=(2)){
				vSearch_Event(counterb+i);
			}
			else 
			{
				i=i+5;
			}
	}
}


  void sleep (void)
{
    SCB_SCR &= ~SCB_SCR_SLEEPDEEP_MASK;
    asm("WFI");/* WFI instruction will start entry into WAIT mode */
}

  void enter_wait(void)
 {
  sleep();
 }
  
void dataprocess(void)
{	
  	if ((buf[0]&(0xF0))==0xC0)
  	{	
		buf[0]=0;
		FTM3_C4SC=FTM_CnSC_ELSA_MASK|FTM_CnSC_MSA_MASK |FTM_CnSC_CHIE_MASK ;
		song=1;
		PIT_TCTRL0 = PIT_TCTRL_TEN_MASK|PIT_TCTRL_TIE_MASK;	
  	}
  	if ((buf[0]&(0xF0))==0xF0)
  	{
		buf[0]=0;
		FTM3_C4SC=FTM_CnSC_ELSA_MASK|FTM_CnSC_MSA_MASK |FTM_CnSC_CHIE_MASK ;
		song=2;
		PIT_TCTRL0 = PIT_TCTRL_TEN_MASK|PIT_TCTRL_TIE_MASK;
  	}
}
  
void vButton_Init(void)
{
	SIM_SCGC5 |=SIM_SCGC5_PORTB_MASK;
	PORTB_PCR2= PORT_PCR_IRQC(9)|PORT_PCR_MUX(1)|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_PFE_MASK;//GPIO INTERRUPCION BOTON
	PORTB_PCR3= PORT_PCR_IRQC(9)|PORT_PCR_MUX(1)|PORT_PCR_PS_MASK|PORT_PCR_PE_MASK|PORT_PCR_PFE_MASK;
	NVICISER1|=1<<(60%32);  
}

