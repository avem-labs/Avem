#pragma once
#ifndef PID_H_
#define PID_H_

extern float g_Yaw, g_Pitch, g_Roll;     //eular

#define K_P 2.57f //0.257 * 0.83 0.255
#define K_I 0
#define K_D 0
#define SUM_ERRO_MAX 900
#define SUM_ERRO_MIN -900

typedef struct {
    float Cache;
    float Erro;
    float p;
    float i;
    float d;
    short output;
} pid_st, *pid_pst;

void pid(float setPoint, float d);

#endif
