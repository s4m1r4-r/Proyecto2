/*
 * PWM_lib.h
 *
 * Created: 25/04/2026 18:52:29
 *  Author: WSami
 */ 


#ifndef PWM_LIB_H_
#define PWM_LIB_H_

#include <avr/io.h>

void init_PWM_Servos(void);
void set_servo_pos(uint8_t servo_num, uint16_t angle);

#endif