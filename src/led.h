#ifndef LED_H
#define LED_H
#include <stdint.h>

void led_init(void);


typedef enum {
    MODE_OFF,
    MODE_ON,
    MODE_SLOW,
    MODE_STROBE,
    MODE_BREATHING
} State_t;

extern volatile State_t currentstate;
extern volatile uint32_t systemMillis;

void update_led_state(void);
uint16_t adc_measure(void);
#endif