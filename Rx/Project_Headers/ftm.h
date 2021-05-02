/*
 * ftm.h
 *
 *  Created on: Oct 24, 2019
 *      Author: joseolafhuerta
 */

#ifndef FTM_H_
#define FTM_H_

typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

void FTM3c4_init_outputcompare(void);
uint16 estrellita(uint32 tiempo);
uint16 martinillo(uint32 tiempo);

#endif /* FTM_H_ */
