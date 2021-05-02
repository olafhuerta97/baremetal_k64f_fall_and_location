/*
 * uart.c
 *
 *  Created on: Sep 29, 2019
 *      Author: joseolafhuerta
 */

#include "uart.h"
#include "derivative.h" 

static uint8 datos=0;
static uint8 Flag = 0;
static UART_GPS St_GpsData = {0};

void UART0_init(void)
{
	// configuracion como salida
	SIM_SCGC4|=SIM_SCGC4_UART0_MASK ; //(1<<10)  Habilitar CLK para el UART0
	SIM_SCGC5|=SIM_SCGC5_PORTB_MASK ;  //(1<<10)
	PORTB_PCR16=PORT_PCR_MUX(0x3);     //(1<<8)+(1<<9)     //Configurar el MUX para el UART0
	PORTB_PCR17=PORT_PCR_MUX(0x3);    //    (1<<8)+(1<<9);
	UART0_BDL=137;
	UART0_C2|=UART_C2_RE_MASK;// re(1<<2)
	UART0_C2|=UART_C2_TE_MASK;  //te (1<<3)
	//UART0_C2|=UART_C2_TIE_MASK;//(1<<7) salida  
	NVICISER0|=(1<<(31%32));    //Rutina de interrupción del UART
}
void UART1_init(void)
{ 
	// configuracion como entrada
	SIM_SCGC4|=SIM_SCGC4_UART1_MASK ; //(1<<10)  
	SIM_SCGC5|=SIM_SCGC5_PORTC_MASK ;  //
	PORTC_PCR3=PORT_PCR_MUX(0x3);     //   rx //Configurar el MUX para el UART0
	PORTC_PCR4=PORT_PCR_MUX(0x3);    //    tx
	UART1_BDL=137;   
	UART1_C2|=UART_C2_RE_MASK;// re(1<<2)
	UART1_C2|=UART_C2_TE_MASK;  //te (1<<3)
	UART1_C2|=UART_C2_RIE_MASK; //(1<<5) entrada enable
	NVICISER1|=(1<<(33%32));    //Rutina de interrupción del UART
}

uint8 UART_GpsDataProcessing (DtAndHrSnd *fhs, uint64 *u64AverageLat, uint64 *u64AverageLon, uint8 gps, uint32 *cnt)
{ 
	gpsparser(gps);
	if (St_GpsData.u8Done == 1)
	{
		St_GpsData.u32Latitude=gradosadecimal(St_GpsData.u32Latitude);
		St_GpsData.u32Longitude=gradosadecimal(St_GpsData.u32Longitude);
		if (St_GpsData.u32Latitude >= 1030000000  &&  St_GpsData.u32Latitude <= 1040000000 &&  St_GpsData.u32Longitude>= 200000000 &&  St_GpsData.u32Longitude<= 210000000)
		{
			*u64AverageLat = *u64AverageLat + St_GpsData.u32Latitude;
			*u64AverageLon = *u64AverageLon + St_GpsData.u32Longitude;
			*cnt= *cnt+1;
			
		}
		St_GpsData.u8Done = 0;
		if(St_GpsData.u32DateHour != 0)
		{
			fhs->fhs1 = (uint8)(St_GpsData.u32DateHour);
			fhs->fhs2 = (uint8)(St_GpsData.u32DateHour >> 8);
			fhs->fhs3 = (uint8)(St_GpsData.u32DateHour >> 16);
			fhs->fhs4 = (uint8)(St_GpsData.u32DateHour >> 24);	
		}
		else
		{
			
		}
	}
	else
	{ 
		//nop//	
	}		
	return 0;
}

uint8 UART0_senddata(uint32 envio)
{
	//enviar dato por uart0
	static uint8 envio_ASCII[]={"          \n\r"};
	
	(void)UART0_S1;
	if (datos==0)
	{
		envio_ASCII[9]=(envio%10)+'0';
						envio/=10;
		envio_ASCII[8]=(envio%10)+'0';
				envio/=10;
		envio_ASCII[7]=(envio%10)+'0';
		envio/=10;
		envio_ASCII[6]=(envio%10)+'0';
		envio/=10;
		envio_ASCII[5]=(envio%10)+'0';
		envio/=10;
		envio_ASCII[4]=(envio%10)+'0';
		envio/=10;
		envio_ASCII[3]=(envio%10)+'0';
		envio/=10;
		envio_ASCII[2]=(envio%10)+'0';
		envio/=10;
		envio_ASCII[1]=(envio%10)+'0';
		envio_ASCII[0]=envio/10+'0';
		Flag = Busy;
	}
	UART0_D=envio_ASCII[datos++];
	
	if (envio_ASCII[datos]==0)
	{
		datos=0;
		Flag = Ready;
	}
	return Flag;
}

