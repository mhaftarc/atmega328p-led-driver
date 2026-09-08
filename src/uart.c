#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>

static volatile uint8_t overflow = 0;
static volatile uint8_t index = 0;
volatile uint8_t message_ready = 0;
char buffer[20];



void uart_init(void){
    UCSR0B |= ((1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0)); // tx rx enable
    UBRR0 = 103; // baud rate
    UCSR0C |= ((1 << UCSZ01) | (1 << UCSZ00)); // 8n1 format

};


void uart_transmit(char *data){

    while(*data != '\0') {  // sending each string sign by sign 
        while(!(UCSR0A & (1 << UDRE0))) {
        }

        UDR0 = *data;
        data++;
    }
};

ISR(USART_RX_vect){

    if(overflow == 1){
        if(UDR0 =='\n'){
            overflow = 0;
            index = 0;
        }
    }else{
        if(index >= 19){
            overflow = 1;
        }else{
           buffer[index] = UDR0;

        if(buffer[index] == '\n'){
            message_ready = 1;
            buffer[index] = '\0';
            index = 0;
        }else{
        index++;
        }
        }
}
};