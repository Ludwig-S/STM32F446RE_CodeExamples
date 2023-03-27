#include "stm32f4xx.h"
#include "board.h"
#include "sysTick.h"

#include <unity.h>

// tests:
void test_boardLED_set(void) {
  boardLED_set();
  TEST_ASSERT_EQUAL_UINT32(1, (GPIOA->ODR & GPIO_ODR_OD5) >> GPIO_ODR_OD5_Pos);
}

void test_boardLED_reset(void) {
  boardLED_reset();
  TEST_ASSERT_EQUAL_UINT32(0, (GPIOA->ODR & GPIO_ODR_OD5));
}

void test_dummyTest_assertion_false(void){
  TEST_ASSERT_EQUAL_UINT32(1, 0);
}

void test_dummyTest_message_false(void) {
  TEST_ASSERT_MESSAGE( 1 == 0 , "1 is not equal to 0 :'(");
}


// unity functions:
void setUp(void) {
    boardLED_init();
}

void tearDown(void) {
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_boardLED_set);
  RUN_TEST(test_boardLED_reset);
  RUN_TEST(test_dummyTest_assertion_false); // this test will fail
  RUN_TEST(test_dummyTest_message_false); // this test will fail
  UNITY_END();
}
