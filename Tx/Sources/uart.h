/*
 * uart.h
 *
 *  Created on: Sep 29, 2019
 *      Author: joseolafhuerta
 */

#ifndef UART_H_
#define UART_H_

#define Busy (unsigned char)-1
#define Ready 1
#define Error 0


typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef signed long sint32;
typedef unsigned long long uint64;
typedef signed long long sint64;


typedef struct 
{
	uint32 u32Latitude;
	uint32 u32Longitude;
	uint32 u32DateHour;
	uint8  u8Done;
}UART_GPS;

typedef struct
{
	uint8 fhs1;
	uint8 fhs2;
	uint8 fhs3;
	uint8 fhs4;
}DtAndHrSnd;

void UART0_init(void);
void UART1_init(void);
uint8 UART0_senddata(uint32 envio);
uint8 fuga (sint64 x0, sint64 yo, sint64 x1, sint64 y1, sint64 x2, sint64 y2, sint64 x3, sint64 y3, sint64 xp, sint64 yp);
sint64 pendiente (sint64 X0, sint64 Y0, sint64 X1, sint64 Y1);
void gpsparser(uint8 entrada);
UART_GPS UART_u8GiveMeYourData(void);
uint8 UART_GpsDataProcessing (DtAndHrSnd *fhs, uint64 *u64AverageLat, uint64 *u64AverageLon, uint8 gps, uint32 *cnt);
uint32 gradosadecimal(uint32 grados);

#endif /* UART_H_ */
