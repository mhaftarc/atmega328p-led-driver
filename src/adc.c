#include "adc.h"
#include <avr/io.h>


void adc_init(void){
    
    ADMUX |= ((1 << REFS0) | (1 << MUX1)); // Select voltage reference and ADC input
    ADCSRA |= ((1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2)); // ADC turning on and prescaler
}

uint16_t adc_measure(void){
    ADCSRA |= (1 << ADSC); // measure start
    while(ADCSRA & (1 << ADSC));
    return ADC;
}