#include "uart.h"
#include "stm32f10x.h"

int top = -1;	//Stack Pointer

void uart_init(unsigned int pclk2, unsigned int bound) {
    float temp;
    unsigned short mantissa;
    unsigned short fraction;
    temp = (float)(pclk2*1000000)/(bound*16);
    mantissa = temp;
    fraction = (temp - mantissa) * 16;
    mantissa <<= 4;
    mantissa += fraction;
    RCC->APB2ENR |= RCC_APB2ENR_IOPAEN;   //GPIOA Enable
    RCC->APB2ENR |= RCC_APB2ENR_USART1EN;  //USART1 Enable

    GPIOA->CRH &= 0xFFFFF00F;
    GPIOA->CRH |= 0x000008B0;

    RCC->APB2RSTR |= RCC_APB2RSTR_USART1RST;
    RCC->APB2RSTR &= ~RCC_APB2RSTR_USART1RST;

    USART1->BRR = mantissa;
    USART1->CR1 |= 0x200C;	//Set up UE, TE and RE

    USART1->CR1 |= 1<<8;	//PE Interrup Enable
    USART1->CR1 |= 1<<5;	//RX Not Empty Interrup Enable


    USART1->SR;     //Read Reg SR to Clean TXE and TE,(Reset value: 0x00C0)

	SCB->AIRCR &= 0x05FAF8FF;	// AIRCE Key: 0x05FA
	SCB->AIRCR |= 0x05FA0400;	// Set up group value

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 3);

}

void USART1_IRQHandler(void) {
	if(USART1->SR & USART_SR_RXNE) {
		char cmd = USART1->DR;	// Read this register to clear RXNE flag
		switch (cmd) {
			case 0x0D:
			case 0x0A:
				UART_CR();
				uart_sendStr("Handle Command:\t");
				uart_sendStr(gCmdCache);
				UART_CR();
				clrCache();
				break;
			case 0x08:
			case 0x7F:
				pop = '\0';
				uart_sendData(0x7F);
				uart_sendData(0x08);
				break;
			case '$':
				clrCache();
			default:
				push(cmd);
				uart_sendData(cmd);
				break;
		}
	}
}

void uart_sendData(unsigned char data) {
    USART1->DR = data;
    while((USART1->SR & 0x40) == 0);
}

void uart_showData(short k) {
	char tem[] = "00000";
	char *result = tem;

	uart_sendData(k<0 ? '-':'+');
	if(k<0)
		k = -k;
	unsigned short bit = 1;
	while(*result) {
		(*result++) = (char)(k / bit % 10 + '0');
		bit *= 10;
	}
	for(unsigned char i = 0; i < 5; i++)
		uart_sendData(tem[4-i]);
}

unsigned char uart_Float2Char(float value) {
    unsigned char IntegerPart;
    float DecimalPart;
    unsigned char i = 0;
    char temp;

    char array[8] = {0,0,0,0,0,0,0};

    if(value < 0 ) {
        value = value * -1;
        uart_sendData('-');
    }

    if(value >= 1) {
        IntegerPart = (unsigned char)value;
        DecimalPart = value - IntegerPart;
    } else {
        IntegerPart = 0;
        DecimalPart = value - IntegerPart;
    }

    if(IntegerPart == 0) {
        array[0] = '0';
        array[1] = '.';
        i = 1;
    } else {
        while(IntegerPart > 0) {
            array[i] = IntegerPart%10 + '0';
            IntegerPart/=10;
            i++;
        }
        i--;
        for(unsigned char j = 0; j < i; j++) {
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

    for(unsigned char j = 0; j < i; j ++) {
        uart_sendData(array[j]);
    }

    return i;
}

void uart_sendStr(char *cmd) {
	while(*cmd)
		uart_sendData(*cmd++);
}
