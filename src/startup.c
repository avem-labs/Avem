#include "stm32f10x.h"
typedef void( *const intfunc )( void );
#define WEAK __attribute__ ((weak))
/* provided by the linker script */
//extern unsigned long _etext; /* start address of the static initialization data */
extern unsigned long _sidata; /* start address of the static initialization data */
extern unsigned long _sdata; /* start address of the data section */
extern unsigned long _edata; /* end address of the data section */
extern unsigned long _sbss; /* start address of the bss section */
extern unsigned long _ebss; /* end address of the bss section */
extern unsigned long _estack; /* end address of the stack section */

void Reset_Handler(void) __attribute__((__interrupt__));
void __Init_Data(void);
void Default_Handler(void);

extern int main(void);

void WEAK NMI_Handler(void);
void WEAK HardFault_Handler(void);
void WEAK MemManage_Handler(void);
void WEAK BusFault_Handler(void);
void WEAK UsageFault_Handler(void);
void WEAK MemManage_Handler(void);
void WEAK SVC_Handler(void);
void WEAK DebugMon_Handler(void);
void WEAK PendSV_Handler(void);
void WEAK SysTick_Handler(void);
void WEAK WWDG_IRQHandler(void);
void WEAK PVD_IRQHandler(void);
void WEAK TAMPER_IRQHandler(void);
void WEAK RTC_IRQHandler(void);
void WEAK FLASH_IRQHandler(void);
void WEAK RCC_IRQHandler(void);
void WEAK EXTI0_IRQHandler(void);
void WEAK EXTI1_IRQHandler(void);
void WEAK EXTI2_IRQHandler(void);
void WEAK EXTI3_IRQHandler(void);
void WEAK EXTI4_IRQHandler(void);
void WEAK DMA1_Channel1_IRQHandler(void);
void WEAK DMA1_Channel2_IRQHandler(void);
void WEAK DMA1_Channel3_IRQHandler(void);
void WEAK DMA1_Channel4_IRQHandler(void);
void WEAK DMA1_Channel5_IRQHandler(void);
void WEAK DMA1_Channel6_IRQHandler(void);
void WEAK DMA1_Channel7_IRQHandler(void);
void WEAK ADC1_2_IRQHandler(void);
void WEAK USB_HP_CAN1_TX_IRQHandler(void);
void WEAK USB_LP_CAN1_RX0_IRQHandler(void);
void WEAK CAN1_RX1_IRQHandler(void);
void WEAK CAN1_SCE_IRQHandler(void);
void WEAK EXTI9_5_IRQHandler(void);
void WEAK TIM1_BRK_IRQHandler(void);
void WEAK TIM1_UP_IRQHandler(void);
void WEAK TIM1_TRG_COM_IRQHandler(void);
void WEAK TIM1_CC_IRQHandler(void);
void WEAK TIM2_IRQHandler(void);
void WEAK TIM3_IRQHandler(void);
void WEAK TIM4_IRQHandler(void);
void WEAK I2C1_EV_IRQHandler(void);
void WEAK I2C1_ER_IRQHandler(void);
void WEAK I2C2_EV_IRQHandler(void);
void WEAK I2C2_ER_IRQHandler(void);
void WEAK SPI1_IRQHandler(void);
void WEAK SPI2_IRQHandler(void);
void WEAK USART1_IRQHandler(void);
void WEAK USART2_IRQHandler(void);
void WEAK USART3_IRQHandler(void);
void WEAK EXTI15_10_IRQHandler(void);
void WEAK RTCAlarm_IRQHandler(void);
void WEAK USBWakeUp_IRQHandler(void);
void WEAK TIM8_BRK_IRQHandler(void);
void WEAK TIM8_UP_IRQHandler(void);
void WEAK TIM8_TRG_COM_IRQHandler(void);
void WEAK TIM8_CC_IRQHandler(void);
void WEAK ADC3_IRQHandler(void);
void WEAK FSMC_IRQHandler(void);
void WEAK SDIO_IRQHandler(void);
void WEAK TIM5_IRQHandler(void);
void WEAK SPI3_IRQHandler(void);
void WEAK UART4_IRQHandler(void);
void WEAK UART5_IRQHandler(void);
void WEAK TIM6_IRQHandler(void);
void WEAK TIM7_IRQHandler(void);
void WEAK DMA2_Channel1_IRQHandler(void);
void WEAK DMA2_Channel2_IRQHandler(void);
void WEAK DMA2_Channel3_IRQHandler(void);
void WEAK DMA2_Channel4_5_IRQHandler(void);

