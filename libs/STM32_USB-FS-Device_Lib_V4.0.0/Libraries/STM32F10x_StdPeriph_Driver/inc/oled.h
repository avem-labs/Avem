#pragma once
#ifndef OLED_H_
#define OLED_H_

#define SDIN_OLED BIT_ADDR((GPIOB_BASE+12), 3)
#define SCLK_OLED BIT_ADDR((GPIOB_BASE+12), 4)
#define DC_OLED BIT_ADDR((GPIOB_BASE+12), 5)
#define RST_OLED BIT_ADDR((GPIOB_BASE+12), 6)
#define CS_OLED BIT_ADDR((GPIOB_BASE+12), 7)


void delay_us(volatile unsigned int nus);
void oled_portInit();
void oled_sendData(unsigned char dat);
void oled_sendCmd(unsigned char cmd);
void oled_init();
void fill_ram (unsigned char h, unsigned char l);


#endif
