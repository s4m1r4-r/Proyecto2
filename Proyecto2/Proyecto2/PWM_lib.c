/*
 * PWM_lib.c
 *
 * Created: 25/04/2026 18:50:49
 *  Author: WSami
 */ 


#include "PWM_lib.h"

void init_PWM_Servos(void) {
	// Configurar pines de PWM como salida (OC1A, OC1B, OC3A, OC3B)
	DDRB |= (1 << PB1) | (1 << PB2); // Timer 1
	DDRD |= (1 << PD2) | (1 << PD3); // Timer 3 (Pines específicos del 328PB)

	// Valor para PWM 
	
	//ICR1 = 39999;
	//ICR3 = 39999;

	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);

	TCCR3A = (1 << COM3A1) | (1 << COM3B1) | (1 << WGM31);
	TCCR3B = (1 << WGM33) | (1 << WGM32) | (1 << CS31);
}

void set_servo_pos(uint8_t servo_num, uint16_t val_adc) {
	// Mapeo simple
	//uint16_t pulse = 1000 + (uint32_t)val_adc * 3000 / 1023;
	
	switch(servo_num) {
		case 0: OCR1A = pulse; break;
		case 1: OCR1B = pulse; break;
		case 2: OCR3A = pulse; break;
		case 3: OCR3B = pulse; break;
	}
}