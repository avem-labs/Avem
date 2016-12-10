#ifndef PID_H
#define PID_H

extern float g_Yaw, g_Pitch, g_Roll;     //eular

#define OUTTER_LOOP_KP 0 //0.257 * 0.83 0.255
#define OUTTER_LOOP_KI 0
#define OUTTER_LOOP_KD 0

#define INNER_LOOP_KP 0.03f
#define INNER_LOOP_KI 0
#define INNER_LOOP_KD 0

#define SUM_ERRO_MAX 900
#define SUM_ERRO_MIN -900

#define PID_IMAX 30
#define PID_IMIN -30

typedef struct {
    float InnerLast;
    float OutterLast;
    float *Feedback;
    float *Gyro;
    float Error;
    float p;
    float i;
    float d;
    short output;
    __IO uint16_t *Channel1;
    __IO uint16_t *Channel2;
} pid_st, *pid_pst;

void pid_SingleAxis(pid_pst package, float setPoint);
void pid(float setPoint, float d);

#endif
