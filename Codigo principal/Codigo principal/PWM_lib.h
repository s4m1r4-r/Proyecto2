/*
 * PWM_lib.h
 *
 * Created: 05/05/2026 6:59:01
 *  Author: WSami
 */ 


#ifndef PWM_LIB_H_
#define PWM_LIB_H_

#include <avr/io.h>

void PWM_Init(void);
void Set_Motor_Velocidad(uint8_t izq, uint8_t der);
void Set_Servo_Direccion(uint16_t pulso);
void Set_Servo_Puerta(uint16_t pulso);

#endif 