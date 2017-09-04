#ifndef _AVM_I2C_H_
#define _AVM_I2C_H_


#define SCL_PINNUM 15
#define SDA_PINNUM 14
#define SCL BIT_ADDR(&(GPIOB->ODR), SCL_PINNUM)
#define SDA BIT_ADDR(&(GPIOB->ODR), SDA_PINNUM)
//#define AD0 BIT_ADDR((GPIOC_BASE+12), 6)


#define READ_SDA BIT_ADDR(&(GPIOB->IDR), SDA_PINNUM)

#define IIC_DELAY() delay_custom(1)

extern avm_module_t avm_i2c_module_st;


// void delay_us(volatile unsigned int nus);

void IIC_init();
void IIC_Start();
void IIC_Stop();
void IIC_Ack(unsigned char ack);
unsigned char IIC_Wait_Ack();
void IIC_Send(unsigned char dat);

unsigned char IIC_Read();



#endif
