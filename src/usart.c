#include "stm32f4xx.h"                  // Device header
#include <stdint.h>
#include <string.h>

#include "usart.h"
#include "board.h"

/*
Pin overview:

uC pin:		board pin name / function:


PA2			USART2_TX
PA3			USART2_RX

PA9			D8, trigger input

PA8			D7, output 1
PB10		D6, output 2
PB4			D5, output 3
PB5			D4, output 4
PB3			D3, output 5

*/

// USART2 functions:
// initialize usart2 RX and TX
void usart2_init(void)
{
	// USART2_RX uses port A2 and USART2_TX uses port A3 if each are set to alternate function 7 (p.59 datasheet)
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;	// enable clock to USART2
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock to Port A
	GPIOA->MODER |= (GPIO_MODER_MODE2_1|GPIO_MODER_MODE3_1); // set PA2 and PA3 to alternate function
	GPIOA->AFR[0] |= 
		(GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_0 | GPIO_AFRL_AFRL3_2 
		| GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_0); // set PA2 to AF7 (USART2_TX) and PA3 to AF7 (USART2_RX)
	USART2->CR1 |= (USART_CR1_TE|USART_CR1_RE);// enable transmitter and receiver
	// Baud rate Infos (p. 808 reference manual)
	USART2->BRR = 0x0683;// set baud rate to 9600 (with 16 MHz clock)
	USART2->CR1 |= USART_CR1_UE;// enable USART2 module	
}


void usart2_writeChar(char msg_char)
{
	while(!(USART2->SR & USART_SR_TXE)); // wait while transmit data register is full (register empty => TXE=1)
	USART2->DR = (uint8_t) msg_char; // write char in data register
}


void usart2_printString(char *msg_string)
{
	size_t msg_string_len = strlen(msg_string); // get size of string pointed to by *msg_string
	// print each character of input string
	for (uint32_t i = 0; i < msg_string_len; i++)
		usart2_writeChar(msg_string[i]);
}


void usart2_writeString_oncePerBoardButtonPress(char* msg_string)
{
	static int8_t written = 0;
		
	if(!boardButton_pressed())
		written = 0;		
		
	if(boardButton_pressed() && !written){
		usart2_printString(msg_string);			
		written = 1;
	}
}

// returns received char of USART2
char usart2_readChar(void){
	if(USART2->SR & USART_SR_RXNE){ // if data is ready to be read
		return USART2->DR; // read received char and return it
	}
	else return '\0';
}
