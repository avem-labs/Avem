#include <math.h>
#include "stm32f10x.h"
#include "bit.h"
#include "mpu6050.h"
#include "motor.h"
#include "uart.h"

#include "wifi.h"

#define Kp      100.0f      //比例增益支配率(常量)
#define Ki      0.002f      //积分增益支配率
#define halfT   0.001f      //采样周期的一半

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;   //Quaternion
float exInt = 0, eyInt = 0, ezInt = 0;
float Yaw, Pitch, Roll;     //eular

#define K_P 0.18f //0.257 * 0.83 0.255
#define K_I 0
#define K_D 0
#define SUM_ERRO_MAX 900
#define SUM_ERRO_MIN -900


float iErro, sumErro = 0;
void pid(float setPoint, float d) {
    iErro = Roll - setPoint;    //计算当前误差
    sumErro += iErro;       //对误差进行积分

    if(sumErro > SUM_ERRO_MAX) sumErro = SUM_ERRO_MAX;  //积分限幅
    else if(sumErro < SUM_ERRO_MIN) sumErro = SUM_ERRO_MIN;

    unsigned short resu = (short)(iErro * K_P + sumErro * K_I + d * K_D);  //PID输出

    MOTOR1 += resu;
    MOTOR2 -= resu;

    if(MOTOR1 > 7199) MOTOR1 = 7199;
    else if(MOTOR1 < 800) MOTOR1 = 800;

    if(MOTOR2 > 7199) MOTOR2 = 7199;
    else if(MOTOR2 < 800) MOTOR2 = 800;
}

//ms
void delay(volatile unsigned int count) {
    for(count *= 12000; count!=0; count--);
}

void Comput(SixAxis cache) {

    float norm;     //模
    float vx, vy, vz;
    float ex, ey, ez;

    norm = sqrt(cache.aX*cache.aX + cache.aY*cache.aY + cache.aZ*cache.aZ);     //取模


    //向量化
    cache.aX = cache.aX / norm;
    cache.aY = cache.aY / norm;
    cache.aZ = cache.aZ / norm;


    //估计方向的重力
    vx = 2 * (q1 * q3 - q0 * q2);
    vy = 2 * (q0 * q1 + q2 * q3);
    vz = q0*q0 - q1*q1 - q2*q2 + q3*q3;

    //错误的领域和方向传感器测量参考方向几件的交叉乘积的总和
    ex = (cache.aY * vz - cache.aZ * vy);
    ey = (cache.aZ * vx - cache.aX * vz);
    ez = (cache.aX * vy - cache.aY * vx);

    //积分误差比例积分增益
    exInt = exInt + ex * Ki;
    eyInt = eyInt + ey * Ki;
    ezInt = ezInt + ez * Ki;

    //调整后的陀螺仪测量
    cache.gX = cache.gX + Kp * ex + exInt;
    cache.gY = cache.gY + Kp * ey + eyInt;
    cache.gZ = cache.gZ + Kp * ez + ezInt;

    //整合四元数率和正常化
    q0 = q0 + (-q1 * cache.gX - q2 * cache.gY - q3 * cache.gZ) * halfT;
    q1 = q1 + (q0 * cache.gX + q2 * cache.gZ - q3 * cache.gY) * halfT;
    q2 = q2 + (q0 * cache.gY - q1 * cache.gZ + q3 * cache.gX) * halfT;
    q3 = q3 + (q0 * cache.gZ + q1 * cache.gY - q2 * cache.gX) * halfT;

    //正常化四元
    norm = sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
    q0 = q0 / norm;
    q1 = q1 / norm;
    q2 = q2 / norm;
    q3 = q3 / norm;

    Pitch = asin(-2 * q1 * q3 + 2 * q0 * q2) * 57.3;
    Roll = atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1*q1 - 2 * q2*q2 + 1) * 57.3;
    Yaw = atan2(2 * (q1 * q2 + q0 * q3), q0*q0 + q1*q1 - q2*q2 - q3*q3) * 57.3;
}


int main() {
    initLED();

    PWM_Init(7200,10);

    uart_init(72, 115200);

    MPU_init();

    SixAxis sourceData;

    wifi_init();

    MOTOR1 = 7190;
    MOTOR2 = 7190;

    delay(3000);

    while(1) {
        MPU6050_getStructData(&sourceData);
        Comput(sourceData);

        pid(0, sourceData.gX);
        uart_sendData(' ');
        uart_sendData('M');
        uart_sendData(':');

        uart_showData(MOTOR1);

        uart_sendData(' ');
        uart_sendData('r');
        uart_sendData(':');
        uart_Float2Char(Roll);

        uart_sendData(' ');
        uart_sendData('D');
        uart_sendData(':');
        uart_Float2Char(sourceData.gX);
        uart_sendData(0x0D);
        uart_sendData(0x0A);

    }
    while(1) {


        MPU6050_getStructData(&sourceData);

        Comput(sourceData);

        uart_sendData('P');
        uart_sendData('i');
        uart_sendData('t');
        uart_sendData('c');
        uart_sendData('h');
        uart_sendData(':');
        uart_sendData(' ');

        uart_Float2Char(Pitch);
        uart_sendData(' ');

        uart_sendData('R');
        uart_sendData('o');
        uart_sendData('l');
        uart_sendData('l');
        uart_sendData(':');
        uart_sendData(' ');

        uart_Float2Char(Roll);
        uart_sendData(' ');

        uart_sendData('Y');
        uart_sendData('a');
        uart_sendData('w');
        uart_sendData(':');
        uart_sendData(' ');

        uart_Float2Char(Yaw);

        uart_sendData(0x0D);
        uart_sendData(0x0A);

        delay(100);

    }
}
