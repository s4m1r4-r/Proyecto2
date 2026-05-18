/*
 * =====================================================
 * PROYECTO CARRITO UVG - VERSION FINAL COMPLETA DEFINITIVA
 * MODO 0: MANUAL (JOYSTICK)
 * MODO 1: EEPROM (GRABACIÓN Y REPRODUCCIÓN)
 * MODO 2: ADAFRUIT IO (CONTROL SERIAL REMOTO)
 * =====================================================
 * Autor: SAMIRA RIVERA
 */

#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <avr/eeprom.h>

#include "ADC_lib.h"
#include "PWM_lib.h"
#include "TIMER_lib.h"
#include "UART_lib.h"

//====================================================================
// LECTURA SERIAL LOCAL
//====================================================================
uint8_t Leer_Dato_Serial_Local(void)
{
	if (UCSR0A & (1 << RXC0))
	{
		return UDR0;
	}
	return 0;
}

#define MODO_MANUAL   0
#define MODO_EEPROM   1
#define MODO_ADAFRUIT 2

#define MAX_PASOS 30

uint8_t modo_actual = MODO_MANUAL;

//=========================================
// ACTUALIZAR LEDS INDICADORES
//=========================================
void Actualizar_LEDs(void)
{
	PORTB &= ~(1 << PORTB4); // D12 VERDE
	PORTB &= ~(1 << PORTB5); // D13 AZUL
	PORTC &= ~(1 << PORTC5); // A5 ROJO

	if(modo_actual == MODO_MANUAL)
		PORTB |= (1 << PORTB4);

	else if(modo_actual == MODO_EEPROM)
		PORTB |= (1 << PORTB5);

	else if(modo_actual == MODO_ADAFRUIT)
		PORTC |= (1 << PORTC5);
}

