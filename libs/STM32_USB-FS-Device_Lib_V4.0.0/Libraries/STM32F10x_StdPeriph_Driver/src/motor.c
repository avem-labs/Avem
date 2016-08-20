#include "stm32f10x.h"
#include "motor.h"

//A7
void motor_PWM_Init(unsigned short arr, unsigned short psc) {
    RCC->APB1ENR |= 1<<1;       //TIM3 enable
    RCC->APB2ENR |= 1<<2;       //GPIOA enable

    GPIOA->CRL &= 0x00FFFFFF;
    GPIOA->CRL |= 0xBB000000;   //复用推挽输出
    GPIOA->ODR |= 1<<7;     //CH2 GPIO Config
    GPIOA->ODR |= 1<<6;     //CH1 GPIO Config

    TIM3->ARR = arr - 1;
    TIM3->PSC = psc - 1;

    TIM3->CCMR1 |= 6<<12;   //CH2 Set OC2M[2:0]: PWM Mode
    TIM3->CCMR1 |= 1<<11;   //CH2 Set OC2PF: Enable
    TIM3->CCMR1 |= 7<<4;    //CH1 Set OC2M[2:0]: PWM Mode
    TIM3->CCMR1 |= 1<<3;    //CH1 Set OC2PF: Enable

    TIM3->CCER |= 1<<4;     //CH2 Output Enable
    TIM3->CCER |= 1;        //CH1 Output Enable

    TIM3->CR1 = 0x80;       //APRE Enable
    TIM3->CR1 |= 1;         //Set CEN, Allow to Count
    //TIM1->BDTR |= 1<<15;
}
