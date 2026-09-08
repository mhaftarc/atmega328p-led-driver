#ifndef UART_H
#define UART_H
#include <stdint.h>


extern char buffer[20];
extern volatile uint8_t message_ready;

void uart_init(void);
void uart_transmit(char *data);


#endif