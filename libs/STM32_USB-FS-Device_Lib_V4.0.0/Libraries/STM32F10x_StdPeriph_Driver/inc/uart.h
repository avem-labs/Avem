#pragma once
#ifndef _UART_H
#define _UART_H

void uart_init(unsigned int pclk2, unsigned int bound);
void uart_sendData(unsigned char data);
void uart_showData(short k);
unsigned char uart_Float2Char(float value);

#endif
