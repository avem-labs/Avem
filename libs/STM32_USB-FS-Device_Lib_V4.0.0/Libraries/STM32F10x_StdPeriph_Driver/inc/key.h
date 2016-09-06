#pragma once
#ifndef KEY_H_
#define KEY_H_
void delay(volatile unsigned int count);
#define KEY1 BIT_ADDR((GPIOC_BASE+8), 13)
#define PAUSE() {while(KEY1);delay(300);}

void Key_init();

#endif
