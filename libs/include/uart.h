#ifndef UART_H
#define UART_H

void uart_init(unsigned int pclk2, unsigned int bound);
void uart_sendData(unsigned char data);
void uart_showData(short k);    //THIS FUNCTION IS NOT SO GOOD, EG: uart_showData(1000)
unsigned char uart_Float2Char(float value);
void uart_sendStr(char * cmd);
void USART1_IRQHandler(void);
void uart_decode();

#define UART_CR() do {\
    uart_sendData(0x0D);\
    uart_sendData(0x0A);\
} while(0)
#define UART_CLEAR() uart_sendStr("\033[H\033[J")

#define CMD_MAX_LENGTH	32
#define pop		gCmdCache[top--]
#define push(s)	gCmdCache[++top] = s
#define clrCache()	{\
	while (top > -1) {\
		pop = '\0';\
	}\
}
#define STACK_OVERFLOW	(top == CMD_MAX_LENGTH - 1)

//	decode
#define TOKEN_SEND	'S'
#define TOKEN_LEARN	'L'
#define CMD_NUM_MAX	'8'
#define CMD_NUM_MIN	'1'
#define ISLEGAL_NUM(k)	(((k) >= CMD_NUM_MIN) && ((k) <= CMD_NUM_MAX))

#endif
