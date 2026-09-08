#ifndef WATCHDOG_H
#define WATCHDOG_H
#include <stdint.h>


void watchdog_init(void);
void check_reset_cause(void);

#endif