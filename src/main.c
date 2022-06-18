#include "stm32f4xx.h"                  // Device header
#include <stdint.h>
#include "examples.h"
#include "usart.h"
#include <stdlib.h>
#include <string.h>

#define XTALFREQ 16000000UL

struct SafeString 
{
	char string[5];
	int length;
};

uint8_t setString_SafeString(struct SafeString safeStringInstance, char* string)
{
	if (strlen(string) <safeStringInstance.length) // if SafeString is big enough for string
	{
		strcpy(safeStringInstance.string, string);
	}
}

void main()
{
	usart2_init();
	char c;
	
	while(1)
	{
		c = usart2_readChar();
		if (c != '\0')
		{
			usart2_writeChar(c);
		}
		
	}
}
