#include "stm32f4xx.h"                  // Device header
#include <stdint.h>

#include "board.h"


void boardLED_init(void)
{
	// PA5 (LED):
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock to port A
	GPIOA->MODER |= GPIO_MODER_MODE5_0; // set PA5 to general purpose output
	// output push-pull by default
	// output slow by default
}
void boardLED_set(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BS5;
}

void boardLED_reset(void)
{
	GPIOA->BSRR |= GPIO_BSRR_BR5;
}

uint32_t boardLED_getState(void)
{
	return (GPIOA->ODR & GPIO_ODR_OD5) >> GPIO_ODR_OD5_Pos; // return output state of PA5
}

void boardLED_toggle(void)
{
	if (boardLED_getState())
		boardLED_reset();
	else
		boardLED_set();
}

// board button:
void boardButton_init(void)
{
	// PC13 (button):
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN; // enable clock to port C
	// input by default
	// external pull up resistor already on NUCLEO board => output no pull up and down (reset state)
}


uint32_t boardButton_pressed(void)
{
	// not pressed = high; pressed = low
	return !(GPIOC->IDR & GPIO_IDR_ID13);
}


void board_lightLEDIfButtonIsPressed(void)
{
	if(!boardButton_pressed()) // if button is not pressed
		boardLED_reset(); // LED off
	else // if button pressed (low state)
		boardLED_set(); // LED on
}
