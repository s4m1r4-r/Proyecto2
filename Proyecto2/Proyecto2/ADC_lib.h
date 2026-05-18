/*
 * ADC_lib.h
 *
 * Created: 25/04/2026 18:49:51
 *  Author: WSami
 */ 


#ifndef ADC_LIB_H_
#define ADC_LIB_H_

#include <avr/io.h>
#include <stdint.h>

void init_ADC(void);
uint16_t read_ADC(uint8_t channel);

#endif