__attribute__ ((section(".isr_vectors")))
void (* const g_pfnVectors[])(void) = {
 (intfunc)((unsigned long)&_estack), /* The stack pointer after relocation */
 Reset_Handler,
/* Reset Handler */
 NMI_Handler,
/* NMI Handler */
 HardFault_Handler,
/* Hard Fault Handler */
 MemManage_Handler,
/* MPU Fault Handler */
 BusFault_Handler,
/* Bus Fault Handler */
 UsageFault_Handler,
/* Usage Fault Handler */
 0,
/* Reserved */
 0,
/* Reserved */
 0,
/* Reserved */
 0,
/* Reserved */
 SVC_Handler,
/* SVCall Handler */
 DebugMon_Handler,
/* Debug Monitor Handler */
 0,
/* Reserved */
 PendSV_Handler,
/* PendSV Handler */
 SysTick_Handler,
/* SysTick Handler */
/* External Interrupts */
 WWDG_IRQHandler,
/* Window Watchdog */
 PVD_IRQHandler,
/* PVD through EXTI Line detect */
 TAMPER_IRQHandler,
/* Tamper */
 RTC_IRQHandler,
/* RTC */
 FLASH_IRQHandler,
/* Flash */
 RCC_IRQHandler,
/* RCC */
 EXTI0_IRQHandler,
/* EXTI Line 0 */
 EXTI1_IRQHandler,
/* EXTI Line 1 */
 EXTI2_IRQHandler,
/* EXTI Line 2 */
 EXTI3_IRQHandler,
/* EXTI Line 3 */
 EXTI4_IRQHandler,
/* EXTI Line 4 */
 DMA1_Channel1_IRQHandler, /* DMA1 Channel 1 */
 DMA1_Channel2_IRQHandler, /* DMA1 Channel 2 */
 DMA1_Channel3_IRQHandler, /* DMA1 Channel 3 */
 DMA1_Channel4_IRQHandler, /* DMA1 Channel 4 */
 DMA1_Channel5_IRQHandler, /* DMA1 Channel 5 */
 DMA1_Channel6_IRQHandler, /* DMA1 Channel 6 */
 DMA1_Channel7_IRQHandler, /* DMA1 Channel 7 */
 ADC1_2_IRQHandler,
/* ADC1 & ADC2 */
 USB_HP_CAN1_TX_IRQHandler, /* USB High Priority or CAN1 TX */
 USB_LP_CAN1_RX0_IRQHandler, /* USB Low Priority or CAN1 RX0 */
 CAN1_RX1_IRQHandler,
/* CAN1 RX1 */
 CAN1_SCE_IRQHandler,
/* CAN1 SCE */
 EXTI9_5_IRQHandler,
/* EXTI Line 9..5 */
 TIM1_BRK_IRQHandler,
/* TIM1 Break */
 TIM1_UP_IRQHandler,
/* TIM1 Update */
 TIM1_TRG_COM_IRQHandler, /* TIM1 Trigger and Commutation */
 TIM1_CC_IRQHandler,
/* TIM1 Capture Compare */
 TIM2_IRQHandler,
/* TIM2 */
 TIM3_IRQHandler,
/* TIM3 */
 TIM4_IRQHandler,
/* TIM4 */
 I2C1_EV_IRQHandler,
/* I2C1 Event */
 I2C1_ER_IRQHandler,
/* I2C1 Error */
 I2C2_EV_IRQHandler,
/* I2C2 Event */
 I2C2_ER_IRQHandler,
/* I2C2 Error */
 SPI1_IRQHandler,
/* SPI1 */
 SPI2_IRQHandler,
/* SPI2 */
 USART1_IRQHandler,
/* USART1 */
 USART2_IRQHandler,
/* USART2 */
 USART3_IRQHandler,
/* USART3 */
 EXTI15_10_IRQHandler,
/* EXTI Line 15..10 */
 RTCAlarm_IRQHandler,
/* RTC Alarm through EXTI Line */
 USBWakeUp_IRQHandler,
/* USB Wakeup from suspend */
 TIM8_BRK_IRQHandler,
 TIM8_UP_IRQHandler,
 TIM8_TRG_COM_IRQHandler,
 TIM8_CC_IRQHandler,
 ADC3_IRQHandler,
 FSMC_IRQHandler,
 SDIO_IRQHandler,
 TIM5_IRQHandler,
 SPI3_IRQHandler,
 UART4_IRQHandler,
 UART5_IRQHandler,
 TIM6_IRQHandler,
 TIM7_IRQHandler,
 DMA2_Channel1_IRQHandler,
 DMA2_Channel2_IRQHandler,
 DMA2_Channel3_IRQHandler,
 DMA2_Channel4_5_IRQHandler,
 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,
 0, 0, 0, 0, 0,
 0, 0, 0,
 (intfunc)0xF1E0F85F
/* @0x1E0. This is for boot in RAM mode for STM32F10x High Density devices. */
};
void __Init_Data(void) {
	unsigned long *src, *dst;
	/* copy the data segment into ram */
	src = &_sidata;
	dst = &_sdata;
	if (src != dst)
		while(dst < &_edata)
			*(dst++) = *(src++);
	/* zero the bss segment */
	dst = &_sbss;
	while(dst < &_ebss)
		*(dst++) = 0;
}
void Reset_Handler(void) {
	__Init_Data(); /* Initialize memory, data and bss */
	extern u32 _isr_vectors_offs; /* the offset to the vector table in ram */
	SCB->VTOR = 0x08000000 | ((u32)&_isr_vectors_offs & (u32)0x1FFFFF80); /* set interrupt vector table address */
	SystemInit(); /* configure the clock */
	main(); /* start execution of the program */
	while(1) {}
}
#pragma weak MMI_Handler	= Default_Handler
#pragma weak MemManage_Handler	= Default_Handler
#pragma weak BusFault_Handler	= Default_Handler
#pragma weak UsageFault_Handler	= Default_Handler
#pragma weak SVC_Handler	= Default_Handler
#pragma weak DebugMon_Handler	= Default_Handler
#pragma weak PendSV_Handler	= Default_Handler
#pragma weak SysTick_Handler	= Default_Handler
#pragma weak WWDG_IRQHandler	= Default_Handler
#pragma weak PVD_IRQHandler	= Default_Handler
#pragma weak TAMPER_IRQHandler	= Default_Handler
#pragma weak RTC_IRQHandler	= Default_Handler
#pragma weak FLASH_IRQHandler	= Default_Handler
#pragma weak RCC_IRQHandler	= Default_Handler
#pragma weak EXTI0_IRQHandler	= Default_Handler
#pragma weak EXTI1_IRQHandler	= Default_Handler

