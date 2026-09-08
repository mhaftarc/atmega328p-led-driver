#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <avr/wdt.h>
#include "led.h"
#include "adc.h"
#include "uart.h"
#include "timer.h"
#include "watchdog.h"
#include "buttons.h"
#include "commands.h"


int main(void) {

uart_init();
check_reset_cause();
watchdog_init();
adc_init();
timer0_init();
timer1_pwm();
sei();

    
while (1) {
    wdt_reset();
    update_led_state();

    if(message_ready == 1){
        handle_command(buffer);
        message_ready = 0;
    }
}

}








