#include "stm32f4xx.h"                  // Device header
#include <stdint.h>
#include <stdio.h>
#include "usart.h"
#include "board.h"
#include "sysTick.h"
#include "timer.h"


#define XTALFREQ 16000000UL


void run_LEDinPWM(uint32_t dutyCycle)
{
	// init PA5
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 	// clock to Port A
	GPIOA->MODER |= GPIO_MODER_MODE5_1; 	// set PA5 to alternate
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0; 	// set PA5 to AF1 => TIM2 channel 1
	// init Timer 2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 	// clock to TIM2
	TIM2->CCER |= TIM_CCER_CC1E; 			// enable Timer 2 channel 1
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_2 | TIM_CCMR1_OC1M_1); // OC1M = 110 => PWM mode 1
	// PWM mode 1: In upcounting, channel 1 is active as long as TIMx_CNT<TIMx_CCR1
	// else inactive. In downcounting, channel 1 is inactive (OC1REF=‘0) as long as
	// TIMx_CNT>TIMx_CCR1 else active (OC1REF=1)
	// timer channel configured as output by default
	TIM2->PSC = 0ul; 			// no prescaler
	TIM2->ARR = 99ul;			// reload value = 99
	TIM2->CCR1 = dutyCycle;		// write duty cycle
	TIM2->CNT = 0ul; 			// clear counter
	TIM2->CR1 |= TIM_CR1_CEN; 	// enable counter
}


// output compare at PA5, input capture of rising edge at PA6, print timestamp via USART2
void run_inputCaptureTest(void)
{
	usart2_init();
	uint32_t arrTicks = XTALFREQ;
	uint16_t pscTicks = 1ul;
	// set board led (PA5) to output compare mode
	// init Pin A5
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 	// clock to port A
	GPIOA->MODER |= GPIO_MODER_MODE5_1; 	// set PA5 to alternate function
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0; 	// set Pin A5 to alternate function 1 (TIM2 channel 1)
	// init timer 2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 	// clock to timer 2
	TIM2->CCR1 = 0ul; 						// set compare value of channel 1 to 0
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0); // set OC1M to 011 (toggle on match)
	TIM2->PSC = --pscTicks; 				// convert ticks to value in register (due to timer update at underflow)
	TIM2->ARR = --arrTicks;
	TIM2->CNT = 0ul;
	TIM2->CCER |= TIM_CCER_CC1E; 			// enable compare mode
	TIM2->CR1 |= TIM_CR1_CEN; 				// enable counter
	// board led (PA5) is controlled in output compare mode
	
	// init TIM3 to input capture with PA6
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN; 	// enable clock to TIM3
	GPIOA->MODER |= GPIO_MODER_MODE6_1; 	// set PA6 to alternate function
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL6_1; 	// set PA6 to alternate function 2 (TIM3 channel 1)
	TIM3->PSC = 16000ul-1ul; 				// set prescaler to 16000 ticks
	// now the value in CCR is corresponding to the time in ms
	TIM3->CCMR1 = (TIM_CCMR1_IC1F_0 | TIM_CCMR1_IC1F_1 // IC1F = 0011 => f_SAMPLING =f_CK_INT, N=8
	| TIM_CCMR1_CC1S_0); // CC1S = 01 => CC1 channel is configured as input, IC1 is mapped on TI1	
	// TIMx_CCER: CC1P and CC1NP low by default => input capture at rising edge
	TIM3->CCER |= TIM_CCER_CC1E; 			// enable input capture
	TIM3->CR1 |= TIM_CR1_CEN;

	uint32_t timeStamp = 0; // initialize timeStamp variable for storing the captured timestamps

	char printString[300];

	while (1)
	{
		if (TIM3->SR & TIM_SR_CC1IF) // if capture ocurred
		{	// print timestamp via USART2
			timeStamp = TIM3->CCR1;
			sprintf(printString, "\n%ld ms", timeStamp);
			usart2_printString(printString);
		}
	}
}

void run_controlLedWithTimer2CompareMode(uint16_t pscTicks, uint32_t arrTicks)
{
	// init Pin A5
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; 	// clock to port A
	GPIOA->MODER |= GPIO_MODER_MODE5_1; 	// set PA5 to alternate function
	GPIOA->AFR[0] |= GPIO_AFRL_AFSEL5_0; 	// set Pin A5 to alternate function 1 (TIM2 channel 1)
	// init timer 2
	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN; 	// clock to timer 2
	TIM2->CCR1 = 0ul; 						// set compare value of channel 1 to 0
	TIM2->CCMR1 |= (TIM_CCMR1_OC1M_1 | TIM_CCMR1_OC1M_0); // set OC1M to 011 (toggle on match)
	TIM2->PSC = --pscTicks; 				// convert ticks to value in register (due to timer update at underflow)
	TIM2->ARR = --arrTicks;
	TIM2->CNT = 0ul;
	TIM2->CCER |= TIM_CCER_CC1E; 			// enable compare mode
	TIM2->CR1 |= TIM_CR1_CEN; 				// enable counter
	while (1);
}


void run_controlBoardLEDWtihTimer2(uint16_t pscTicks, uint32_t arrTicks)
{
    boardLED_init();
	timer2_init(pscTicks, arrTicks);

	while(1)
	{
		if (TIM2->SR & TIM_SR_UIF) // if update interrupt flag is set, then toggle board LED
		{
			if (boardLED_getState())
			{
				boardLED_reset();
				TIM2->SR &= ~TIM_SR_UIF; // reset update interrupt flag
			}

			else
			{
				boardLED_set();
				TIM2->SR &= ~TIM_SR_UIF; // reset update interrupt flag
			}
		}
	}
}


void run_controlBoardLEDWithSysTickTimer(uint32_t ticks)
{
	boardLED_init();
	SysTick_ConfigWithoutInterrupt(ticks);
	
	int8_t set = 0;

	while(1)
	{
		if (SysTick->CTRL & SysTick_CTRL_COUNTFLAG_Msk) // if SysTick counted to 0
		{
			if (set==0)
			{
				boardLED_set();
				set = 1;
			}
			else
			{
				boardLED_reset();
				set = 0;
			}
		}
	}
}


void run_controlBoardLEDWithBoardButton(void)
{
	boardButton_init();
	boardLED_init();
	usart2_init();
	
	while(1)
	{
		board_lightLEDIfButtonIsPressed();
		usart2_writeString_oncePerBoardButtonPress("Hallo Robert :)\n");
	}
}


void run_controlBoardLEDWithUSART2(void)
{
	boardButton_init();
	boardLED_init();
	usart2_init();
	
	while(1)
	{
		if(USART2->SR & USART_SR_RXNE){ // if data is ready to be read (usart receive buffer not empty)
			char recChar = usart2_readChar();
			// set board led if 'h' is received and reset led if 'l' is received
			if(recChar == 'h')
				boardLED_set();
			else if(recChar == 'l')
				boardLED_reset();
		}
	}
}
