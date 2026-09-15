#include "button_logic.h"



State_t button_cycle(State_t current){
    switch(current){
                case MODE_OFF:
                return MODE_ON;

                case MODE_ON:
                return MODE_SLOW;

                case MODE_SLOW:
                return MODE_STROBE;

                case MODE_STROBE:
                return MODE_BREATHING;

                case MODE_BREATHING:
                return MODE_OFF;
                
                default: 
                return MODE_OFF;

}
}

State_t reset_button(State_t current){
    (void) current;
    return MODE_OFF;
} 
