#include <math.h>
#include "stm32f10x.h"
#include "bit.h"
#include "mpu6050.h"
#include "motor.h"



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

    initUART(72, 115200);

    MPU_init();

    SixAxis sourceData;

    MOTOR1 = 7190;
    MOTOR2 = 7190;
    delay(3000);
    while(1) {
        MPU6050_getStructData(&sourceData);
        Comput(sourceData);

        pid(0, sourceData.gX);
        sendData_uart(' ');
        sendData_uart('M');
        sendData_uart(':');

        showData(MOTOR1);

        sendData_uart(' ');
        sendData_uart('r');
        sendData_uart(':');
        Float2Char(Roll);

        sendData_uart(' ');
        sendData_uart('D');
        sendData_uart(':');
        Float2Char(sourceData.gX);
        sendData_uart(0x0D);
        sendData_uart(0x0A);

    }
    while(1) {


        MPU6050_getStructData(&sourceData);

        Comput(sourceData);

        sendData_uart('P');
        sendData_uart('i');
        sendData_uart('t');
        sendData_uart('c');
        sendData_uart('h');
        sendData_uart(':');
        sendData_uart(' ');

        Float2Char(Pitch);
        sendData_uart(' ');

        sendData_uart('R');
        sendData_uart('o');
        sendData_uart('l');
        sendData_uart('l');
        sendData_uart(':');
        sendData_uart(' ');

        Float2Char(Roll);
        sendData_uart(' ');

        sendData_uart('Y');
        sendData_uart('a');
        sendData_uart('w');
        sendData_uart(':');
        sendData_uart(' ');

        Float2Char(Yaw);

        sendData_uart(0x0D);
        sendData_uart(0x0A);

        delay(100);



    }
}
