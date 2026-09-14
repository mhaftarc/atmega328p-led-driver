#ifndef UART_H
#define UART_H
#include <stdint.h>
#include <stdbool.h>



extern char commandBuffer[20];

void uart_init(void);
void uart_transmit(const char *data);
bool uart_read_byte(uint8_t *c);

#endif