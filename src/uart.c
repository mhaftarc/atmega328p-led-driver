#include "uart.h"
#include <avr/io.h>
#include <avr/interrupt.h>
#define BUFFER_SIZE = 64


char ringBuffer[BUFFER_SIZE]; 
volatile uint8_t writeIndex = 0;
volatile uint8_t readIndex = 0;



void uart_init(void){
    UCSR0B |= ((1 << RXEN0) | (1 << RXCIE0) | (1 << TXEN0)); // tx rx enable
    UBRR0 = 103; // baud rate
    UCSR0C |= ((1 << UCSZ01) | (1 << UCSZ00)); // 8n1 format

}


void uart_transmit(char *data){

    while(*data != '\0') {  // sending each string sign by sign 
        while(!(UCSR0A & (1 << UDRE0))) {
        }

        UDR0 = *data;
        data++;
    }
}


uint8_t uart_read_byte(){    // i took taking output as param into consideration, but since only main and usr uses it, it isnt neccessary and it is more readable

    if (readIndex != writeIndex) {
        uint8_t c = ringBuffer[readIndex];
        readIndex = (readIndex + 1) % BUFFER_SIZE;
        return c;
    }
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


