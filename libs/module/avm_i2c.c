#include <avm_core.h>

static unsigned char avm_i2c_init(void *arg);

avm_module_t avm_i2c_module_st = {
    0,
    NULL,
    avm_i2c_init,
    NULL,
    NULL
};

unsigned char avm_i2c_init(void *arg) {
    IIC_init();
    return 0;
}

static void delay_custom(volatile unsigned int nus) {
    for(nus *= 4; nus; nus--);
}

void IIC_init() {
    RCC->APB2ENR |= RCC_APB2ENR_IOPBEN;       //GPIOB enable
    GPIOB->CRH &= 0x00FFFFFF;   //reset B13 & B15
    GPIOB->CRH |= 0x77000000;   //Set B13 & B15 b0111
    SDA = 1;
    SCL = 1;

//这里B14和B15设置为开漏输出模式(01), 其实一般都用推挽输出模式。
//开漏输出模式有个"好的Bug": 在输出状态下，输入信号没有关断，后面的路也是畅通的
//最终就会进入输入寄存器，导致在输出状态下，输入功能保持正常。
//如果用推挽输出的话，IIC通讯时每次读取SDA之前都要配置SDA为输入，才能读取IDR

//    RCC->APB2ENR |= RCC_APB2ENR_IOPCEN;       //GPIOC enable
//    GPIOC->CRL &= 0xF0FFFFFF;
//    GPIOC->CRL |= 0x03000000;   //推挽输出, AD0 ----> PC6
//    AD0 = 0;
}

void IIC_Start() {
    SDA = 1;
    SCL = 1;
    IIC_DELAY();
    SDA = 0;
    IIC_DELAY();
    SCL = 0;
}

void IIC_Stop() {
    SCL = 0;
    IIC_DELAY();
    SDA = 0;
    IIC_DELAY();
    SCL = 1;
    IIC_DELAY();
    SDA = 1;
    IIC_DELAY();
}

void IIC_Ack(unsigned char ack) {
    SCL = 0;
    IIC_DELAY();
    SDA = ack;
    IIC_DELAY();
    SCL = 1;
    IIC_DELAY();
    SCL = 0;
    IIC_DELAY();
}

unsigned char IIC_Wait_Ack() {
    SCL = 0;
    IIC_DELAY();
    SDA = 1;
    IIC_DELAY();
    SCL = 1;
    IIC_DELAY();
    if(READ_SDA) {
        SCL = 0;
        IIC_DELAY();
        return 0;
    }

    SCL = 0;
    return 1;
}

void IIC_Send(unsigned char dat) {
    unsigned char i;
    SCL = 0;
    for(i = 0; i < 8; i++) {
        IIC_DELAY();
        SDA = (dat&0x80)>>7;
        dat <<= 1;
        IIC_DELAY();
        SCL = 1;
        IIC_DELAY();
        SCL = 0;
    }
}

unsigned char IIC_Read() {
    unsigned char i, dat = 0;
    SDA = 1;
    for(i = 0; i < 8; i++) {
        SCL = 0;
        IIC_DELAY();
        SCL = 1;
        IIC_DELAY();
        dat<<=1;
        dat |= READ_SDA;
    }
    SCL = 0;
    return dat;
}
