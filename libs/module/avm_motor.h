#ifndef _AVM_MOTOR_H_
#define _AVM_MOTOR_H_

extern avm_module_t avm_motor_module_st;

#define MOTOR_NORMAL_STARTUP


#define MOTOR1 TIM3->CCR1
#define MOTOR2 TIM3->CCR2
#define MOTOR3 TIM3->CCR3
#define MOTOR4 TIM3->CCR4

#define MOTOR_ALL MOTOR1 = MOTOR2 = MOTOR3 = MOTOR4

#define THROTTLE_MAX (unsigned short)3600	//2ms	-	top position of throttle
#define THROTTLE_MIN (unsigned short)1620	//0.9ms	-	bottom position of throttle
#define THROTTLE_MID (unsigned short)1650

#ifdef MOTOR_NORMAL_STARTUP
    #define MOTOR_SETTING() do {\
        motor_PWM_Init(36000,40);\
		MOTOR_ALL = THROTTLE_MIN;\
        delay_ms(4000);\
		MOTOR_ALL = THROTTLE_MID;\
    } while(0)
#else
    #define MOTOR_SETTING() do {\
        motor_PWM_Init(36000,40);\
		MOTOR_ALL = THROTTLE_MAX;\
        delay_ms(3000);\
		MOTOR_ALL = THROTTLE_MIN;\
        delay_ms(8000);\
    } while(0)
#endif


void motor_PWM_Init(unsigned short arr, unsigned short psc);  //72MHz / (arr + 1)*(psc + 1)

#endif
