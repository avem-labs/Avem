#pragma once
#ifndef _WIFI_H
#define _WIFI_H
//Only USART1 PCLK2 = 72
#define WIFI_PCLK2 36
#define WIFI_BOUND 115200

void wifi_init();
void wifi_sendSingleByte(unsigned char cmd);
void wifi_sendData(char * cmd);
void wifi_sendCmd(char * cmd);

#endif
