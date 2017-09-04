#include <avm_core.h>

// float InnerLast;			//保存内环旧值以便后向差分
// float OutterLast;		//保存外环旧值以便后向差分
// float *Feedback;			//反馈数据, 实时的角度数据
// float *Gyro;				//角速度
// float Error;				//误差值
// float p;					//比例项(内环外环共用)
// float i;					//积分项(仅用于外环)
// float d;					//微分项(内环外环共用)
// short output;			//PID输出, 用来修改PWM值, 2字节
// __IO uint16_t *Channel1;	//PWM输出, 通道1
// __IO uint16_t *Channel2;	//PWM输出, 通道2

pid_st avm_pid = {
    .InnerLast  = 0,
    .OutterLast = 0,
    .Feedback   = &g_Roll,
    .p          = 0,
    .i          = 0,
    .d          = 0,
    .Channel1   = &MOTOR2,
    .Channel2   = &MOTOR4,
    .Gyro       = &avm_euler.gX,
};

void pid_SingleAxis(pid_pst temp, float setPoint) {
    temp->Error = *temp->Feedback - setPoint;
//Outter Loop PID
    temp->i += temp->Error;
    if (temp->i > PID_IMAX)
		temp->i = PID_IMAX;
    else if (temp->i < PID_IMIN)
		temp->i = PID_IMIN;

    temp->d = *temp->Feedback - temp->OutterLast;

    temp->output = (short)(OUTTER_LOOP_KP * (temp->Error) + OUTTER_LOOP_KI * temp->i + OUTTER_LOOP_KD * temp->d);
    temp->OutterLast = *temp->Feedback; //Save Old Data
//Inner Loop PD
    temp->p = temp->output + *temp->Gyro;
    temp->d = *temp->Gyro - temp->InnerLast;
    temp->output = (short)(INNER_LOOP_KP * temp->p + INNER_LOOP_KD * temp->d);

    if (*temp->Channel1+temp->output > THROTTLE_MAX)
		*temp->Channel1 = THROTTLE_MAX;
    else if (*temp->Channel1+temp->output < THROTTLE_MIN)
		*temp->Channel1 = THROTTLE_MIN;
    else
		*temp->Channel1 += (short)temp->output;

    if (*temp->Channel2-temp->output > THROTTLE_MAX)
		*temp->Channel2 = THROTTLE_MAX;
    else if
		(*temp->Channel2-temp->output < THROTTLE_MIN) *temp->Channel2 = THROTTLE_MIN;
    else
		*temp->Channel2 -= (short)temp->output;

    temp->InnerLast = *temp->Gyro;
}
