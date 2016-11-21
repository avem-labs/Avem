#pragma once
#ifndef UART_H
#define UART_H

void uart_init(unsigned int pclk2, unsigned int bound);
void uart_sendData(unsigned char data);
void uart_showData(short k);    //THIS FUNCTION IS NOT SO GOOD, EG: uart_showData(1000)
unsigned char uart_Float2Char(float value);
void uart_sendStr(char * cmd);

#define UART_CR() {\
    uart_sendData(0x0D);\
    uart_sendData(0x0A);\
}

#endif
