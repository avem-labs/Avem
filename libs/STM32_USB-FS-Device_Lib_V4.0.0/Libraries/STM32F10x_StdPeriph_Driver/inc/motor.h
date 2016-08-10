#pragma once
#ifndef _MOTOR_H_
#define _MOTOR_H_


#define MOTOR1 TIM3->CCR2

void PWM_Init(unsigned short arr, unsigned short psc);  //72MHz / (arr + 1)*(psc + 1)

#endif
