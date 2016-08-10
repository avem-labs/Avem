////////////////////////////////////////////////////////////////////////////
//
//  File        :  stm32f103_cmsis.lsl
//
//  Version     :  @(#)stm32f103_cmsis.lsl      1.2  09/06/04
//
//  Description :  LSL file for the STMicroelectronics STM32F103, CMSIS version
//
//  Copyright 2009 Altium BV     
//
//  NOTE:
//      This file is derived from cm3.lsl and stm32f103.lsl.
//      It is assumed that the user works with the ARMv7M architecture.
//      Other architectures will not work with this lsl file.
//
////////////////////////////////////////////////////////////////////////////

//
// We do not want the vectors as defined in arm_arch.lsl
//
#define __NO_DEFAULT_AUTO_VECTORS       1
#define __NR_OF_VECTORS                 76


#ifndef __STACK
#  define __STACK                       8k
#endif
#ifndef __HEAP
#  define __HEAP                        2k
#endif
#ifndef __VECTOR_TABLE_ROM_ADDR
#  define __VECTOR_TABLE_ROM_ADDR       0x08000000
#endif
#ifndef __XVWBUF
#define __XVWBUF                        256             /* buffer used by CrossView */
#endif

#include <arm_arch.lsl>

////////////////////////////////////////////////////////////////////////////
//
// In the STM32F10x, 3 different boot modes can be selected
// - User Flash memory is selected as boot space
// - SystemMemory is selected as boot space
// - Embedded SRAM is selected as boot space
//
// This aliases the physical memory associated with each boot mode to Block
// 000 (0x00000000 boot memory). Even when aliased in the boot memory space,
// the related memory (Flash memory or SRAM) is still accessible at its
// original memory space.
//
// If no memory is defined yet use the following memory settings
//
#ifndef __MEMORY

memory stm32f103flash
{
    mau = 8;
    type = rom;
    size = 512k;
    map ( size = 512k, dest_offset=0x08000000, dest=bus:ARM:local_bus);
}

memory stm32f103ram
{
    mau = 8;
    type = ram;
    size = 64k;
    map ( size = 64k, dest_offset=0x20000000, dest=bus:ARM:local_bus);
}

#endif /* __MEMORY */


