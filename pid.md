# 串级PID
单极PID适合线性系统，当输出量和被控制量呈线性关系时单极PID能获得较好的效果，但是四轴不是线性系统，现代学者认为，四轴通常可以简化为一个二阶阻尼系统。为什么四轴不是线性系统呢？首先，输出的电压和电机转速不是呈正比的，其次，螺旋桨转速和升力是平方倍关系，故单极PID在四轴上很难取得很好效果，能飞，但是不好飞。
为了解决这个问题，我们提出了串级PID这个解决方法。

> 数字 PID 控制的实现必须用数值逼近的方法.
> 
> 当采样周期相当短时, 用求和代替积分, 用后向差分代替微分, 使模拟 PID 离散化变为差分方程.

> 数字 PID 算法总体分为位置型控制算法和增量型控制算法.

## 伪代码
> 外环PID

- 计算被控轴(*Pitch, Roll, Yaw*)角度误差 *(实际角度 - 期望角度)*

* *积分项*

	- 对误差结果进行累加 *(用求和代替积分)*
	- 对累加结果进行积分限幅

* *微分项*

	- 使用当前的角度值减去上一次的角度值 *(后向差分)*

* *外环输出*

	- 比例系数 **`P`** x 当前误差 + 积分系数 **`I`** x 积分项 + 微分系数 **`D`** x 积分项
- 保存上一次角度值

> 内环PD

* *比例项*

	- 外环输出 + 该轴角速度 * 3.5
* *微分项*

	- 该轴角速度 - 上一次该轴角速度 *(后向差分)*

* *内环输出*

	- 比例系数`p` x 比例项 + 微分系数`d` x 微分项

- 存储角速度

## Code

先定义结构体以及结构体指针, 把运算过程使用的大量变量封装起来, 在专属函数中对其解析和利用

``` c
typedef struct {
    float InnerLast;		//保存内环旧值以便后向差分
    float OutterLast;		//保存外环旧值以便后向差分
    float *Feedback;		//反馈数据, 实时的角度数据
    float *Gyro;				//角速度
    float Error;				//误差值
    float p;					//比例项(内环外环共用)
    float i;					//积分项(仅用于外环)
    float d;					//微分项(内环外环共用)
    short output;			//PID输出, 用来修改PWM值, 2字节
    __IO uint16_t *Channel1;//PWM输出, 通道1
    __IO uint16_t *Channel2;//PWM输出, 通道2
} pid_st, *pid_pst;
```

``` c
void pid_SingleAxis(pid_pst temp, float setPoint) {
    temp->Error = setPoint - *temp->Feedback;
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
    temp->output = (short)INNER_LOOP_KP * temp->p + INNER_LOOP_KD * temp->d;

    if (*temp->Channel1 + temp->output > MOTOR_MAX) *temp->Channel1 = MOTOR_MAX;
    else if (*temp->Channel1 + temp->output < MOTOR_LOW) *temp->Channel1 = MOTOR_LOW;
    else *temp->Channel1 += temp->output;

    if (*temp->Channel2 - temp->output > MOTOR_MAX) *temp->Channel2 = MOTOR_MAX;
    else if (*temp->Channel2 - temp->output < MOTOR_LOW) *temp->Channel2 = MOTOR_LOW;
    else *temp->Channel2 -= temp->output;

    temp->InnerLast = *temp->Gyro;
}
```
