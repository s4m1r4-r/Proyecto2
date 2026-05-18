/*
 * TIMER1_lib.c
 *
 * Created: 25/04/2026 18:55:52
 *  Author: WSami
 */ 


#include "Timer1_lib.h"

void init_Timer1_Servo(void) {
    // Configurar pines PB1 (OC1A) y PB2 (OC1B) como salidas
    DDRB |= (1 << PB1) | (1 << PB2);

    // Configuración de Timer 1:

    
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);

    //CALCULO TOP
  //  ICR1 = 39999;

    // Inicializar servos 
  //  OCR1A = 3000;
  //  OCR1B = 3000;
}

void set_PWM_Timer1A(uint16_t duty) {
    OCR1A = duty;
}

void set_PWM_Timer1B(uint16_t duty) {
    OCR1B = duty;
}