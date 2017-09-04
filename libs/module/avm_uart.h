#ifndef _AVM_UART_H_
#define _AVM_UART_H_


void uart_init(unsigned int pclk2, unsigned int bound);
void uart_sendData(unsigned char data);
void uart_short2char(short k);
void uart_int2char(unsigned int k);
unsigned char uart_Float2Char(float value);
void uart_sendStr(char *cmd);
void USART1_IRQHandler(void);

extern avm_module_t avm_uart_module_st;

#define UART_CR() do {\
    uart_sendData(0x0D);\
    uart_sendData(0x0A);\
} while(0)

#define UART_CLEAR() uart_sendStr("\033[H\033[J")

#define TOKEN           '$'

#endif