#pragma weak EXTI2_IRQHandler 	= Default_Handler
#pragma weak EXTI3_IRQHandler	= Default_Handler
#pragma weak EXTI4_IRQHandler	= Default_Handler
#pragma weak DMA1_Channel1_IRQHandler	= Default_Handler
#pragma weak DMA1_Channel2_IRQHandler	= Default_Handler
#pragma weak DMA1_Channel3_IRQHandler	= Default_Handler
#pragma weak DMA1_Channel4_IRQHandler	= Default_Handler
#pragma weak DMA1_Channel5_IRQHandler	= Default_Handler
#pragma weak DMA1_Channel6_IRQHandler	= Default_Handler
#pragma weak DMA1_Channel7_IRQHandler	= Default_Handler
#pragma weak ADC1_2_IRQHandler		= Default_Handler
#pragma weak USB_HP_CAN1_TX_IRQHandler	= Default_Handler
#pragma weak USB_LP_CAN1_RX0_IRQHandler	= Default_Handler
#pragma weak CAN1_RX1_IRQHandler 	= Default_Handler
#pragma weak CAN1_SCE_IRQHandler 	= Default_Handler
#pragma weak EXTI9_5_IRQHandler		= Default_Handler
#pragma weak TIM1_BRK_IRQHandler 	= Default_Handler
#pragma weak TIM1_UP_IRQHandler		= Default_Handler
#pragma weak TIM1_TRG_COM_IRQHandler	= Default_Handler
#pragma weak TIM1_CC_IRQHandler		= Default_Handler
#pragma weak TIM2_IRQHandler		= Default_Handler
#pragma weak TIM3_IRQHandler		= Default_Handler
#pragma weak TIM4_IRQHandler		= Default_Handler
#pragma weak I2C1_EV_IRQHandler		= Default_Handler
#pragma weak I2C1_ER_IRQHandler		= Default_Handler
#pragma weak I2C2_EV_IRQHandler		= Default_Handler
#pragma weak I2C2_ER_IRQHandler		= Default_Handler
#pragma weak SPI1_IRQHandler		= Default_Handler
#pragma weak SPI2_IRQHandler		= Default_Handler
#pragma weak USART1_IRQHandler		= Default_Handler
#pragma weak USART2_IRQHandler		= Default_Handler
#pragma weak USART3_IRQHandler		= Default_Handler
#pragma weak EXTI15_10_IRQHandler	= Default_Handler
#pragma weak RTCAlarm_IRQHandler	= Default_Handler
#pragma weak USBWakeUp_IRQHandler 	= Default_Handler
#pragma weak TIM8_BRK_IRQHandler 	= Default_Handler
#pragma weak TIM8_UP_IRQHandler		= Default_Handler
#pragma weak TIM8_TRG_COM_IRQHandler	= Default_Handler
#pragma weak TIM8_CC_IRQHandler		= Default_Handler
#pragma weak ADC3_IRQHandler		= Default_Handler
#pragma weak FSMC_IRQHandler		= Default_Handler
#pragma weak SDIO_IRQHandler		= Default_Handler
#pragma weak TIM5_IRQHandler		= Default_Handler
#pragma weak SPI3_IRQHandler		= Default_Handler
#pragma weak UART4_IRQHandler		= Default_Handler
#pragma weak UART5_IRQHandler		= Default_Handler
#pragma weak TIM6_IRQHandler		= Default_Handler
#pragma weak TIM7_IRQHandler		= Default_Handler
#pragma weak DMA2_Channel1_IRQHandler	= Default_Handler
#pragma weak DMA2_Channel2_IRQHandler	= Default_Handler
#pragma weak DMA2_Channel3_IRQHandler	= Default_Handler
#pragma weak DMA2_Channel4_5_IRQHandler	= Default_Handler

void Default_Handler(void)
{
while (1) {}
}
