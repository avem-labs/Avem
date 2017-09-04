#include "avm_core.h"

static unsigned char avm_uart_init(void *conf);
static unsigned int avm_uart_conf[] = {72, 115200};

avm_module_t avm_uart_module_st = {
    0,
    &avm_uart_conf[0],
    avm_uart_init,
    NULL,
    NULL
};


static unsigned char avm_uart_init(void *conf) {
    uart_init(((unsigned int *)conf)[0], ((unsigned int *)conf)[1]);
    return 0;
}


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

	NVIC_EnableIRQ(USART1_IRQn);
	NVIC_SetPriority(USART1_IRQn, 3);

}

void USART1_IRQHandler(void) {
	if(USART1->SR & USART_SR_RXNE) {
		const char cmd = USART1->DR;	// 读取串口接收寄存器来清除 RXNE 标志
		switch (cmd) {
			case TOKEN:
				MPU6050_getStructData(&avm_euler);
				IMU_Comput(avm_euler);
                pid_SingleAxis(&avm_pid, 0);

				uart_Float2Char(g_Pitch);
				uart_sendStr("@");

				uart_Float2Char(g_Roll);
				uart_sendStr("@");

				uart_Float2Char(g_Yaw);
                uart_sendStr("@");

                uart_short2char(*avm_pid.Channel1);
                uart_sendStr("@");

                uart_short2char(*avm_pid.Channel2);
				uart_sendStr("\n\r");
				break;
			case '>':	// Jump to bootloader
				uart_sendStr("Running bootloader...");
				jump2ISP();
				// NOTE: running bootloader
			default:	//其它按键
				break;
		}
	}
}

void uart_sendData(unsigned char data) {
    USART1->DR = data;
    while((USART1->SR & 0x40) == 0);
}

void uart_short2char(short k) {
	char cache[] = "00000";
	int i = 4;
	unsigned short bit[] = {10000, 1000, 100, 10, 1};

	do {
		cache[i] += (char)(k / bit[i] % 10);
	} while(i--);
	uart_sendStr(cache);
}

void uart_int2char(unsigned int k) {
	char cache[] = "0000000000";	// Max value is 4294967295
	unsigned char i = 9;
	const unsigned int bit[] = {1000000000, 100000000, 10000000, 1000000, 100000, 10000, 1000, 100, 10, 1};

	do {
		cache[i] += (char)(k / bit[i] % 10);
	} while(i--);
	uart_sendStr(cache);
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
