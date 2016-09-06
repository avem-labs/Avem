#pragma once
#ifndef UART_H_
#define UART_H_

void uart_init(unsigned int pclk2, unsigned int bound);
void uart_sendData(unsigned char data);
void uart_showData(short k);    //THIS FUNCTION IS NOT SO GOOD, EG: uart_showData(1000)
unsigned char uart_Float2Char(float value);
void uart_sendStr(char * cmd);
#endif
