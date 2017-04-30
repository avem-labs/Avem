#include "FreeRTOS.h"
#include "task.h"

#include <math.h>
#include "stm32f10x.h"
#include "bit.h"
#include "mpu6050.h"
#include "motor.h"
#include "uart.h"
#include "wifi.h"
#include "pid.h"
#include "tty.h"


double _asin(double i) {return asin(i);}
double _atan2(double i,double k) {return atan2(i,k);}
double _sqrt(double i) {return sqrt(i);}


//ms
void delay(unsigned int t) {
	SysTick->LOAD = 9000 * t;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x01;
	for(unsigned int tmp = SysTick->CTRL;(tmp&0x01)&&(!(tmp&SysTick_CTRL_COUNTFLAG));tmp = SysTick->CTRL);
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
}


#define DEBUG_BLDC		//Config

#if defined (DEBUG_MPU6050_EULER) || defined (DEBUG_MPU6050_SOURCEDATA) || defined (DEBUG_BLDC)
    SixAxis sourceData;
#endif

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
#ifdef DEBUG_BLDC
    pid_st g_pid_roll = {
        .InnerLast  = 0,
        .OutterLast = 0,
        .Feedback   = &g_Roll,
        .i          = 0,
        .Channel1   = &MOTOR2,
        .Channel2   = &MOTOR4,
        .Gyro       = &sourceData.gX,
    };
#endif

void mpu_task() {
	while(1) {
		MPU6050_getStructData(&sourceData);
        IMU_Comput(sourceData);
		vTaskDelay(10);
	}
}

#ifdef DEBUG_BLDC
	void pid_task() {
		while(1) {
			pid_SingleAxis(&g_pid_roll, 0);
			vTaskDelay(10);
		}
	}
#endif

void uart_task() {
	while(1) {
		uart_sendStr("Pitch Angle: ");
        uart_Float2Char(g_Pitch);

        uart_sendStr("; Roll Angle: ");
        uart_Float2Char(g_Roll);

        uart_sendStr("; Yaw Angle: ");
        uart_Float2Char(g_Yaw);

        UART_CR();

		vTaskDelay(100);
	}
}

void uart_debugPID() {
	while(1) {
		TTY_CLEAR();

		TTY_RED();
		uart_sendStr(" Motor占空比: ");
		TTY_NONE();
		TTY_BLUE();
		uart_showData(*g_pid_roll.Channel1);
		uart_sendStr("\t");
		uart_showData(*g_pid_roll.Channel2);
		TTY_NONE();

		uart_sendStr("\n\rRoll:\t");
		uart_Float2Char(*g_pid_roll.Feedback);

		uart_sendStr("\tGyro:\t");
		uart_Float2Char(*g_pid_roll.Gyro);

		uart_sendStr("\n\rP:\t");
		uart_Float2Char(g_pid_roll.p);

		uart_sendStr("\n\rI:\t");
		uart_Float2Char(g_pid_roll.i);

		uart_sendStr("\n\rD:\t");
		uart_Float2Char(g_pid_roll.d);

		uart_sendStr("\n\r=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=_=\n\rInner Cache:\t");
		uart_Float2Char(g_pid_roll.InnerLast);
		uart_sendStr("\n\rOutter Cache:\t");
		uart_Float2Char(g_pid_roll.OutterLast);

		uart_sendStr("\n\rOutput:\t\t");
		TTY_RED();
		uart_showData(g_pid_roll.output);
		TTY_NONE();
		uart_sendStr("\n\r");
		vTaskDelay(100);
	}
}

int main() {
	#ifdef DEBUG_BLDC
		//Brushless motor auto init
	    MOTOR_SETTING();
	#endif

    uart_init(72, 115200);
    uart_sendStr("Config MPU6050...");
    UART_CR();
    MPU_init();
    uart_sendStr("MPU6050 Connect Success!");
    UART_CR();

	// xTaskCreate(uart_task, "UART_TASK", 100, NULL, 1, NULL);
	xTaskCreate(mpu_task, "MPU_TASK", 100, NULL, 3, NULL);
	xTaskCreate(pid_task, "PID_TASK", 100, NULL, 2, NULL);
	vTaskStartScheduler();
	uart_sendStr("Stack Overflow...");
	while(1);
}
