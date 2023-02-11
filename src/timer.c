#include "stm32f4xx.h"                  // Device header
#include <stdint.h>

// pscTicks is the number of prescaler ticks, arrTicks the number of auto reload register ticks
// the resulting timer period tim_per with a clock frequency of f_clk is equal to: 
// tim_per = pscTicks * arrTicks / f_clk
void timer2_init(uint16_t pscTicks, uint32_t arrTicks)
{
    RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; // enable clock to timer 2
    TIM2->PSC = --pscTicks; // write prescaler register (decrement due to update event after underflow)
    TIM2->ARR = --arrTicks; // write auto reload register
    TIM2->CNT = 0ul; // clear counter
    TIM2->CR1 |= TIM_CR1_CEN; // enable counter (start TIM2)
}
