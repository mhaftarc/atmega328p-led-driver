#ifndef LED_H
#define LED_H
#include <stdint.h>


typedef enum {
    MODE_OFF,
    MODE_ON,
    MODE_SLOW,
    MODE_STROBE,
    MODE_BREATHING
} State_t;

extern volatile State_t currentstate;


void update_led_state(void);
void led_init(void);

#endif