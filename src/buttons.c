#include "buttons.h"
#include "timer.h"
#include "led.h"
#include <avr/io.h>
#include <avr/interrupt.h>



void buttons_init(void)
{
DDRC &= ~((1 << PC0) | (1 << PC1));
PORTC |= ((1 << PC0) | (1 << PC1));
PCICR |= (1 << PCIE1);
PCMSK1 |= ((1 << PCINT9) | (1 << PCINT8));
}

// interupts masks
// EICRA |= (1 << ISC11);     // masks for int 0 and int1
// EICRA |= (1 << ISC01);
// EIMSK |= (1 << INT0);
// EIMSK |= (1 << INT1);
// inputs and outputs
// DDRD &= ~(1 << PD3); 
// DDRD &= ~(1 << PD2);


// INT0/INT1 originally used here.
// Arduino pins 1-7 were broken, so buttons were moved
// to PC0/PC1 and pin-change interrupts (PCINT1) are used.


ISR(PCINT1_vect){ 
    static uint8_t lastPinState = 0xFF;
    uint8_t currentPinState = PINC;
    if(!(currentPinState & (1 << PC0)) && (lastPinState & (1 << PC0))){
         if(debounceTimer == 0){
        debounceTimer = 30;
            switch(currentstate){
                case MODE_OFF:
                currentstate = MODE_ON;
                break;
                case MODE_ON:
                currentstate = MODE_SLOW;
                break;
                case MODE_SLOW:
                currentstate = MODE_STROBE;
                break;
                case MODE_STROBE:
                currentstate = MODE_BREATHING;
                break;  
                case MODE_BREATHING:
                currentstate = MODE_OFF;
                break;
            }
    }};
    if(!(currentPinState & (1 << PC1)) && (lastPinState & (1 << PC1))){
         if(debounceTimer == 0){
        debounceTimer = 30;
        currentstate = MODE_OFF;
    }

    }
     lastPinState = currentPinState;
    } 
/*ISR(INT0_vect){
`
    if(debounceTimer == 0){
        debounceTimer = 30;
            switch(currentstate){
                case MODE_OFF:
                currentstate = MODE_ON;
                break;
                case MODE_ON:
                currentstate = MODE_SLOW;
                break;
                case MODE_SLOW:
                currentstate = MODE_STROBE;
                break;
                case MODE_STROBE:
                currentstate = MODE_BREATHING;
                break;  
                case MODE_BREATHING:
                currentstate = MODE_OFF;
                break;
            }
    };
    
}
ISR(INT1_vect){
    
    if(debounceTimer == 0){
        debounceTimer = 30;
        currentstate = MODE_OFF;
    }

}*/

   

