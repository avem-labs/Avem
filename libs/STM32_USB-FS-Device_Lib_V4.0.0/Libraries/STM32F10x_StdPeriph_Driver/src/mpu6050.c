#include "mpu6050.h"

#include "bit.h"
#include "stm32f10x.h"
#include "i2c.h"
#include "uart.h"

void MPU_Sigle_Write(unsigned char reg_addr, unsigned char reg_data) {
    IIC_Start();
    IIC_Send(MPU6050_ADDR);
    if(!IIC_Wait_Ack()) LED1 = 1;

    IIC_Send(reg_addr);
    IIC_Wait_Ack();

    IIC_Send(reg_data);
    IIC_Wait_Ack();

    IIC_Stop();
}

unsigned char MPU_Sigle_Read(unsigned reg_addr) {
    unsigned char reg_data;
    IIC_Start();
    IIC_Send(MPU6050_ADDR);
    IIC_Wait_Ack();

    IIC_Send(reg_addr);
    IIC_Wait_Ack();

    IIC_Start();
    IIC_Send(MPU6050_ADDR+1);
    IIC_Wait_Ack();

    reg_data = IIC_Read();
    IIC_Ack(1);
    IIC_Stop();
    return reg_data;
}

short MPU_GetData(unsigned char REG_Addr) {
    unsigned char H, L;
    H = MPU_Sigle_Read(REG_Addr);
    L = MPU_Sigle_Read(REG_Addr+1);
    return (short)(H<<8)+L;
}

void MPU_init() {
    IIC_init();
    do {
        MPU_Sigle_Write(PWR_MGMT_1, 0x00);
        MPU_Sigle_Write(SMPLRT_DIV, 0x07);
        MPU_Sigle_Write(CONFIG, 0x06);
        MPU_Sigle_Write(GYRO_CONFIG, 0x18);
        MPU_Sigle_Write(ACCEL_CONFIG, 0x01);
    } while(IMU_NOT_CONNECTED);
}

void MPU6050_getStructData(pSixAxis cache) {
    cache->gX = (float)MPU_GetData(GYRO_XOUT_H)/16.4f;
    cache->gY = (float)MPU_GetData(GYRO_YOUT_H)/16.4f;
    cache->gZ = (float)MPU_GetData(GYRO_ZOUT_H)/16.4f;

    cache->aX = (float)MPU_GetData(ACCEL_XOUT_H)/1671.83f;
    cache->aY = (float)MPU_GetData(ACCEL_YOUT_H)/1671.83f;
    cache->aZ = (float)MPU_GetData(ACCEL_ZOUT_H)/1671.83f;

    cache->aY += A_Y_OFFSET;
    cache->gX += G_X_OFFSET;
    cache->aZ += A_Z_OFFSET;
}
void MPU6050_debug(pSixAxis cache) {
    uart_Float2Char((float)cache->gX);
    uart_sendData(' ');
    uart_Float2Char((float)cache->gY);
    uart_sendData(' ');
    uart_Float2Char((float)cache->gZ);
    uart_sendData(' ');

    uart_Float2Char((float)cache->aX);
    uart_sendData(' ');
    uart_Float2Char((float)cache->aY);
    uart_sendData(' ');
    uart_Float2Char((float)cache->aZ);
    uart_sendData(' ');
    uart_sendData(0x0D);
    uart_sendData(0x0A);
}
