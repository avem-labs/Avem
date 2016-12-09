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
    temp->p = temp->output + *temp->Gyro;
    temp->d = *temp->Gyro - temp->InnerLast;
    temp->output = (short)(INNER_LOOP_KP * temp->p + INNER_LOOP_KD * temp->d);

    if (*temp->Channel1+temp->output > THROTTLE_MAX) *temp->Channel1 = THROTTLE_MAX;
    else if (*temp->Channel1+temp->output < THROTTLE_MIN) *temp->Channel1 = THROTTLE_MIN;
    else *temp->Channel1 += (short)temp->output;

    if (*temp->Channel2-temp->output > THROTTLE_MAX) *temp->Channel2 = THROTTLE_MAX;
    else if (*temp->Channel2-temp->output < THROTTLE_MIN) *temp->Channel2 = THROTTLE_MIN;
    else *temp->Channel2 -= (short)temp->output;

    temp->InnerLast = *temp->Gyro;
}
