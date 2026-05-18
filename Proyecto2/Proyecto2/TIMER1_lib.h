/*
 * TIMER1_lib.h
 *
 * Created: 25/04/2026 18:57:29
 *  Author: WSami
 */ 


#ifndef TIMER1_LIB_H_
#define TIMER1_LIB_H_

#include <avr/io.h>
#include <stdint.h>

// Configura el Timer 1 para Fast PWM 
void init_Timer1_Servo(void);

//Actualización de servos
void set_PWM_Timer1A(uint16_t duty);
void set_PWM_Timer1B(uint16_t duty);

#endif