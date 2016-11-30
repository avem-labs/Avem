#include "stm32f10x.h"
#include "pid.h"
#include "motor.h"

void pid_SingleAxis(pid_pst temp, float setPoint) {
    temp->Error = *temp->Feedback - setPoint;
//Outter Loop PID
    temp->i += temp->Error;
    if (temp->i > PID_IMAX) temp->i = PID_IMAX;
    else if (temp->i < PID_IMIN) temp->i = PID_IMIN;

    temp->d = *temp->Feedback - temp->OutterLast;

    temp->output = (short)(OUTTER_LOOP_KP * (temp->Error) + OUTTER_LOOP_KI * temp->i + OUTTER_LOOP_KD * temp->d);
    temp->OutterLast = *temp->Feedback; //Save Old Data
//Inner Loop PD
    temp->p = temp->output + *temp->Gyro * 3.5f;
    temp->d = *temp->Gyro - temp->InnerLast;
    temp->output = (short)(INNER_LOOP_KP * temp->p + INNER_LOOP_KD * temp->d);

    if (*temp->Channel1+temp->output > MOTOR_MAX) *temp->Channel1 = MOTOR_MAX;
    else if (*temp->Channel1+temp->output < MOTOR_LOW) *temp->Channel1 = MOTOR_LOW;
    else *temp->Channel1 += (short)temp->output;

    if (*temp->Channel2-temp->output > MOTOR_MAX) *temp->Channel2 = MOTOR_MAX;
    else if (*temp->Channel2-temp->output < MOTOR_LOW) *temp->Channel2 = MOTOR_LOW;
    else *temp->Channel2 -= (short)temp->output;

    temp->InnerLast = *temp->Gyro;
}

// float g_iErro, g_sumErro = 0;
// void pid(float setPoint, float d) {
//     g_iErro = g_Pitch - setPoint;    //计算当前误差
//     g_sumErro += g_iErro;       //对误差进行积分
//
//     if(g_sumErro > SUM_ERRO_MAX) g_sumErro = SUM_ERRO_MAX;  //积分限幅
//     else if(g_sumErro < SUM_ERRO_MIN) g_sumErro = SUM_ERRO_MIN;
//
//     short resu = (short)(g_iErro * K_P + g_sumErro * K_I + d * K_D);  //PID输出
//
//     if((MOTOR2) > MOTOR_MAX)
//         MOTOR2 = MOTOR_MAX;
//     else if((MOTOR2) < MOTOR_LOW)
//         MOTOR2 = MOTOR_LOW;
//     else MOTOR2 = resu;
//
//     if((MOTOR4) > MOTOR_MAX)
//         MOTOR4 = MOTOR_MAX;
//     else if((MOTOR4) < MOTOR_LOW)
//         MOTOR4 = MOTOR_LOW;
//     else MOTOR4 = resu;
// }
