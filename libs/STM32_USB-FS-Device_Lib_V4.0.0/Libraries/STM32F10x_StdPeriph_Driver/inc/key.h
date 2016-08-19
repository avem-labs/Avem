#pragma once
#ifndef _KEY_H
#define _KEY_H
void delay(volatile unsigned int count);
#define KEY1 BIT_ADDR((GPIOC_BASE+8), 13)
#define PAUSE() {while(KEY1);delay(10);}

void Key_init();

#endif
