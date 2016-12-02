#ifndef MOTOR_H
#define MOTOR_H

#define MOTOR_NORMAL_STARTUP


#define MOTOR1 TIM3->CCR1
#define MOTOR2 TIM3->CCR2
#define MOTOR3 TIM3->CCR3
#define MOTOR4 TIM3->CCR4

#define MOTOR_MAX (unsigned short)3600
#define MOTOR_MIN (unsigned short)1620
#define MOTOR_LOW (unsigned short)1620

#ifdef MOTOR_NORMAL_STARTUP
    #define MOTOR_SETTING() {\
        motor_PWM_Init(36000,40);\
		MOTOR1 = MOTOR_MIN;\
        MOTOR2 = MOTOR_MIN;\
        MOTOR3 = MOTOR_MIN;\
        MOTOR4 = MOTOR_MIN;\
        delay(10000);\
    }
#else
    #define MOTOR_SETTING() {\
        motor_PWM_Init(36000,40);\
        MOTOR1 = MOTOR_MAX;\
        MOTOR2 = MOTOR_MAX;\
        MOTOR3 = MOTOR_MAX;\
        MOTOR4 = MOTOR_MAX;\
        delay(3000);\
        MOTOR1 = MOTOR_MIN;\
        MOTOR2 = MOTOR_MIN;\
        MOTOR3 = MOTOR_MIN;\
        MOTOR4 = MOTOR_MIN;\
        delay(8000);\
    }
#endif

void motor_PWM_Init(unsigned short arr, unsigned short psc);  //72MHz / (arr + 1)*(psc + 1)

#endif
