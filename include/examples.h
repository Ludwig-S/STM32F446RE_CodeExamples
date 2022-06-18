#if !defined(__EXAMPLES_H)
#define __EXAMPLES_H

#include <stdint.h>

void run_controlBoardLEDWithSysTickTimer(uint32_t ticks);
void run_controlBoardLEDWtihTimer2(uint16_t pscTicks, uint32_t arrTicks);
void run_controlBoardLEDWithSysTickTimer(uint32_t ticks);
void run_controlBoardLEDWithBoardButton(void);
void run_controlBoardLEDWithUSART2(void);
void run_controlLedWithTimer2CompareMode(uint16_t pscTicks, uint32_t arrTicks);
void run_inputCaptureTest(void);
void run_LEDinPWM(uint32_t dutyCycle);

#endif // __EXAMPLES_H


