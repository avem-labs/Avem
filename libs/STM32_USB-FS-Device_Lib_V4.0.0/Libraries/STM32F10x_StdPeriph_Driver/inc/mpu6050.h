#pragma once
#ifndef MPU6050_H_
#define MPU6050_H_

//=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_
#define LED0 BIT_ADDR((GPIOA_BASE+12), 8)
#define LED1 BIT_ADDR((GPIOD_BASE+12), 2)
//=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_

void delay(volatile unsigned int count);
//=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_

#define MPU6050_ADDR 0xD0 //    0x68 >> 1

#define SMPLRT_DIV      0x19
#define CONFIG 0x1A
#define GYRO_CONFIG     0x1B
#define ACCEL_CONFIG    0x1C

#define ACCEL_XOUT_H    0x3B
#define ACCEL_XOUT_L    0x3C
#define ACCEL_YOUT_H    0x3D
#define ACCEL_YOUT_L    0x3E
#define ACCEL_ZOUT_H    0x3F
#define ACCEL_ZOUT_L    0x40

#define TEMP_OUT_H      0x41
#define TEMP_OUT_L      0x42

#define GYRO_XOUT_H     0x43
#define GYRO_XOUT_L     0x44
#define GYRO_YOUT_H     0x45
#define GYRO_YOUT_L     0x46
#define GYRO_ZOUT_H     0x47
#define GYRO_ZOUT_L     0x48

#define PWR_MGMT_1      0x6B
#define WHO_AM_I        0x75

//=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_

#define A_Y_OFFSET 11
#define G_X_OFFSET 4
#define A_Z_OFFSET 0

#define IMU_ADDRESS 0x68
#define IMU_CONNECTED (MPU_Sigle_Read(WHO_AM_I)==IMU_ADDRESS)

typedef struct{
    float gX;
    float gY;
    float gZ;
    float aX;
    float aY;
    float aZ;
}SixAxis, *pSixAxis;


void initLED();
void MPU_Sigle_Write(unsigned char reg_addr, unsigned char reg_data);
unsigned char MPU_Sigle_Read(unsigned reg_addr);
short MPU_GetData(unsigned char REG_Addr);
void MPU_init();
void MPU6050_getStructData(pSixAxis cache);
void MPU6050_debug(pSixAxis cache);

#endif
