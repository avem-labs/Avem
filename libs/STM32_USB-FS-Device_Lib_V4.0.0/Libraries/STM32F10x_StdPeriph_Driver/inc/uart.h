#pragma once
#ifndef _UART_H
#define _UART_H

void initUART(unsigned int pclk2, unsigned int bound);
void sendData_uart(unsigned char data);
void showData(short k);
unsigned char Float2Char(float value);

#endif
