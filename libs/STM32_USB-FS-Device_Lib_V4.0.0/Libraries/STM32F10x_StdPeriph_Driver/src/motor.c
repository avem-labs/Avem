#include "motor.h"
#include "stm32f10x.h"

//A7
void motor_PWM_Init(unsigned short arr, unsigned short psc) {
    //RCC->APB1ENR |= 1<<1;       //TIM3 enable
    //RCC->APB2ENR |= 1<<2;       //GPIOA enable

    RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;   //TIM3 Enable
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   //IO Port A Enable
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;

    GPIOA->CRL &= 0x00FFFFFF;   //Clean
    GPIOA->CRL |= 0xBB000000;   //复用推挽输出
    GPIOA->ODR |= 1<<7;     //CH2 GPIO Config
    GPIOA->ODR |= 1<<6;     //CH1 GPIO Config

    GPIOB->CRL &= 0xFFFFFF00;
    GPIOB->CRL |= 0x000000BB;
    GPIOB->ODR |= 1;
    GPIOB->ODR |= 1<<1;

    TIM3->ARR = arr - 1;
    TIM3->PSC = psc - 1;

    TIM3->CCMR1 |= 6<<4;    //CH1 Set OC1M[2:0]: PWM Mode
    TIM3->CCMR1 |= 1<<3;    //CH1 Set OC1PE: Enable
    TIM3->CCMR1 |= 6<<12;   //CH2 Set OC2M[2:0]: PWM Mode
    TIM3->CCMR1 |= 1<<11;   //CH2 Set OC2PE: Enable
    TIM3->CCMR2 |= 6<<4;    //CH3 Set OC3M[2:0]: PWM Mode
    TIM3->CCMR2 |= 1<<3;    //CH3
    TIM3->CCMR2 |= 6<<12;   //CH4
    TIM3->CCMR2 |= 1<<11;

    TIM3->CCER |= 1;        //CH1 Output Enable
    TIM3->CCER |= 1<<4;     //CH2 Output Enable
    TIM3->CCER |= 1<<8;
    TIM3->CCER |= 1<<12;

    TIM3->CR1 = 0x80;       //APRE Enable
    TIM3->CR1 |= 1;         //Set CEN, Allow to Count
    //TIM1->BDTR |= 1<<15;  //高级定时器需要使能BDTR寄存器
}
