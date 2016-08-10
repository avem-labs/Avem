#include "stm32f10x.h"
#include "motor.h"

//A7
void PWM_Init(unsigned short arr, unsigned short psc) {
    RCC->APB1ENR |= 1<<1;       //TIM3 enable
    RCC->APB2ENR |= 1<<2;       //GPIOA enable
    GPIOA->CRL &= 0x0FFFFFFF;
    GPIOA->CRL |= 0xB0000000;   //复用推挽输出
    GPIOA->ODR |= 1<<7;
    TIM3->ARR = arr - 1;
    TIM3->PSC = psc - 1;
    TIM3->CCMR1 |= 7<<12;
    TIM3->CCMR1 |= 1<<11;
    TIM3->CCER |= 1<<4;
    TIM3->CR1 = 0x80;
    TIM3->CR1 |= 1;
    //TIM1->BDTR |= 1<<15;
}