//=========================================
// MAIN PROGRAM
//=========================================
int main(void)
{
	//=========================================
	// LEDS INDICADORES
	//=========================================
	DDRB |= (1 << DDB4); // D12
	DDRB |= (1 << DDB5); // D13
	DDRC |= (1 << DDC5); // A5
	DDRB |= (1 << DDB3); // D11 FAROS

	//=========================================
	// MOTORES
	//=========================================
	DDRD |= (1 << DDD5); // D5 ENA
	DDRD |= (1 << DDD6); // D6 ENB
	DDRD |= (1 << DDD3); // D3 -> IN3
	DDRD |= (1 << DDD4); // D4 -> IN4

	//=========================================
	// SERVOS
	//=========================================
	DDRB |= (1 << DDB1); // D9
	DDRB |= (1 << DDB2); // D10

	//=========================================
	// BOTÓN CAMBIO DE MODOS
	//=========================================
	DDRD &= ~(1 << DDD2);
	PORTD |= (1 << PORTD2);

	//=========================================
	// BOTÓN GRABACIÓN EEPROM
	//=========================================
	DDRC &= ~(1 << DDC4);
	PORTC |= (1 << PORTC4);

	//=========================================
	// INICIALIZACIÓN
	//=========================================
	ADC_Init();
	PWM_Init();
	Timer_Tempo_Init();
	UART_Init();

	Actualizar_LEDs();

	Set_Servo_Direccion(3250);
	Set_Servo_Puerta(1500);

	PORTB &= ~(1 << PORTB3);

	//=========================================
	// VARIABLES
	//=========================================
	uint8_t boton_modo_anterior = 1;
	uint8_t boton_grab_anterior = 1;

	uint8_t joy_direccion;
	uint8_t joy_movimiento;
	uint8_t joy_puerta;
	uint8_t pot_velocidad;

	// EEPROM
	uint16_t direccion_escritura = 0;
	uint16_t direccion_lectura = 0;
	uint8_t estado_puerta_reproducido = 0;

	// ADAFRUIT
	uint8_t dato_serial = 0;

	//=========================================
	// LOOP PRINCIPAL
	//=========================================
	while(1)
	{
		//=========================================
		// CAMBIO DE MODOS
		//=========================================
		uint8_t boton_modo_actual = (PIND & (1 << PIND2));

		if((boton_modo_anterior != 0) && (boton_modo_actual == 0))
		{
			_delay_ms(30);

			if(!(PIND & (1 << PIND2)))
			{
				modo_actual++;

				if(modo_actual > MODO_ADAFRUIT)
					modo_actual = MODO_MANUAL;

				Set_Motor_Velocidad(0, 0);

				PORTD &= ~(1 << PORTD3);
				PORTD &= ~(1 << PORTD4);

				direccion_lectura = 0;
				dato_serial = 0; // Reseteamos comando serial al cambiar modo

				Actualizar_LEDs();
			}
		}

		boton_modo_anterior = boton_modo_actual;

		//=========================================================
		// MODO 0: MANUAL
		//=========================================================
		if(modo_actual == MODO_MANUAL)
		{
			joy_direccion  = ADC_Read(0);
			joy_movimiento = ADC_Read(1);
			joy_puerta     = ADC_Read(2);
			pot_velocidad  = ADC_Read(3);

			//=========================================
			// DIRECCIÓN
			//=========================================
			uint16_t pulso_direccion =
			2000 + ((uint32_t)joy_direccion * 2500UL / 255UL);

			Set_Servo_Direccion(pulso_direccion);

			//=========================================
			// PUERTA
			//=========================================
			if(joy_puerta > 140)
			{
				Set_Servo_Puerta(3000);
				PORTB |= (1 << PORTB3);
			}
			else if(joy_puerta < 110)
			{
				Set_Servo_Puerta(1500);
				PORTB &= ~(1 << PORTB3);
			}

			//=========================================
			// MOTORES OUT3 / OUT4
			//=========================================
			if (joy_movimiento > 140)
			{
				PORTD |=  (1 << PORTD3);
				PORTD &= ~(1 << PORTD4);

				Set_Motor_Velocidad(0, pot_velocidad);
			}
			else if (joy_movimiento < 110)
			{
				PORTD &= ~(1 << PORTD3);
				PORTD |=  (1 << PORTD4);

				Set_Motor_Velocidad(0, pot_velocidad);
			}
			else
			{
				PORTD &= ~(1 << PORTD3);
				PORTD &= ~(1 << PORTD4);

				Set_Motor_Velocidad(0, 0);
			}

			//=========================================
			// GRABAR EEPROM
			//=========================================
			uint8_t boton_grab_actual = (PINC & (1 << PINC4));

			if((boton_grab_anterior != 0) && (boton_grab_actual == 0))
			{
				_delay_ms(30);

				if(!(PINC & (1 << PINC4)))
				{
					if(direccion_escritura < MAX_PASOS)
					{
						eeprom_update_byte(
						(uint8_t*)direccion_escritura,
						joy_puerta);

						direccion_escritura++;

						// PARPADEO FAROS
						PORTB |= (1 << PORTB3);
						_delay_ms(150);
						PORTB &= ~(1 << PORTB3);
					}
				}
			}

			boton_grab_anterior = boton_grab_actual;
		}

		//=========================================================
		// MODO 1: EEPROM
		//=========================================================
		else if(modo_actual == MODO_EEPROM)
		{
			Set_Motor_Velocidad(0, 0);

			PORTD &= ~(1 << PORTD3);
			PORTD &= ~(1 << PORTD4);

			if(direccion_escritura == 0)
			{
				Set_Servo_Puerta(1500);
			}
			else
			{
				estado_puerta_reproducido =
				eeprom_read_byte((uint8_t*)direccion_lectura);

				if(estado_puerta_reproducido > 140)
				{
					Set_Servo_Puerta(3000);
					PORTB |= (1 << PORTB3);
				}
				else
				{
					Set_Servo_Puerta(1500);
					PORTB &= ~(1 << PORTB3);
				}

				_delay_ms(800);

				direccion_lectura++;

				if(direccion_lectura >= direccion_escritura ||
				   direccion_lectura >= MAX_PASOS)
				{
					direccion_lectura = 0;
				}
			}
		}

		//=========================================================
		// MODO 2: ADAFRUIT IO (CORREGIDO CON MEMORIA DE ESTADO)
		//=========================================================
		else if(modo_actual == MODO_ADAFRUIT)
		{
			uint8_t nuevo_dato = Leer_Dato_Serial_Local();

			// Guardamos en memoria solo si es un comando real (ignora ceros y basuras)
			if(nuevo_dato != 0 && nuevo_dato != '\n' && nuevo_dato != '\r')
			{
				dato_serial = nuevo_dato;
			}

			// Mantiene la acción basándose en el último comando guardado
			switch(dato_serial)
			{
				//=========================================
				// PUERTA ABRIR
				//=========================================
				case 'F':
					Set_Servo_Puerta(3000);
					PORTB |= (1 << PORTB3);
					break;

				//=========================================
				// PUERTA CERRAR
				//=========================================
				case 'B':
					Set_Servo_Puerta(1500);
					PORTB &= ~(1 << PORTB3);
					break;

				//=========================================
				// ADELANTE
				//=========================================
				case 'W':
					PORTD |=  (1 << PORTD3);
					PORTD &= ~(1 << PORTD4);
					Set_Motor_Velocidad(0, 180);
					break;

				//=========================================
				// REVERSA
				//=========================================
				case 'X':
					PORTD &= ~(1 << PORTD3);
					PORTD |=  (1 << PORTD4);
					Set_Motor_Velocidad(0, 180);
					break;

				//=========================================
				// STOP
				//=========================================
				case 'S':
					PORTD &= ~(1 << PORTD3);
					PORTD &= ~(1 << PORTD4);
					Set_Motor_Velocidad(0, 0);
					break;

				default:
					break;
			}
		}

		_delay_ms(10);
	}

	return 0;
}