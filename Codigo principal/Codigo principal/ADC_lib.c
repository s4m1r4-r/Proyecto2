/*
 * CFile1.c
 *
 * Created: 05/05/2026 6:57:59
 *  Author: WSami
 */ 

#include "ADC_lib.h"

void ADC_Init(void) {
	// Referencia AVCC (5V) y ajuste a la izquierda (ADLAR=1) para leer solo 8 bits (ADCH)
	ADMUX = (1 << REFS0) | (1 << ADLAR);
	// Habilitar ADC y Prescaler de 128 (16MHz/128 = 125kHz)
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint8_t ADC_Read(uint8_t canal) {
	// Seleccionar canal analógico (0 a 3 para tus Joysticks y Potenciómetro)
	ADMUX = (ADMUX & 0xF0) | (canal & 0x0F);
	// Iniciar conversión
	ADCSRA |= (1 << ADSC);
	// Esperar a que termine la conversión
	while (ADCSRA & (1 << ADSC));
	// Retornar el valor de 8 bits (0-255)
	return ADCH;
}