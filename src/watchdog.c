#include "watchdog.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <uart.h>

void watchdog_init(void){
    WDTCSR |= ((1 << WDE) | (1 << WDCE)); // watchdog enable
    WDTCSR = ((1 << WDP2) | (1 << WDP1) | (1 << WDE)); // prescaler set to 1s 

};


void check_reset_cause(void){
    uint8_t resetCause = MCUSR;
    MCUSR = 0;

    if(resetCause & (1 << WDRF)){
        uart_transmit("watchdog reset\n");   
    }
    else if(resetCause & (1 << BORF)){
        uart_transmit("brown out reset\n");   
    }
    else if(resetCause & (1 << EXTRF)){
        uart_transmit("external reset\n");   
    }
    else if(resetCause & (1 << PORF)){
        uart_transmit("power on reset\n");   
    }

};