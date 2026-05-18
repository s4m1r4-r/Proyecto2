/*
 * TIMER_lib.h
 *
 * Created: 05/05/2026 7:00:19
 *  Author: WSami
 */ 


#ifndef TIMER_LIB_H_
#define TIMER_LIB_H_

#include <avr/io.h>
#include <avr/interrupt.h>

void Timer_Tempo_Init(void);
uint32_t get_millis(void);

#endif