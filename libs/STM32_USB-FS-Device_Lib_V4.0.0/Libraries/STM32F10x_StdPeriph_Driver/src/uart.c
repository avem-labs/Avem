#include "uart.h"
#include "stm32f10x.h"

void initUART(unsigned int pclk2, unsigned int bound) {
    float temp;
    unsigned short mantissa;
    unsigned short fraction;
    temp = (float)(pclk2*1000000)/(bound*16);
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
void sendData_uart(unsigned char data) {
    USART1->DR = data;
    while((USART1->SR & 0x40) == 0);
}
void showData(short k) {
    unsigned char a, b, c, d, e;

    sendData_uart(k<0?'-':'+');
    if(k<0) k=-k;
    e = (unsigned char)(k % 10);
    d = (unsigned char)(k/10) % 10;
    c = (unsigned char)(k/100) % 10;
    b = (unsigned char)(k/1000) % 10;
    a = (unsigned char)(k/10000);

    sendData_uart(a+0x30);
    sendData_uart(b+0x30);
    sendData_uart(c+0x30);
    sendData_uart(d+0x30);
    sendData_uart(e+0x30);
    sendData_uart(' ');
}

unsigned char Float2Char(float value) {
    unsigned char IntegerPart;
    float DecimalPart;
    unsigned char i = 0;
    unsigned char j = 0;
    char temp;

    char array[8] = {0,0,0,0,0,0,0};

    if(value < 0 ) {
        value = value * -1;
        sendData_uart('-');
    }
    if(value >= 1) {
        IntegerPart = (unsigned char)value;
        DecimalPart = value - IntegerPart;
    }
    else {
        IntegerPart = 0;
        DecimalPart = value - IntegerPart;
    }
    if(IntegerPart == 0) {
        array[0] = '0';
        array[1] = '.';
        i = 1;
    }
    else {
        while(IntegerPart > 0) {
            array[i] = IntegerPart%10 + '0';
            IntegerPart/=10;
            i++;
        }
        i--;
        for(j = 0; j < i; j++) {
            temp = array[j];
            array[j] = array[i-j];
            array[i-j] = temp;
        }
        i++;
        array[i] = '.';
    }
    i++;
    array[i++] = (unsigned int)(DecimalPart * 10)%10 + '0';
    array[i++] = (unsigned int)(DecimalPart * 100)%10 + '0';
    array[i++] = (unsigned int)(DecimalPart * 1000)%10 + '0';
    array[i++] = (unsigned int)(DecimalPart * 10000)%10 + '0';
    array[i++] = '\0';

    for(j = 0; j < i; j ++) {
        sendData_uart(array[j]);
    }

    return i;
}

