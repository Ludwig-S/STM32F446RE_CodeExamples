#if !defined(__SYSTICK_H)
#define __SYSTICK_H

uint32_t SysTick_ConfigWithoutInterrupt(uint32_t ticks);
void waitSeconds(int seconds);

#endif // __SYSTICK_H
