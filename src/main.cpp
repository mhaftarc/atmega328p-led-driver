#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

 typedef enum {
    MODE_OFF,
    MODE_ON,
    MODE_SLOW,
    MODE_STROBE,
    MODE_BREATHING
} State_t;

volatile State_t currentstate = MODE_OFF;
volatile uint8_t debounceTimer = 0;
volatile uint16_t ledTimer = 0;


void timer0_init(void){
    TCCR0A |= (1 << WGM01);
    TCCR0B |= (1 << CS01) | (1 << CS00);
    OCR0A = 249;
    TIMSK0 |= (1 << OCIE0A);
};

void timer1_init(void) {
    TCCR1A |= (1 << COM1A1) | (1 << WGM10);
    TCCR1B |= (1 << WGM12) | (1 << CS11) | (1 << CS10);
    OCR1A = 0; 
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
                currentstate = MODE_BREATHING;
                break; 
                case MODE_BREATHING:
                currentstate = MODE_OFF;
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
    if(ledTimer > 0){
        ledTimer--;
    }
}







int main(void) {

    //wejscia i wyjscia
    DDRD &= ~(1 << PD3); 
    DDRD &= ~(1 << PD2);
    DDRB |= (1 << PB1);

    // pull up rezystory
    PORTD |= (1 << PD2);
    PORTD |= (1 << PD3);

    //interupts maski
    EICRA |= (1 << ISC11);
    EICRA |= (1 << ISC01);
    EIMSK |= (1 << INT0);
    EIMSK |= (1 << INT1);
    
    timer0_init();
    timer1_init();
    sei();
    
    uint8_t brightness = 0;
    int8_t fadeAmount = 5;

    while (1) {
         
            switch(currentstate){
                case MODE_OFF:
                PORTB &= ~(1 << PB1);
                break;
                case MODE_ON:
                PORTB |= (1 << PB1);
                break;
                case MODE_SLOW:
                    if(ledTimer == 0){
                        PORTB ^= (1 << PB1);
                        ledTimer = 500;
                    }
                break;
                case MODE_STROBE:
                    if(ledTimer == 0){
                        PORTB ^= (1 << PB1);
                        ledTimer = 50;
                    }
                break;
                case MODE_BREATHING:
                if (ledTimer == 0) {
                    brightness += fadeAmount;
                    OCR1A = brightness;

                    // Zmiana kierunku po osiągnięciu krańców zakresu
                    if (brightness == 0 || brightness == 255) {
                        fadeAmount = -fadeAmount;
                    }
                    ledTimer = 15;       // Krok płynności co 15 ms
                }
                break;
    }

} }

