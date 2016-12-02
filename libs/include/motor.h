#ifndef MOTOR_H
#define MOTOR_H

#define MOTOR_NORMAL_STARTUP


#define MOTOR1 TIM3->CCR1
#define MOTOR2 TIM3->CCR2
#define MOTOR3 TIM3->CCR3
#define MOTOR4 TIM3->CCR4

#define MOTOR_ALL MOTOR1 = MOTOR2 = MOTOR3 = MOTOR4

#define THROTTLE_MAX (unsigned short)3600	//2ms	-	top position of throttle
#define THROTTLE_MIN (unsigned short)1620	//0.9ms	-	bottom position of throttle
#define THROTTLE_MID (unsigned short)2200

#ifdef MOTOR_NORMAL_STARTUP
    #define MOTOR_SETTING() {\
        motor_PWM_Init(36000,40);\
		MOTOR_ALL = THROTTLE_MIN;\
        delay(4000);\
		MOTOR_ALL = THROTTLE_MID;\
    }
#else
    #define MOTOR_SETTING() {\
        motor_PWM_Init(36000,40);\
		MOTOR_ALL = THROTTLE_MAX;\
        delay(3000);\
		MOTOR_ALL = THROTTLE_MIN;\
        delay(8000);\
    }
#endif

void motor_PWM_Init(unsigned short arr, unsigned short psc);  //72MHz / (arr + 1)*(psc + 1)

#endif
