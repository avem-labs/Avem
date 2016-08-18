#pragma once
#ifndef _WIFI_H
#define _WIFI_H

#define WIFI_PCLK2 72
#define WIFI_BOUND 9600

void wifi_init();
void wifi_sendData(unsigned char cmd);

#endif
