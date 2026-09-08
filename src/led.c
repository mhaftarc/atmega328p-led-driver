#include "led.h"
#include <avr/io.h>

volatile State_t currentstate = MODE_OFF;

void led_init(void)
{
    // pull up rezystory
//  PORTD |= (1 << PD2);
// PORTD |= (1 << PD3);
PORTC |= ((1 << PC0) | (1 << PC1));
DDRB |= (1 << PB1);
};


void update_led_state(void){
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
            }
    };
}