#include <math.h>
#include "stm32f10x.h"
#include "bit.h"
#include "mpu6050.h"
#include "motor.h"
#include "uart.h"
#include "wifi.h"
#include "key.h"

#define Kp      100.0f      //比例增益支配率(常量)
#define Ki      0.002f      //积分增益支配率
#define halfT   0.001f      //采样周期的一半

float g_q0 = 1, g_q1 = 0, g_q2 = 0, g_q3 = 0;   //Quaternion
float g_exInt = 0, g_eyInt = 0, g_ezInt = 0;
float g_Yaw, g_Pitch, g_Roll;     //eular

#define K_P 2.57f //0.257 * 0.83 0.255
#define K_I 0
#define K_D 0
#define SUM_ERRO_MAX 900
#define SUM_ERRO_MIN -900


float g_iErro, g_sumErro = 0;
void pid(float setPoint, float d) {
    g_iErro = g_Roll - setPoint;    //计算当前误差
    g_sumErro += g_iErro;       //对误差进行积分

    if(g_sumErro > SUM_ERRO_MAX) g_sumErro = SUM_ERRO_MAX;  //积分限幅
    else if(g_sumErro < SUM_ERRO_MIN) g_sumErro = SUM_ERRO_MIN;

    short resu = (short)(g_iErro * K_P + g_sumErro * K_I + d * K_D);  //PID输出


    if((MOTOR1 + resu) > MOTOR_MAX)
        MOTOR1 = MOTOR_MAX;
    else if((MOTOR1 + resu) < MOTOR_MIN)
        MOTOR1 = MOTOR_MIN;
    else MOTOR1 += resu;
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
    vx = 2 * (g_q1 * g_q3 - g_q0 * g_q2);
    vy = 2 * (g_q0 * g_q1 + g_q2 * g_q3);
    vz = g_q0*g_q0 - g_q1*g_q1 - g_q2*g_q2 + g_q3*g_q3;

    //错误的领域和方向传感器测量参考方向几件的交叉乘积的总和
    ex = (cache.aY * vz - cache.aZ * vy);
    ey = (cache.aZ * vx - cache.aX * vz);
    ez = (cache.aX * vy - cache.aY * vx);

    //积分误差比例积分增益
    g_exInt += ex * Ki;
    g_eyInt += ey * Ki;
    g_ezInt += ez * Ki;

    //调整后的陀螺仪测量
    cache.gX += Kp * ex + g_exInt;
    cache.gY += Kp * ey + g_eyInt;
    cache.gZ += Kp * ez + g_ezInt;

    //整合四元数率和正常化
    g_q0 += (-g_q1 * cache.gX - g_q2 * cache.gY - g_q3 * cache.gZ) * halfT;
    g_q1 += (g_q0 * cache.gX + g_q2 * cache.gZ - g_q3 * cache.gY) * halfT;
    g_q2 += (g_q0 * cache.gY - g_q1 * cache.gZ + g_q3 * cache.gX) * halfT;
    g_q3 += (g_q0 * cache.gZ + g_q1 * cache.gY - g_q2 * cache.gX) * halfT;

    //正常化四元
    norm = sqrt(g_q0*g_q0 + g_q1*g_q1 + g_q2*g_q2 + g_q3*g_q3);
    g_q0 = g_q0 / norm;
    g_q1 = g_q1 / norm;
    g_q2 = g_q2 / norm;
    g_q3 = g_q3 / norm;

    g_Pitch = asin(-2 * g_q1 * g_q3 + 2 * g_q0 * g_q2) * 57.3;
    g_Roll = atan2(2 * g_q2 * g_q3 + 2 * g_q0 * g_q1, -2 * g_q1*g_q1 - 2 * g_q2*g_q2 + 1) * 57.3;
    g_Yaw = atan2(2 * (g_q1 * g_q2 + g_q0 * g_q3), g_q0*g_q0 + g_q1*g_q1 - g_q2*g_q2 - g_q3*g_q3) * 57.3;
}

//#define DEBUG_WIFI
#define DEBUG_MPU6050_EULER     //Do not comment this, even not work
int main() {

    SixAxis sourceData;
    wifi_Config();

#ifdef DEBUG_WIFI
     while(1) {
         wifi_sendCmd("AT+CIPSEND=0,20");
         delay(50);
         wifi_sendCmd("<html>aki<br></html>");
         delay(1000);
     }
#endif

    initLED();

//Brushless motor auto init
    MOTOR_SETTING();

    uart_init(72, 115200);
    MPU_init();




    while(1) {
#ifdef DEBUG_MPU6050_SOURCEDATA
        MPU6050_getStructData(&sourceData);
        MPU6050_debug(&sourceData);
#endif

#ifdef DEBUG_PID
        MPU6050_getStructData(&sourceData);
        Comput(sourceData);

        pid(0, sourceData.gX);

        uart_sendStr(" Motor:");
        uart_showData(MOTOR1);

        uart_sendStr(" , Roll:");
        uart_Float2Char(g_Roll);

        uart_sendStr(" D: ");
        uart_Float2Char(sourceData.gX);
        uart_sendData(0x0D);
        uart_sendData(0x0A);

#endif

#ifdef DEBUG_MPU6050_EULER
        MPU6050_getStructData(&sourceData);

        Comput(sourceData);


        uart_sendStr(" Pitch Angle: ");
        uart_Float2Char(g_Pitch);


        uart_sendStr("; Roll Angle: ");
        uart_Float2Char(g_Roll);

        uart_sendStr("; Yaw Angle: ");
        uart_Float2Char(g_Yaw);

        uart_sendData(0x0D);
        uart_sendData(0x0A);

        delay(100);
#endif
    }
}
