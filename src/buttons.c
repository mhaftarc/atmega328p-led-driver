#include "buttons.h"
#include "timer.h"
#include "led.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include "button_logic.h"



void buttons_init(void)
{
DDRC &= ~((1 << PC0) | (1 << PC1));
PORTC |= ((1 << PC0) | (1 << PC1));

// PCINT not used here , nevertheless  debouncing handled by polling method
// left here as reference in case interrupt wake-up is needed later
// PCICR |= (1 << PCIE1);
// PCMSK1 |= ((1 << PCINT9) | (1 << PCINT8));
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
   /*static uint8_t lastPinState = 0xFF;
    uint8_t currentPinState = PINC;
    
    if(!(currentPinState & (1 << PC0)) && (lastPinState & (1 << PC0))){
         if(debounceTimer == 0){
        debounceTimer = 30;
            currentstate = button_cycle(currentstate);
            }
    };
    if(!(currentPinState & (1 << PC1)) && (lastPinState & (1 << PC1))){
         if(debounceTimer == 0){
        debounceTimer = 30;
        currentstate = reset_button(currentstate);
    }

    }
     lastPinState = currentPinState;*/
    } 



void buttons_tick(void){

    static uint32_t lastTick = 0;
    static uint8_t lastRaw = 0xFF;
    static uint8_t stableState = 0xFF;
    static uint8_t stableCount = 0;
    static uint8_t previousStable = 0xFF;


    uint32_t now = get_systemMillis();
    if (now - lastTick < 1) {
        return;
    }
    lastTick = now;

    uint8_t currentRaw = PINC;

    if(currentRaw == lastRaw){
        if(stableCount < 255){
        stableCount++;
        }
    }else{
        stableCount = 0;
    }

    if(stableCount > 5){
        previousStable = stableState;
        stableState = currentRaw;
        if(!(stableState & (1 << PC0)) && (previousStable & (1 << PC0))){
            currentstate = button_cycle(currentstate);
        };
        if(!(stableState & (1 << PC1)) && (previousStable & (1 << PC1))){
            currentstate = reset_button(currentstate);
   }
    }

    lastRaw = currentRaw;

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

   

