/*
 * PWM_lib.c
 *
 * Created: 05/05/2026 6:59:27
 *  Author: WSami
 */ 


#include "PWM_lib.h"

void PWM_Init(void) {
	// --- TIMER 0: Motores DC (PD6/D6 y PD5/D5) ---
	// Fast PWM de 8 bits, salidas No Invertidas
	TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM01) | (1 << WGM00);
	TCCR0B = (1 << CS01) | (1 << CS00); // Prescaler 64

	// --- TIMER 1: Servos (PB1/D9 y PB2/D10) ---
	// Modo 14 (Fast PWM con tope en ICR1), salidas No Invertidas
	TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler 8
	ICR1 = 39999; // Frecuencia exacta de 50Hz (Periodo de 20ms)
}

void Set_Motor_Velocidad(uint8_t izq, uint8_t der) {
	OCR0A = der; // Motor Derecho (D6)
	OCR0B = izq; // Motor Izquierdo (D5)
}

void Set_Servo_Direccion(uint16_t pulso) {
	OCR1A = pulso; // Servo Delantero (D9)
}

void Set_Servo_Puerta(uint16_t pulso) {
	OCR1B = pulso; // Servo Puerta (D10)
}