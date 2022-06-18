#if !defined(__BOARD_H)
#define __BOARD_H

#include <stdint.h>

void boardButton_init(void);
int32_t boardButton_pressed(void);	
void boardLED_init(void);
void boardLED_set(void);
void boardLED_reset(void);
int32_t boardLED_getState(void);
void board_lightLEDIfButtonIsPressed(void);

#endif // __BOARD_H

