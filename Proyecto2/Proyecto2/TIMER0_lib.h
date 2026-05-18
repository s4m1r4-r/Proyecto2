/*
 * TIMER0_lib.h
 *
 * Created: 25/04/2026 18:53:11
 *  Author: WSami
 */ 


#ifndef TIMER0_LIB_H_
#define TIMER0_LIB_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void init_Timer0_Millis(void);
uint32_t get_millis(void);

#endif