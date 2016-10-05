#include "pid.h"
#include "stm32f10x.h"
#include "motor.h"

float g_iErro, g_sumErro = 0;
void pid(float setPoint, float d) {
    g_iErro = g_Pitch - setPoint;    //计算当前误差
    g_sumErro += g_iErro;       //对误差进行积分

    if(g_sumErro > SUM_ERRO_MAX) g_sumErro = SUM_ERRO_MAX;  //积分限幅
    else if(g_sumErro < SUM_ERRO_MIN) g_sumErro = SUM_ERRO_MIN;

    short resu = (short)(g_iErro * K_P + g_sumErro * K_I + d * K_D);  //PID输出

    if((MOTOR2 + resu) > MOTOR_MAX)
        MOTOR2 = MOTOR_MAX;
    else if((MOTOR2 + resu) < MOTOR_LOW)
        MOTOR2 = MOTOR_LOW;
    else MOTOR2 += resu;

    if((MOTOR4 + resu) > MOTOR_MAX)
        MOTOR4 = MOTOR_MAX;
    else if((MOTOR4 + resu) < MOTOR_LOW)
        MOTOR4 = MOTOR_LOW;
    else MOTOR4 += resu;
}
