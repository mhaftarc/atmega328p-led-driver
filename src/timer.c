#include "timer.h"
#include <avr/io.h>
#include <avr/interrupt.h>


volatile uint32_t systemMillis = 0;
volatile uint8_t debounceTimer = 0;




void timer0_init(void){
    TCCR0A |= (1 << WGM01);         
    TCCR0B |= (1 << CS01) | (1 << CS00);
    OCR0A = 249;
    TIMSK0 |= (1 << OCIE0A);
};


void timer1_pwm(void){
    TCCR1A |= ((1 << COM1A1) | (1 << WGM11) | (1 << WGM10));
    TCCR1B |= ((1 << WGM12) | (1 << CS11) | (1 << CS10));
};

ISR(TIMER0_COMPA_vect) {
    systemMillis++;

    if (debounceTimer > 0) {
        debounceTimer--;
    }
};

