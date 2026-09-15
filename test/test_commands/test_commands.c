#include <unity.h>
#include <stdbool.h>
#include <stdint.h>
#include "commands.h"
#include "led.h"

volatile State_t currentstate = MODE_OFF;

void uart_transmit(const char *data) { (void)data; }  // stubs, since during testing we dont have uart on computer
bool uart_read_byte(uint8_t *c) { (void)c; return false; }

void setUp(void) {}  // unity must have functions
void tearDown(void) {}

void valid_command(void) {
    currentstate = MODE_OFF;
    handle_command("ON");
    TEST_ASSERT_EQUAL(MODE_ON, currentstate);
}

void false_command(void) {
    currentstate = MODE_STROBE;
    handle_command("BLABLA");
    TEST_ASSERT_EQUAL(MODE_STROBE, currentstate);
}


int main(void) {
    UNITY_BEGIN();
    RUN_TEST(valid_command);
    RUN_TEST(false_command);
    return UNITY_END();
}