/*
 * lcd.h
 *
 *  Created on: Oct 25, 2019
 *      Author: joseolafhuerta
 */

#ifndef LCD_H_
#define LCD_H_

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;
typedef signed long sint32;
typedef unsigned long long uint64;

#define D7 (1<<1)
#define D6 (1<<3)
#define D5 (1<<2)
#define D4 (1<<0)
#define E (1<<3)
#define RS (1<<2) 

void PORT_init(void);
void LCD_init(void);
void rsIntruction(void);
void enable(void);
uint32 outputPorts(uint32 dato);
void data(void);
void rst(void);
void pass(void);
void message(uint8 cadena[16]);
void delay(uint16 tiempo_ms);
void LPTMR_init(void);
void detect_datenhour(uint32 fecha,uint16 hora,uint8 *linetochange);
void vEvent_init(void);
void vnew_Event(uint16 counter);
void vSearch_Event(uint16 counter);


#endif /* LCD_H_ */