//
// Custom vector table defines interrupts according to CMSIS standard
//
# if defined(__CPU_ARMV7M__)
section_setup ::linear
{
                // vector table with handler addresses
                vector_table "vector_table" ( vector_size = 4, size = __NR_OF_VECTORS, run_addr = __VECTOR_TABLE_ROM_ADDR,
                                              template = ".text.handler_address",
                                              template_symbol = "_lc_vector_handler",
                                              vector_prefix = "_vector_",
                                              fill = loop,
                                              no_inline
                                            )
                {
                        vector ( id = 0, fill = "_lc_ub_stack" );                               // Top of Stack
                        vector ( id = 1, fill = "Reset_Handler" );                         /* Reset Handler                           */
                        vector( id = 2, optional, fill = "NMI_Handler" );  /* 2 Non Maskable Interrupt                */
                        vector ( id = 3, optional, fill = "HardFault_Handler" );
                        vector ( id = 4, optional, fill = "MemManage_Handler" );
                        vector ( id = 5, optional, fill = "BusFault_Handler" );
                        vector ( id = 6, optional, fill = "UsageFault_Handler" );
                        vector ( id = 11, optional, fill = "SVC_Handler" );
                        vector ( id = 12, optional, fill = "DebugMon_Handler" );
                        vector ( id = 14, optional, fill = "PendSV_Handler" );
                        vector ( id = 15, optional, fill = "SysTick_Handler" );

                        // External Interrupts :
                        vector ( id = 16, optional, fill = "WWDG_IRQHandler" );                 // Window Watchdog
                        vector ( id = 17, optional, fill = "PVD_IRQHandler" );                  // PVD through EXTI Line detect
                        vector ( id = 18, optional, fill = "TAMPER_IRQHandler" );               // Tamper
                        vector ( id = 19, optional, fill = "RTC_IRQHandler" );                  // RTC
                        vector ( id = 20, optional, fill = "FLASH_IRQHandler" );                // Flash
                        vector ( id = 21, optional, fill = "RCC_IRQHandler" );                  // RCC
                        vector ( id = 22, optional, fill = "EXTI0_IRQHandler" );                // EXTI Line 0
                        vector ( id = 23, optional, fill = "EXTI1_IRQHandler" );                // EXTI Line 1
                        vector ( id = 24, optional, fill = "EXTI2_IRQHandler" );                // EXTI Line 2
                        vector ( id = 25, optional, fill = "EXTI3_IRQHandler" );                // EXTI Line 3
                        vector ( id = 26, optional, fill = "EXTI4_IRQHandler" );                // EXTI Line 4
                        vector ( id = 27, optional, fill = "DMAChannel1_IRQHandler" );          // DMA Channel 1
                        vector ( id = 28, optional, fill = "DMAChannel2_IRQHandler" );          // DMA Channel 2
                        vector ( id = 29, optional, fill = "DMAChannel3_IRQHandler" );          // DMA Channel 3
                        vector ( id = 30, optional, fill = "DMAChannel4_IRQHandler" );          // DMA Channel 4
                        vector ( id = 31, optional, fill = "DMAChannel5_IRQHandler" );          // DMA Channel 5
                        vector ( id = 32, optional, fill = "DMAChannel6_IRQHandler" );          // DMA Channel 6
                        vector ( id = 33, optional, fill = "DMAChannel7_IRQHandler" );          // DMA Channel 7
                        vector ( id = 34, optional, fill = "ADC1_2_IRQHandler" );               // ADC1 and ADC2
                        vector ( id = 35, optional, fill = "USB_HP_CAN1_TX_IRQHandler" );        // USB High Priority or CAN1 TX
                        vector ( id = 36, optional, fill = "USB_LP_CAN1_RX0_IRQHandler" );       // USB LowPriority or CAN1 RX0
                        vector ( id = 37, optional, fill = "CAN_RX1_IRQHandler" );              // CAN RX1
                        vector ( id = 38, optional, fill = "CAN_SCE_IRQHandler" );              // CAN SCE
                        vector ( id = 39, optional, fill = "EXTI9_5_IRQHandler" );              // EXTI Line 9..5
                        vector ( id = 40, optional, fill = "TIM1_BRK_IRQHandler" );             // TIM1 Break
                        vector ( id = 41, optional, fill = "TIM1_UP_IRQHandler" );              // TIM1 Update
                        vector ( id = 42, optional, fill = "TIM1_TRG_COM_IRQHandler" );         // TIM1 Trigger and Commutation
                        vector ( id = 43, optional, fill = "TIM1_CC_IRQHandler" );              // TIM1 Capture Compare
                        vector ( id = 44, optional, fill = "TIM2_IRQHandler" );                 // TIM2
                        vector ( id = 45, optional, fill = "TIM3_IRQHandler" );                 // TIM3
                        vector ( id = 46, optional, fill = "TIM4_IRQHandler" );                 // TIM4
                        vector ( id = 47, optional, fill = "I2C1_EV_IRQHandler" );              // I2C1 Event
                        vector ( id = 48, optional, fill = "I2C1_ER_IRQHandler" );              // I2C1 Error
                        vector ( id = 49, optional, fill = "I2C2_EV_IRQHandler" );              // I2C2 Event
                        vector ( id = 50, optional, fill = "I2C2_ER_IRQHandler" );              // I2C2 Error
                        vector ( id = 51, optional, fill = "SPI1_IRQHandler" );                 // SPI1
                        vector ( id = 52, optional, fill = "SPI2_IRQHandler" );                 // SPI2
                        vector ( id = 53, optional, fill = "USART1_IRQHandler" );               // USART1
                        vector ( id = 54, optional, fill = "USART2_IRQHandler" );               // USART2
                        vector ( id = 55, optional, fill = "USART3_IRQHandler" );               // USART3
                        vector ( id = 56, optional, fill = "EXTI15_10_IRQHandler" );            // EXTI Line 15..10
                        vector ( id = 57, optional, fill = "RTCAlarm_IRQHandler" );             // RTC Alarm through EXTI Line
                        vector ( id = 58, optional, fill = "USBWakeUp_IRQHandler" );            // USB Wakeup from suspend
                        vector ( id = 59, optional, fill = "TIM8_BRK_IRQHandler" );             // TIM8 Break
                        vector ( id = 60, optional, fill = "TIM8_UP_IRQHandler" );              // TIM8 Update
                        vector ( id = 61, optional, fill = "TIM8_TRG_COM_IRQHandler" );         // TIM8 Trigger and Commutation
                        vector ( id = 62, optional, fill = "TIM8_CC_IRQHandler" );              // TIM8 Capture Compare
                        vector ( id = 63, optional, fill = "ADC3_IRQHandler" );                 // ADC3
                        vector ( id = 64, optional, fill = "FSMC_IRQHandler" );                 // FSMC
                        vector ( id = 65, optional, fill = "SDIO_IRQHandler" );                 // SDIO
                        vector ( id = 66, optional, fill = "TIM5_IRQHandler" );                 // TIM5
                        vector ( id = 67, optional, fill = "SPI3_IRQHandler" );                 // SPI3
                        vector ( id = 68, optional, fill = "UART4_IRQHandler" );                // UART4
                        vector ( id = 69, optional, fill = "UART5_IRQHandler" );                // UART5
                        vector ( id = 70, optional, fill = "TIM6_IRQHandler" );                 // TIM6
                        vector ( id = 71, optional, fill = "TIM7_IRQHandler" );                 // TIM7
                        vector ( id = 72, optional, fill = "DMA2_Channel1_IRQHandler" );        // DMA2 Channel1
                        vector ( id = 73, optional, fill = "DMA2_Channel2_IRQHandler" );        // DMA2 Channel2
                        vector ( id = 74, optional, fill = "DMA2_Channel3_IRQHandler" );        // DMA2 Channel3
                        vector ( id = 75, optional, fill = "DMA2_Channel4_5_IRQHandler" );      // DMA2 Channel4 and DMA2 Channel5
                }
}
# endif
