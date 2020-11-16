/* main implementation: use this 'C' sample to create your own application
 *
 */

#include "derivative.h" /* include peripheral declarations */
#include "uart.h"
#include "nrf24.h"
#include "i2c.h"
#include "lptmr.h"
#include "pit.h" 
#include "adc.h" 

#define punto0x    207337772//207330209//207366975//207367001//20721175
#define punto0y    1034570482//1034569082//1034566605//1034565988//103385425
#define punto1x    207340984//207336041//207377034//207377258//20722495
#define punto1y    1034570146//1034568420//1034565192//1034565496//103385277
#define punto2x    207341414//207336225//207377055//207377427//20722704
#define punto2y    1034576642//1034571635//1034570196//1034570235//103387506
#define punto3x    207338110//207330470//207366813//207367216//20721125
#define punto3y    1034576969//1034572017//1034571060//1034571214//103387169

//declaracion var
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef signed long sint32;
typedef unsigned long long uint64;
uint8 test=0;
//prototipado de funciones

//variables
static uint8 eventid = 0;
static uint64 u64AverageLat = 0;
static uint64 u64AverageLon = 0;
static uint32 latav=0;
static uint32 lonav=0;
static uint8 estado=0;  // quitar
DtAndHrSnd fhs = {0};
DtAndHrSnd *fhspointer;
uint32 contador=0;
uint16 u16filter=(uint16)-1;

int main(void)
{
	pit_init(10000);
	ADC0_SE18_init();
	leds_init();
	UART1_init(); //inicializar UART1
	UART0_init();
	SPI1_Init();  //inicializar SPI1
	RF_INITtx();   //inicializar RF24
	I2C_init();   //inicializar I2C0
	ACCEL_init();  // inicializar accelerometro
	LPTMR_init(10000);
	while(1)
	{
		while (eventid)  
		{
			RF24send5bytes(eventid,fhs.fhs4,fhs.fhs3,fhs.fhs2, fhs.fhs1);
			Pause(30000);
		}
	}
	return 0;
}

void ADC1_IRQHandler(void)// interrupcion ADC1
{
	unsigned short u16data_ADC;
	u16data_ADC=ADC1_RA;
	movmean50(u16data_ADC,&u16filter);
	if(u16filter>=29000)
	{
		GreenHIGH();
		YellowHIGH(); 
		RedHIGH();				
	}
	if(u16filter>28000&&u16filter<29000)
	{
		GreenLOW();
		YellowHIGH(); 
		RedHIGH();		
	}
	if(u16filter<=27000)
	{
		GreenLOW();
		YellowLOW(); 
		RedHIGH();
	}
}

void PIT0_IRQHandler(void)
{
	PIT_TFLG0|= PIT_TFLG_TIF_MASK;
	ADC1_SC1A= (ADC_SC1_ADCH(18)|ADC_SC1_AIEN_MASK);//Canal y enable adc
}




void UART1_Status_IRQHandler()   //prioridad 31
{
	uint8 gps;
	(void)UART1_S1;
	gps=UART1_D; 
	fhspointer = &fhs;
	UART_GpsDataProcessing(fhspointer, &u64AverageLat, &u64AverageLon, gps, &contador);
}

void UART0_Status_IRQHandler()   //prioridad 31
{
	uint8 flag=0;
	(void)UART0_S1;
	if (estado==0){
	flag=UART0_senddata(latav);
	if (flag==1)
	{
		estado++;
	}
	}
	if (estado==1){
	flag=UART0_senddata(lonav);
	if (flag==1)
	{
		UART0_C2&=~UART_C2_TIE_MASK;
		estado=0;
	}
	}
}

void LPTimer_IRQHandler(){  // prioridad 58
	LPTMR0_CSR|=LPTMR_CSR_TCF_MASK;
	UART0_C2|=UART_C2_TIE_MASK;
	latav=u64AverageLat/contador;
	lonav=u64AverageLon/contador;
	u64AverageLat = 0;
	u64AverageLon = 0;
	contador=0;
	if(latav &&  lonav)//latav &&  lonav | 1 
	{
	eventid=fuga( punto0x ,punto0y, punto1x , punto1y , punto2x, punto2y, punto3x, punto3y, latav, lonav);
	}
}

void PORTC_IRQHandler()  //prioridad 61
{
    uint8 status;
    if(PORT_PCR_ISF_MASK & PORTC_PCR6) //interrupcion accel
    {
    	PORTC_PCR6 |= PORT_PCR_ISF_MASK;			// Clear the interrupt flag
        eventid=0xC1;
    }
    else
    {
    	
    }
    if(PORT_PCR_ISF_MASK & PORTC_PCR18)    //interrupcion rf
    {
    	PORTC_PCR18|=PORT_PCR_ISF_MASK;  // apagar bandera
		status = RF24_Status();
	 	eventid= apagarbanderaydatoenviado(status);
    }
    else
    {
    	
    }
}




	
