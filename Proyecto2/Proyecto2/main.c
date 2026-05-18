/*
 * Proyecto2.c
 *
 * Created: 25/04/2026 18:44:21
 * Author : Wendy Samira Hernández Rivera
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <ctype.h>

// --- CONFIGURACIÓN UART ---
void uart_init(uint32_t baud) {
	uint16_t baud_setting = (F_CPU / (16 * baud)) - 1;
	UBRR0H = (uint8_t)(baud_setting >> 8);
	UBRR0L = (uint8_t)baud_setting;
	UCSR0B = (1 << RXEN0) | (1 << TXEN0); // Habilitar RX y TX
	UCSR0C = (3 << UCSZ01); // 8 bits de datos, 1 bit de parada
}

void uart_transmit(char data) {
	while (!(UCSR0A & (1 << UDRE0))); // Esperar a que el buffer esté vacío
	UDR0 = data;
}

void uart_print(char* s) {
	while (*s) uart_transmit(*s++);
}

char uart_receive(void) {
	while (!(UCSR0A & (1 << RXC0))); // Esperar dato
	return UDR0;
}

// --- CONFIGURACIÓN ADC ---
void adc_init() {
	ADMUX = (1 << REFS0); // Referencia AVcc
	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // Prescaler 128
}

uint16_t adc_read(uint8_t ch) {
	ADMUX = (ADMUX & 0xF0) | (ch & 0x0F);
	ADCSRA |= (1 << ADSC);
	while (ADCSRA & (1 << ADSC));
	return ADC;
}

// --- PWM Y PINES ---
void init_hardware() {
	DDRB |= (1 << DDB3) | (1 << DDB4); // PB3 (D11) y PB4 (D12) como salida
	
	// Timer 2 para Hardware PWM en PB3 (OC2A)
	TCCR2A = (1 << COM2A1) | (1 << WGM21) | (1 << WGM20); // Fast PWM
	TCCR2B = (1 << CS21); // Prescaler 8
}

void mostrarMenu() {
	uart_print("\r\n--- MENU DE CONTROL PROYECTO 2 (C) ---\r\n");
	uart_print("A: Brillo ALTO (100%)\r\n");
	uart_print("M: Brillo MEDIO (50%)\r\n");
	uart_print("B: Brillo BAJO (10%)\r\n");
	uart_print("O: APAGAR LUCES\r\n");
	uart_print("S: ACTIVAR POTENCIOMETRO (Manual)\r\n");
	uart_print("--------------------------------------\r\n");
}

int main(void) {
	uart_init(115200); // Velocidad configurada según tus preferencias
	adc_init();
	init_hardware();
	
	uint8_t brillo = 0;
	uint8_t modoManual = 1;
	char comando;

	mostrarMenu();

	while (1) {
		// Revisar si hay comando serial (No bloqueante)
		if (UCSR0A & (1 << RXC0)) {
			comando = UDR0;
			if (toupper(comando) == 'S') {
				modoManual = 1;
				uart_print(">> MODO MANUAL ACTIVADO\r\n");
				} else if (comando != '\n' && comando != '\r') {
				modoManual = 0;
				if (toupper(comando) == 'A') { brillo = 255; uart_print(">> UART: ALTO\r\n"); }
				else if (toupper(comando) == 'M') { brillo = 127; uart_print(">> UART: MEDIO\r\n"); }
				else if (toupper(comando) == 'B') { brillo = 30; uart_print(">> UART: BAJO\r\n"); }
				else if (toupper(comando) == 'O') { brillo = 0; uart_print(">> UART: APAGADO\r\n"); }
			}
		}

		if (modoManual) {
			uint16_t pot = adc_read(0); // Leer A0
			brillo = pot >> 2; // Escalar 10 bits a 8 bits
		}

		// Aplicar brillo
		OCR2A = brillo; // Hardware PWM en D11[cite: 1]
		
		// Software PWM simple para D12 (PB4)
		if (brillo > 0) {
			PORTB |= (1 << PORTB4);
			_delay_us(100); // Ajuste de frecuencia manual
			if (brillo < 250) { // Si no es máximo, apagar un momento
				PORTB &= ~(1 << PORTB4);
				_delay_us(100);
			}
			} else {
			PORTB &= ~(1 << PORTB4);
		}
	}
}

void init_PWM1(void) {
	// Configurar el pin PB1 (OC1A) como salida
	DDRB |= (1 << DDB1);

	// Configurar Timer 1: Fast PWM, TOP en ICR1
	// COM1A1: Non-inverting PWM
	// WGM13 y WGM12: Modo 14 (Fast PWM con ICR1 como TOP)
	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler de 8

	// Definir el periodo de 20ms (50Hz)
	// Formula: ICR1 = (F_CPU / (Prescaler * F_PWM)) - 1
	// ICR1 = (16,000,000 / (8 * 50)) - 1 = 39,999
	ICR1 = 39999;
}

void mover_servo(uint16_t duty) {
	// El valor de OCR1A determina el ancho del pulso
	// 1ms (0 grados)  -> ~2000
	// 1.5ms (90 grados) -> ~3000
	// 2ms (180 grados) -> ~4000
	OCR1A = duty;
}

int main(void) {
	init_PWM1();

	while (1) {
		// Mover a 0 grados
		mover_servo(2000);
		_delay_ms(1000);

		// Mover a 90 grados
		mover_servo(3000);
		_delay_ms(1000);

		// Mover a 180 grados
		mover_servo(4000);
		_delay_ms(1000);
	}
}


void init_PWM2(void) {
	// Configurar el pin PB1 (OC1A) como salida
	DDRB |= (1 << DDB1);

	// Configurar Timer 1: Fast PWM, TOP en ICR1
	// COM1A1: Non-inverting PWM
	// WGM13 y WGM12: Modo 14 (Fast PWM con ICR1 como TOP)
	TCCR1A = (1 << COM1A1) | (1 << WGM11);
	TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11); // Prescaler de 8

	// Definir el periodo de 20ms (50Hz)
	// Formula: ICR1 = (F_CPU / (Prescaler * F_PWM)) - 1
	// ICR1 = (16,000,000 / (8 * 50)) - 1 = 39,999
	ICR1 = 39999;
}

void mover_servo(uint16_t duty) {
	OCR1A = duty;
}

int main(void) {
	init_PWM1();

	while (1) {
		// Mover a 0 grados
		mover_servo(2000);
		_delay_ms(1000);

		// Mover a 90 grados}
		mover_servo(3000);
		_delay_ms(1000);

		// Mover a 180 grados
		mover_servo(4000);
		_delay_ms(1000);
	}
}
