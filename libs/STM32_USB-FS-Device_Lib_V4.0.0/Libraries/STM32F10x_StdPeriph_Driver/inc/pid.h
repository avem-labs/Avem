#pragma once
#ifndef PID_H_
#define PID_H_

extern float g_Yaw, g_Pitch, g_Roll;     //eular

#define K_P 2.57f //0.257 * 0.83 0.255
#define K_I 0
#define K_D 0
#define SUM_ERRO_MAX 900
#define SUM_ERRO_MIN -900

#define PID_IMAX 300
#define PID_IMIN -300

#define KP 1
#define KI 1
#define KD 1

typedef struct {
    float Last;
    float *Feedback;
    float Erro;
    float p;
    float i;
    float d;
    short output;
} pid_st, *pid_pst;

void pid_SingleAxis(pid_pst package, float setPoint);
void pid(float setPoint, float d);

#endif
