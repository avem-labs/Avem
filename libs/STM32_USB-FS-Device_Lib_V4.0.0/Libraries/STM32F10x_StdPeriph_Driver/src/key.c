#include "key.h"
#include "stm32f10x.h"
void Key_init() {
    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;   //GPIOC
    GPIOC->CRH &= 0xFF0FFFFF;   //Clean Pin 13
    GPIOC->CRH |= 0x00800000;   //上拉输入
    GPIOC->ODR |= 1<<13;
}
