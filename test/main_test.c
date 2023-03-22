#include "stm32f4xx.h"
#include <unity.h>

void test_DummyTest(void) {
  TEST_ASSERT(1==1);
}

uint32_t SysTick_waitTicks(uint32_t ticks)
{
  if ((ticks - 1UL) > SysTick_LOAD_RELOAD_Msk)
  {
    return (1UL);                                                   /* Reload value impossible */
  }
  SysTick->LOAD  = (uint32_t)(ticks - 1UL);                         /* set reload register */
  SysTick->VAL   = 0UL;                                             /* Load the SysTick Counter Value */
  SysTick->CTRL  = SysTick_CTRL_CLKSOURCE_Msk |
                   SysTick_CTRL_ENABLE_Msk;                         /* Enable SysTick Timer */
  while ((SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk));            /* wait until timer finished */
  return (0UL);                                                     /* Function successful */
}

void waitSeconds(int seconds) {
  // 180 MHz
  // 1 / 180e6 => 180e6 = 1 second
  SysTick_waitTicks((int)(seconds*180e6));
}

void setUp(void) {
}

void tearDown(void) {
}

int main(int argc, char **argv) {
  waitSeconds(2);
  UNITY_BEGIN();
  RUN_TEST(test_DummyTest);
  UNITY_END();

  while(1){}
}