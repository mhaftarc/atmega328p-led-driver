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
volatile uint8_t debounceTimer = 0;


void timer0_init(void){
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS01) | (1 << CS00);
    OCR0A = 249;
    TIMSK0 |= (1 << OCIE0A);
};

ISR(INT0_vect){

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

};
ISR(TIMER0_COMPA_vect) {
    if (debounceTimer > 0) {
        debounceTimer--;
    }
}







int main(void) {

    //wejscia i wyjscia
    DDRD &= ~(1 << PD3); 
    DDRD &= ~(1 << PD2);
    DDRB |= (1 << PB5);

    // pull up rezystory
    PORTD |= (1 << PD2);
    PORTD |= (1 << PD3);

    //interupts maski
    EICRA |= (1 << ISC11);
    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);
    EIMSK |= (1 << INT1);
    
    timer0_init();
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

