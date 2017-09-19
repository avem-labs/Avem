#ifndef _AVM_PID_H_
#define _AVM_PID_H_

#define OUTTER_LOOP_KP 3 //0.257 * 0.83 0.255
#define OUTTER_LOOP_KI 1
#define OUTTER_LOOP_KD 0.3f

#define INNER_LOOP_KP 1
#define INNER_LOOP_KI 0
#define INNER_LOOP_KD 0.3f

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

extern pid_st avm_pid;

void pid_SingleAxis(pid_pst package, float setPoint);
void pid(float setPoint, float d);

#endif
