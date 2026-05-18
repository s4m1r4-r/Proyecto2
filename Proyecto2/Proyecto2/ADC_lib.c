/*
 * ADC_lib.c
 *
 * Created: 25/04/2026 18:49:35
 *  Author: WSami
 */ 


#include "ADC_lib.h"

void init_ADC(void) {
	// Referencia de voltaje AVcc (5V)
	ADMUX = (1 << REFS0);
	// Habilitar ADC y Prescaler de 128 
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t read_ADC(uint8_t channel) {
	// Seleccionar 
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	// Iniciar conversión
	ADCSRA |= (1 << ADSC);
	// Esperar a que termine
	while (ADCSRA & (1 << ADSC));
	return ADC;
}