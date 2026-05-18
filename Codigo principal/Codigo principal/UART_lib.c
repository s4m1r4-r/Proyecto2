/*
 * UART_lib.c
 *
 * Created: 05/05/2026 7:01:00
 *  Author: WSami
 */ 



#include "UART_lib.h"

void UART_Init(void) {
	UBRR0H = 0;
	UBRR0L = 8; // 115200 baudios con cristal de 16MHz
	UCSR0A |= (1 << U2X0);
	UCSR0B = (1 << RXEN0) | (1 << TXEN0);
	UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void UART_Char(char c) {
	while (!(UCSR0A & (1 << UDRE0)));
	UDR0 = c;
}

// Verifica si el bit RXC0 (Receive Complete) se puso en 1 (indica que llegó un byte)
uint8_t UART_Available(void) {
	return (UCSR0A & (1 << RXC0));
}

// Devuelve el byte almacenado en el registro UDR0 y limpia el buffer
char UART_ReadChar(void) {
	return UDR0;
}