#include <unity.h>
#include "button_logic.h"



volatile State_t currentstate = MODE_OFF;

void uart_transmit(const char *data) { (void)data; }  
bool uart_read_byte(uint8_t *c) { (void)c; return false; }

void setUp(void) {} 
void tearDown(void) {}



void command_changing(void){
    TEST_ASSERT_EQUAL(MODE_ON,button_cycle(MODE_OFF));
    TEST_ASSERT_EQUAL(MODE_SLOW,button_cycle(MODE_ON));
    TEST_ASSERT_EQUAL(MODE_STROBE,button_cycle(MODE_SLOW));
    TEST_ASSERT_EQUAL(MODE_BREATHING,button_cycle(MODE_STROBE));
    TEST_ASSERT_EQUAL(MODE_OFF,button_cycle(MODE_BREATHING));


}



int main(void){
    UNITY_BEGIN();
    RUN_TEST(command_changing);
    return UNITY_END();
}