uint8 fuga (sint64 x0, sint64 y0, sint64 x1, sint64 y1, sint64 x2, sint64 y2, sint64 x3, sint64 y3, sint64 xp, sint64 yp){
	uint8 f=0;
	sint64 m1=0; 
	sint64 m2=0; 
	sint64 m3=0; 
	sint64 m4=0; 
	sint64 b1=0; 
	sint64 b2=0; 
	sint64 b3=0; 
	sint64 b4=0;
	m1 = pendiente(x0,y0,x1,y1); //pendiente 1
	m2 = pendiente(x1,y1,x2,y2); //pendiente 2
	m3 = pendiente(x2,y2,x3,y3); //pendiente 3
	m4 = pendiente(x3,y3,x0,y0); //pendiente 4
	b1=y0-(m1*x0)/1000; //b para pendiente 1
	b2=y1-(m2*x1)/1000; //b para pendiente 2
	b3=y2-(m3*x2)/1000; //b para pendiente 3
	b4=y3-(m4*x3)/1000; //b para pendiente 4
	if(yp>((m1*xp)/1000+b1) && yp<((m2*xp)/1000+b2) && yp<((m3*xp)/1000+b3) && yp>((m4*xp)/1000+b4)){
	f=0xC1;
	}
	else f=0;
	return f;
}

sint64 pendiente (sint64 X0, sint64 Y0, sint64 X1, sint64 Y1){
	sint64 M=0;
		Y0=Y0*1000;
		Y1=Y1*1000;
		M =(sint64)((Y1-Y0)/(X1-X0));
	return M;
}

void gpsparser(uint8 entrada)
{
	static uint32  dato;
	static uint16 hora;
	static uint8  estado;
	static uint32 fecha;
	static uint8 valid; 
	
	if ('$'==entrada)
	{
		estado =1;
		dato=0;
		hora=0;
		fecha=0;
		valid=0;
	}
	else if ('G'==entrada && 1 == estado)
	{
		estado++;
	}
	else if ('P'==entrada && 2 == estado)
	{
		estado++;
	}
	else if ('R'==entrada && 3 == estado)
	{
		estado++;
	}
	else if ('M'==entrada && 4== estado)
	{
		estado++;
	}
	else if ('C'==entrada && 5== estado)
	{
		estado++;
	}
	else if (','==entrada && 6== estado)
	{
		estado++;
	}
	else if (',' != entrada && 7 == estado )
	{
		if(entrada >= '0'&& entrada<='9')
		{
			dato=(dato*10)+(entrada-'0');
		} 
	}
	else if (',' ==entrada && 7 == estado )
	{
		hora = dato/10000; 
		dato=0;
		estado++;
	}
	else if ('A'==entrada && 8== estado)
	{
		valid=1;
		estado++;	
	}
	else if ('V'==entrada && 8== estado)
	{
		valid=0;
		estado++;	
	}
	else if (','==entrada && 9== estado){estado++;}
	else if (',' != entrada && 10 == estado )
	{
		if(entrada >= '0' && entrada<='9'){dato=(dato*10)+(entrada-'0');} 
	}
	else if (',' == entrada && 10 == estado )
	{
		St_GpsData.u32Longitude = dato; 
		asm ("nop");
		dato=0;
		estado++;
	}
	else if (','==entrada && 11== estado){estado++;}
	else if (',' != entrada && 12 == estado )
	{
		if(entrada >= '0' && entrada<='9'){dato=(dato*10)+(entrada-'0');} 
	}
	else if (',' == entrada && 12 == estado )
	{
		St_GpsData.u32Latitude = dato; 
		dato=0;
		estado++;
	}
	else if (','==entrada && 13== estado){estado++;}
	else if (','==entrada && 14== estado){estado++;}
	else if (','==entrada && 15== estado){estado++;}
	else if (',' != entrada && 16 == estado )
	{
		if(entrada >= '0' && entrada<='9'){dato=(dato*10)+(entrada-'0');} 
	}
	else if (',' == entrada && 16 == estado )
	{
		fecha = dato; 
		St_GpsData.u32DateHour=0;
		dato=0;
		if(valid == 1 && hora < 2400  && fecha < 320000  && fecha > 010100)
		{
			St_GpsData.u32DateHour=(fecha<<12)|(hora);
			St_GpsData.u8Done = 1;
		}
		else
		{
			
		}
	}
	else
	{
	//nop	
	}
}

uint32 gradosadecimal(uint32 grados)
{
	uint32 enteros2;
	uint32 enteros1;
	uint32 decimal;
	enteros1= (grados/10000000)*10000000;  //grados
	enteros2= ((grados%10000000)*(10))/6;  //minutos
	decimal=enteros1+enteros2;
    return decimal;
}
	
void transformdate(uint16 *time,  uint32 *date )
{
	uint8 mes;
    mes=(*date/100)%100;
	if (*time>=600)	*time=*time-600;
	else 
	{
	*time=*time+1800;
		if(*date>10000 && *date<20000)
		{		
			if(mes==1)*date=*date+301099;
			else if(mes==5 || mes==7 || mes==10 || mes==12) *date=*date+289900;	
			else if(mes==2 || mes==4 || mes==6 || mes==8  || mes==9 || mes==11) *date=*date+299900;
			else if(mes==3) *date=*date+279900;
		}
		else  *date=*date-10000;
	}	
}	

