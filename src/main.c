#include <math.h>
#include "stm32f10x.h"
#include "bit.h"
#include "mpu6050.h"
#include "motor.h"
#include "uart.h"
#include "wifi.h"
#include "pid.h"

#define Kp      100.0f      //比例增益支配率(常量)
#define Ki      0.002f      //积分增益支配率
#define halfT   0.001f      //采样周期的一半

float g_q0 = 1, g_q1 = 0, g_q2 = 0, g_q3 = 0;   //Quaternion
float g_exInt = 0, g_eyInt = 0, g_ezInt = 0;
float g_Yaw, g_Pitch, g_Roll;

// float Cache;
// float *RealTime;
// float Erro;
// float p;
// float i;
// float d;
// short output;
pid_st g_pid_pitch = {
    .Cache = 0.01,
    .RealTime = &g_Yaw,
    .p = 0,
    .i = 0,
    .d = 0
};

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

#define DEBUG_MPU6050_SOURCEDATA

int main() {
#if defined (DEBUG_PID) || defined (DEBUG_MPU6050_EULER) || defined (DEBUG_MPU6050_SOURCEDATA) || defined (DEBUG_BLDC)
    SixAxis sourceData;
#endif

//Brushless motor auto init
#ifdef DEBUG_BLDC
    MOTOR_SETTING();
#endif

    uart_init(72, 115200);
    uart_sendStr("Config MPU6050...");
    UART_CR();
    MPU_init();
    uart_sendStr("MPU6050 Connect Success!");
    UART_CR();



    while(1) {
#ifdef DEBUG_WIFI
        wifi_Config();
         while(1) {
             wifi_sendCmd("AT+CIPSEND=0,20");
             delay(50);
             wifi_sendCmd("<html>aki<br></html>");
             delay(1000);
         }
#endif
#ifdef DEBUG_MPU6050_SOURCEDATA
        MPU6050_getStructData(&sourceData);
        MPU6050_debug(&sourceData);
#endif

#if defined (DEBUG_PID) || defined (DEBUG_BLDC)
        MPU6050_getStructData(&sourceData);
        Comput(sourceData);

        pid(0, sourceData.gX);

        uart_sendStr(" Motor:");
        uart_showData(MOTOR1);

        uart_sendStr(" , Roll:");
        uart_Float2Char(g_Roll);

        uart_sendStr(" D: ");
        uart_Float2Char(sourceData.gX);
        UART_CR();
#endif

#ifdef DEBUG_MPU6050_EULER
        MPU6050_getStructData(&sourceData);

        Comput(sourceData);


        uart_sendStr("Pitch Angle: ");
        uart_Float2Char(g_Pitch);


        uart_sendStr("; Roll Angle: ");
        uart_Float2Char(g_Roll);

        uart_sendStr("; Yaw Angle: ");
        uart_Float2Char(g_Yaw);

        UART_CR();

        delay(100);
#endif
    }
}
