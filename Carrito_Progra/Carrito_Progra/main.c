/*
 * Carrito_Progra.c
 *
 * Created: 29/04/2026 14:39:16
 * Author : WSami
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Función para configurar el ADC (Lectura del Potenciómetro)
void ADC_Init() {
	// Referencia de voltaje en AVcc (5V)
	ADMUX = (1 << REFS0);
	// Habilitar ADC y Prescaler de 128 (16MHz/128 = 125KHz)
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS1);
}

uint16_t ADC_Read(uint8_t channel) {
	// Seleccionar canal (limpiar los últimos 4 bits y poner el canal)
	ADMUX = (ADMUX & 0xF0) | (channel & 0x0F);
	// Iniciar conversión
	ADCSRA |= (1 << ADSC);
	// Esperar a que termine
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

void PWM_Hardware_Init() {
	// Configurar Pin PB3 (OC2A / Digital 11 en Arduino) como salida
	DDRB |= (1 << DDB3);
	
	// Configurar Timer 2: Fast PWM, modo No invertido
	// WGM21:0 = 3 (Fast PWM), COM2A1 = 1
	TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20);
	// Prescaler de 64
	TCCR2B = (1 << CS22);
}

int main(void) {
	// Configurar Pin PB4 (Digital 12 en Arduino) como salida para Soft PWM
	DDRB |= (1 << DDB4);
	
	ADC_Init();
	PWM_Hardware_Init();
	
	uint16_t adc_val;
	uint8_t brillo;

	while (1) {
		// 1. Leer Potenciómetro en A0 (Canal 0)
		adc_val = ADC_Read(0);
		
		// 2. Mapear de 10 bits (0-1023) a 8 bits (0-255)
		brillo = adc_val >> 2;

		// 3. Aplicar a Hardware PWM (LED en PB3 / D11)
		OCR2A = brillo;

		// 4. Aplicar a Software PWM (LED en PB4 / D12)
		if (brillo > 0) {
			PORTB |= (1 << PORTB4);
			// Retraso proporcional al brillo
			for(int i=0; i < brillo; i++) { _delay_us(10); }
			
			PORTB &= ~(1 << PORTB4);
			// Retraso proporcional al resto del ciclo
			for(int i=0; i < (255 - brillo); i++) { _delay_us(10); }
			} else {
			PORTB &= ~(1 << PORTB4);
		}
	}
}