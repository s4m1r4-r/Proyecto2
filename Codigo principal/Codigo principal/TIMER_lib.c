/*
 * TIMER_lib.c
 *
 * Created: 05/05/2026 6:59:54
 *  Author: WSami
 */ 


#include "TIMER_lib.h"

volatile uint32_t ms_contador = 0;

void Timer_Tempo_Init(void) {
	TCCR2A = 0x00;       // Modo Normal
	TCCR2B = (1 << CS22); // Prescaler 64
	TCNT2 = 6;           // Precarga para contar 250 ticks (1ms)
	TIMSK2 |= (1 << TOIE2); // Habilitar interrupción por desbordamiento
}

uint32_t get_millis(void) {
	uint32_t ms_copia;
	uint8_t sreg_bak = SREG;
	cli();               // Deshabilitar interrupciones para lectura segura
	ms_copia = ms_contador;
	SREG = sreg_bak;     // Restaurar interrupciones
	return ms_copia;
}

ISR(TIMER2_OVF_vect) {
	TCNT2 = 6;           // Reiniciar precarga
	ms_contador++;       // Sumar 1 milisegundo
}