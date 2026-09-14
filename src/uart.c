#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#define BUFFER_SIZE 64


char ringBuffer[BUFFER_SIZE]; 
volatile uint8_t writeIndex = 0;
volatile uint8_t readIndex = 0;




void uart_init(void){
    UCSR0B |= ((1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0)); // tx rx enable
    UBRR0 = 103; // baud rate
    UCSR0C |= ((1 << UCSZ01) | (1 << UCSZ00)); // 8n1 format

}


void uart_transmit(const char *data){

    while(*data != '\0') {  // sending each string sign by sign 
        while(!(UCSR0A & (1 << UDRE0))) {
        }

        UDR0 = *data;
        data++;
    }
}


bool uart_read_byte(uint8_t *c){    //output as param, the function returns 1 or 0, if it see's the bit, and stores the message under the pointer adress as value

    if (readIndex != writeIndex) {
        *c = ringBuffer[readIndex];
        readIndex = (readIndex + 1) % BUFFER_SIZE;
        return true;
    } else {
        return false;
    }
}


ISR(USART_RX_vect){
    uint8_t c = UDR0;
    uint8_t next = (writeIndex + 1) % BUFFER_SIZE;

if (next != readIndex) {
    ringBuffer[writeIndex] = c;
    writeIndex = next;
}  
}