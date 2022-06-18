#if !defined(__USART_H)
#define __USART_H

void usart2_init(void);
void usart2_writeChar(char msg_char);
void usart2_printString(char *msg_string);
void usart2_writeString_oncePerBoardButtonPress(char* msg_string);
char usart2_readChar(void);

#endif // __USART_H
