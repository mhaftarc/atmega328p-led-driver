#include "buttons.h"
#include "timer.h"
#include "led.h"
#include <avr/io.h>
#include <avr/interrupt.h>



void buttons_init(void)
{
    //wejscia i wyjscia
//  DDRD &= ~(1 << PD3); 
// DDRD &= ~(1 << PD2);
DDRC &= ~((1 << PC0) | (1 << PC1));

//interupts maski
//  EICRA |= (1 << ISC11);     //maski dla int0 i int1, ale mam wyłamane piny 1-7 na arduino!
// EICRA |= (1 << ISC01);
// EIMSK |= (1 << INT0);
// EIMSK |= (1 << INT1);
PCICR |= (1 << PCIE1);
PCMSK1 |= ((1 << PCINT9) | (1 << PCINT8));
};



ISR(PCINT1_vect){  // ta sama logika tylko z rozpoznaniem przyciskow
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
    
};
ISR(INT1_vect){
    
    if(debounceTimer == 0){
        debounceTimer = 30;
        currentstate = MODE_OFF;
    }

};*/

   

