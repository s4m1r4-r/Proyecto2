/*
 * UART_lib.h
 *
 * Created: 05/05/2026 7:00:43
 *  Author: WSami
 */ 


#ifndef UART_LIB_H_
#define UART_LIB_H_

#include <avr/io.h>
#include <stdint.h>

void UART_Init(void);
void UART_Char(char c);
uint8_t UART_Available(void); // <- NUEVA
char UART_ReadChar(void);     // <- NUEVA

#endif /* UART_LIB_H_ */