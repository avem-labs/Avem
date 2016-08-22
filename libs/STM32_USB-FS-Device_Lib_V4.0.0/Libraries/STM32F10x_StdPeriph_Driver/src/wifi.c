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

    RCC->APB2ENR |= 1<<3;   //GPIOB Enable
    RCC->APB1ENR |= 1<<18;  //USART3 Enable

    GPIOB->CRH &= 0xFFFF00FF;
    GPIOB->CRH |= 0x00008B00;

    RCC->APB1RSTR |= 1<<18;
    RCC->APB1RSTR &= ~(1<<18);

    USART3->BRR = mantissa;
    USART3->CR1 |= 0x200C;

    USART3->CR1 |= 1<<8;
    USART3->CR1 |= 1<<5;
}

void wifi_sendSingleByte(unsigned char cmd) {
    USART3->DR = cmd;
    while((USART3->SR & 0x40) == 0);
}

void wifi_sendData(char * cmd) {
    unsigned short pointer = 0;
    while(*(cmd + pointer)) {
        wifi_sendSingleByte((unsigned char)*(cmd + pointer));
        pointer++;
    }

    wifi_sendSingleByte(0x0D);
    wifi_sendSingleByte(0x0A);
}

void wifi_sendCmd(char * cmd) {
    wifi_sendData("AT+");
    wifi_sendData(cmd);
}
