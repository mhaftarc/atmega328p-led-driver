#include "commands.h"
#include "led.h"
#include <string.h>


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
};
