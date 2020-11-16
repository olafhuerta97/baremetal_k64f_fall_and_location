/*
 * pit.h
 *
 *  Created on: Oct 24, 2019
 *      Author: joseolafhuerta
 */

#ifndef PIT_H_
#define PIT_H_
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned long uint32;

void pit_init(uint32 tiempo);
void pit1_init(uint32 tiempo);

#endif /* PIT_H_ */
