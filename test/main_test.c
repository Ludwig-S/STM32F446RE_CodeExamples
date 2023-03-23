#include "stm32f4xx.h"
#include "board.h"
#include "sysTick.h"

#include <unity.h>

void test_DummyTest(void) {
  TEST_ASSERT_MESSAGE( 1 == 1 , "1 == 1 :)");
}

void test_boardLED_set(void) {
  boardLED_init();
  boardLED_set();
  waitSeconds(1);
  TEST_ASSERT_MESSAGE( boardLED_getState() == 1 , "test_boardLED_set failed");
}

void setUp(void) {
}

void tearDown(void) {
}

int main(int argc, char **argv) {
  UNITY_BEGIN();
  RUN_TEST(test_DummyTest);
  RUN_TEST(test_boardLED_set);
  UNITY_END();
}
