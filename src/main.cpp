#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>

 typedef enum {
    MODE_OFF,
    MODE_ON,
    MODE_SLOW,
    MODE_STROBE,
    MODE_BREATHING
} State_t;

volatile State_t currentstate = MODE_OFF;
volatile uint8_t debounceTimer = 0;
volatile uint32_t systemMillis = 0;
char buffer[20];
volatile uint8_t index, message_ready = 0;


void timer0_init(void){
    TCCR0A |= (1 << WGM01);         
    TCCR0B |= (1 << CS01) | (1 << CS00);
    OCR0A = 249;
    TIMSK0 |= (1 << OCIE0A);
}

void timer1_pwm(void){
    TCCR1A |= ((1 << COM1A1) | (1 << WGM11) | (1 << WGM10));
    TCCR1B |= ((1 << WGM12) | (1 << CS11) | (1 << CS10));
}

void adc_init(void){
    ADMUX |= ((1 << REFS0) | (1 << MUX1)); // the voltage reference and pin choose
    ADCSRA |= ((1 << ADEN) | (1 << ADPS0) | (1 << ADPS1) | (1 << ADPS2)); // ADC turning on and prescaler
}

void uart_init(void){
    UCSR0B |= ((1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0)); // tx rx enable
    UBRR0 = 103; // baud rate
    UCSR0C |= ((1 << UCSZ01) | (1 << UCSZ00)); // 8n1 format

}

void uart_transmit(char data){
    while(!(UCSR0A & (1 << UDRE0))){
    }
    UDR0 = data;
}




uint16_t adc_measure(void){
    ADCSRA |= (1 << ADSC); // measure start
    while(ADCSRA & (1 << ADSC));
    return ADC;
}

void handle_command(char *command){ // pointer on the first element of string since the adress doesnt change
    if(strcmp(command, "ON") == 0){
        currentstate = MODE_ON;
    }
    else if(strcmp(command, "OFF") == 0){
        currentstate = MODE_OFF;
    }
    else if(strcmp(command, "BREATHING") == 0){
        currentstate = MODE_BREATHING;
    }
    else if(strcmp(command, "STROBE") == 0){
        currentstate = MODE_STROBE;
    }
    else if(strcmp(command, "SLOW") == 0){
        currentstate = MODE_SLOW;
    }
}

/*ISR(INT0_vect){

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

   
ISR(TIMER0_COMPA_vect) {
    systemMillis++;

    if (debounceTimer > 0) {
        debounceTimer--;
    }
}

ISR(USART_RX_vect){
    
    buffer[index] = UDR0;
    if(buffer[index] == '\n'){
        message_ready = 1;
        buffer[index] = '\0';
        index = 0;
    }else{
    index++;
    }

}


int main(void) {

    //wejscia i wyjscia
  //  DDRD &= ~(1 << PD3); 
   // DDRD &= ~(1 << PD2);
    DDRB |= (1 << PB1);
    DDRC &= ~((1 << PC0) | (1 << PC1));
    

    // pull up rezystory
  //  PORTD |= (1 << PD2);
   // PORTD |= (1 << PD3);
   PORTC |= ((1 << PC0) | (1 << PC1));

    //interupts maski
  //  EICRA |= (1 << ISC11);     //maski dla int0 i int1, ale mam wyłamane piny 1-7 na arduino!
   // EICRA |= (1 << ISC01);
   // EIMSK |= (1 << INT0);
   // EIMSK |= (1 << INT1);
    PCICR |= (1 << PCIE1);
    PCMSK1 |= ((1 << PCINT9) | (1 << PCINT8));

    adc_init();
    timer0_init();
    timer1_pwm();
    uart_init();
    sei();
    

    while (1) {
         
            switch(currentstate){
                case MODE_OFF:
                OCR1A = 0;
                break;

                case MODE_ON:
                OCR1A = adc_measure();
                break;

                case MODE_SLOW:{
                static uint32_t lastSwitch = 0;  // used static instead od global variable to prevent messing up variables, static remembers the state in the loop here
                static uint8_t ledState = 0;

                if(systemMillis - lastSwitch >=500){
                    lastSwitch = systemMillis;
                    ledState = !ledState;

                    OCR1A = ledState ? adc_measure() : 0;
                }
                break;}

                case MODE_STROBE:{
                static uint32_t lastSwitch = 0;  
                static uint8_t ledState = 0;

                if(systemMillis - lastSwitch >=50){
                    lastSwitch = systemMillis;
                    ledState = !ledState;

                    OCR1A = ledState ? adc_measure() : 0;
                }} break;

                case MODE_BREATHING:{
                static uint16_t brightness = 0;
                static uint32_t lastUpdate = 0;
                static uint8_t direction = 0;
                if(systemMillis - lastUpdate >= 10){
                    lastUpdate = systemMillis;
                    if(direction == 0){
                        if(brightness >= 1018){
                            brightness = 1023;
                            direction = 1;
                        }else {
                            brightness += 5;
                        }
                       
                    }else {
                        if(brightness <= 5){
                            brightness = 0;
                            direction = 0;
                        }
                        else{
                            brightness -= 5;
                        }
                    }
                    OCR1A = brightness;
                }    
                }    break;
    }

    if(message_ready == 1){
    handle_command(buffer);
    message_ready = 0;
}

}








}