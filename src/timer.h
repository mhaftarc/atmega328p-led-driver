#ifndef TIMER_H
#define TIMER_h
#include <stdint.h>



extern volatile uint32_t systemMillis;
extern volatile uint8_t debounceTimer;

void timer0_init(void);
void timer1_pwm(void);


#endif