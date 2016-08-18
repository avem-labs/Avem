#include "wifi.h"

#include "stm32f10x.h"

void wifi_init() {
    float temp;
    unsigned short mantissa;
    unsigned short fraction;
    temp = (float)(WIFI_PCLK2 * 1000000) / (WIFI_BOUND * 16);
    mantissa = temp;
    fraction = (temp - mantissa) * 16;
    mantissa <<= 4;
    mantissa += fraction;
    RCC->APB2ENR |= 1<<2;   //GPIOA Enable
    RCC->APB2ENR |= 1<<14;  //USART1 Enable

    GPIOA->CRH &= 0xFFFFF00F;
    GPIOA->CRH |= 0x000008B0;

    RCC->APB2RSTR |= 1<<14;
    RCC->APB2RSTR &= ~(1<<14);

    USART1->BRR = mantissa;
    USART1->CR1 |= 0x200C;

    USART1->CR1 |= 1<<8;
    USART1->CR1 |= 1<<5;

}

