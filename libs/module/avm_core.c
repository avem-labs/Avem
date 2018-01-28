#include <avm_core.h>

static unsigned char avm_core_init(void *arg);

avm_module_t avm_core_module_st = {
	0,
	NULL,
	avm_core_init,
	NULL,
	NULL
};

void jump2ISP() {
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
	__set_MSP(*(unsigned int *)ISP_ADDR);
	((void (*)(void))*((unsigned int *)(ISP_ADDR + 4)))();
}

void delay_ms(unsigned int t) {
	SysTick->LOAD = 9000 * t;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x01;
	for(unsigned int tmp = SysTick->CTRL;(tmp&0x01)&&(!(tmp&SysTick_CTRL_COUNTFLAG));tmp = SysTick->CTRL);
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
}

// 延时函数, 单位为微秒
void delay_us(unsigned int t) {
	SysTick->LOAD = 9 * t;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x01;
	for(unsigned int tmp = SysTick->CTRL;(tmp&0x01)&&(!(tmp&SysTick_CTRL_COUNTFLAG));tmp = SysTick->CTRL);
	SysTick->CTRL = 0;
	SysTick->VAL = 0;
}

avm_module_t *avm_modules_pool[] = {
	&avm_core_module_st,
    &avm_uart_module_st,
	&avm_i2c_module_st,
	&avm_motor_module_st,
	&avm_mpu_module_st,
    NULL
};

static unsigned char avm_core_init(void *arg) {
	NVIC_SetPriorityGrouping(0x07 - NVIC_GROUPING);

	avm_module_t *p_core = avm_modules_pool[0];
	// Count
	static unsigned char avm_modules_num = 0;
	for(;avm_modules_pool[avm_modules_num] != NULL;avm_modules_num++) {
		avm_modules_pool[avm_modules_num]->index = avm_modules_num;
	}

	p_core->confs = (void *)&avm_modules_num;	// Stored
	// Initial
	avm_module_t **this = &avm_modules_pool[1];
	for(;*this != NULL;this++) {
			(*this)->init_module((*this)->confs);
	}

	return 0;
}
