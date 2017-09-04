#ifndef _AVM_CORE_H_
#define _AVM_CORE_H_

typedef struct avm_module_s     avm_module_t;
// typedef struct avm_string_s     avm_string_t;

#include <math.h>
#include <stm32f10x.h>
// #include <avm_string.h>
#include <avm_bit.h>
#include <avm_mpu6050.h>
#include <avm_motor.h>
#include <avm_uart.h>
#include <avm_wifi.h>
#include <avm_pid.h>
#include <avm_i2c.h>
#include <avm_tty.h>

struct avm_module_s {
    unsigned short  index;
    unsigned int    *confs;
    unsigned char   (*init_module)(void *arg);
    unsigned char   (*enable_module)(void);
    unsigned char   (*disable_module)(void);
};

extern avm_module_t *avm_modules_pool[];
extern avm_module_t avm_core_module_st;

void delay_ms(unsigned int t);
void delay_us(unsigned int t);

#define NVIC_GROUPING	3
// Jump to built-in bootloader
#define ISP_ADDR		0x1FFFF000
void jump2ISP();


#endif