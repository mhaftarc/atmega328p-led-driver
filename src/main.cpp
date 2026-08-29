#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

 typedef enum {
    MODE_OFF,
    MODE_ON,
    MODE_SLOW,
    MODE_STROBE
} State_t;

volatile State_t currentstate = MODE_OFF;

ISR(INT0_vect){

    if(!(PINB & (1 << PB0))){
            _delay_ms(30);
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
                currentstate = MODE_OFF;
                break;
            }
    };
    
}






int main(void) {

   


    DDRB &= ~(1 << PB0);
    DDRB |= (1 << PB5);
    PORTB |= (1 << PB0);
    sei();

    while (1) {
         
            switch(currentstate){
                case MODE_OFF:
                PORTB &= ~(1 << PB5);
                break;
                case MODE_ON:
                PORTB |= (1 << PB5);
                break;
                case MODE_SLOW:
                PORTB ^= (1 << PB5);
                _delay_ms(500);
                break;
                case MODE_STROBE:
                PORTB ^= (1 << PB5);
                _delay_ms(50);
                break;
    }

} }

