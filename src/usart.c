#include "stm32f4xx.h"
#include "USART.h"
#include <stdint.h>
#include <stdlib.h>
#include <math.h>  
#include <stdio.h>
#include <stddef.h>
#include <string.h>


// variable definitions:
// function definitions:
void usart2_init(void)
{
	// USART2_RX uses port A2 and USART2_TX uses port A3 if each are set to alternate function 7 (p.59 datasheet)
	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;	// enable clock to USART2
	RCC->AHB1ENR |= RCC_AHB1ENR_GPIOAEN; // enable clock to Port A
	GPIOA->MODER |= (GPIO_MODER_MODE2_1|GPIO_MODER_MODE3_1); // set PA2 and PA3 to alternate function 0b10
	//GPIOA->AFR[0] |= (GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_0 | GPIO_AFRL_AFRL3_2 | GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_0); // set PA2 to AF7 (USART2_TX) and PA3 to AF7 (USART2_RX)
	MODIFY_REG(GPIOA->AFR[0], GPIO_AFRL_AFRL2|GPIO_AFRL_AFRL3, GPIO_AFRL_AFRL2_2 | GPIO_AFRL_AFRL2_1 | GPIO_AFRL_AFRL2_0 | GPIO_AFRL_AFRL3_2 
		| GPIO_AFRL_AFRL3_1 | GPIO_AFRL_AFRL3_0);
	USART2->CR1 |= (USART_CR1_TE|USART_CR1_RE);// enable transmitter and receiver
	// Baud rate Infos can be found on page 808 of reference manual)
	USART2->BRR = 0x0683;// set baud rate to 9600 (with 16 MHz clock)
	// enable USART2 interrupt:
	USART2->CR1 |= USART_CR1_RXNEIE; // enable receive interrupt
	NVIC_EnableIRQ(USART2_IRQn); // enable USART2 interrupt in NVIC
	USART2->CR1 |= USART_CR1_UE;// enable USART2 module
}


void usart2_writeChar(char msg_char)
{
	while(!(USART2->SR & USART_SR_TXE)); // wait while transmit data register is full (register empty => TXE=1)
	USART2->DR = (uint8_t) msg_char; // write char in data register
}


void usart2_writeString(char *msg_string)
{
	size_t msg_string_len = strlen(msg_string); // get size of string pointed to by *msg_string
	// print each character of input string
	for (uint32_t i = 0; i < msg_string_len; i++)
		usart2_writeChar(msg_string[i]);
}


char usart2_readChar(void)
{
	if (USART2->SR & USART_SR_RXNE){ // if data is ready to be read
		return USART2->DR; // read received char and return it
	}
	else return '\0';
}


// similar to: https://stackoverflow.com/questions/905928/using-floats-with-sprintf-in-embedded-c:
char *convertDoubleToString(double input)
{
	static char str[50];
	// reset static char array:
	size_t sizeOfStr = sizeof(str)/sizeof(str[0]);
	size_t i;
	for (i = 0; i < sizeOfStr; i++)
	{
		str[i] = 0;
	}
	// building the output string:
	char *tmpSign = (input < 0) ? "-" : "";
	double tmpVal = (input < 0) ? -input : input;

	int32_t tmpInt1 = tmpVal;                  // Get the integer (678).
	double tmpFrac = tmpVal - tmpInt1;      // Get fraction (0.0123).
	int32_t tmpInt2 = trunc(tmpFrac * 10000);  // Turn into integer (123).

	// Print as parts, note that you need 0-padding for fractional bit.
	sprintf (str, "%s%ld.%04ld", tmpSign, tmpInt1, tmpInt2);
	return str;
}


//___________________________________________________________________________________
// interrupt request handlers:
void USART2_IRQHandler(void)
{
	char c = usart2_readChar();
	if (c == 'y')
		{
			usart2_writeString("Yout typed y\n");
		}
}
