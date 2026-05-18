/*
 * TIMER0_lib.c
 *
 * Created: 25/04/2026 18:53:41
 *  Author: WSami
 */ 


#include "Timer0_lib.h"

volatile uint32_t millis_counter = 0;

void init_Timer0_Millis(void) {
	// Modo CTC
	TCCR0A = (1 << WGM01);
	// Prescaler 64
	TCCR0B = (1 << CS01) | (1 << CS00);
	// Valor de comparación
	//OCR0A = 249;
	// Habilitar interrupción por comparación
	TIMSK0 |= (1 << OCIE0A);
	sei();
}

ISR(TIMER0_COMPA_vect) {
	millis_counter++;
}

uint32_t get_millis(void) {
	uint32_t temp;
	cli(); // Deshabilitar lectura
	temp = millis_counter;
	sei();
	return temp;
}