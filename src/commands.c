#include "commands.h"
#include "led.h"
#include <string.h>
#include "uart.h"






void handle_command(const char *command){ // pointer on the first element of string since the adress doesnt change
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



void command_process(void){
    uint8_t c;
    static char commandBuffer[20];
    static uint8_t i = 0;

    if(uart_read_byte(&c)){
        if(c == '\n'){
            commandBuffer[i] = '\0';
            handle_command(commandBuffer);
            i = 0;
        }else{
            commandBuffer[i] = c;
            i++;
        }
    }
}
