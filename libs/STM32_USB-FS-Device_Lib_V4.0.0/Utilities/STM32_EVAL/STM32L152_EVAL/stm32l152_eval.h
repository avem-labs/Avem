/**
  ******************************************************************************
  * @file    stm32l152_eval.h
  * @author  MCD Application Team
  * @version V5.0.2
  * @date    09-March-2012
  * @brief   This file contains definitions for STM32L152_EVAL's Leds, push-buttons
  *          and COM ports hardware resources.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2012 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L152_EVAL_H
#define __STM32L152_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"
#include "stm32_eval_legacy.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */

/** @addtogroup STM32L152_EVAL
  * @{
  */
      
/** @addtogroup STM32L152_EVAL_LOW_LEVEL
  * @{
  */ 

/** @defgroup STM32L152_EVAL_LOW_LEVEL_Exported_Types
  * @{
  */
typedef enum 
{
  LED1 = 0,
  LED2 = 1,
  LED3 = 2,
  LED4 = 3
} Led_TypeDef;

typedef enum 
{
  BUTTON_KEY = 0,
  BUTTON_RIGHT = 1,
  BUTTON_LEFT = 2,
  BUTTON_UP = 3,
  BUTTON_DOWN = 4,
  BUTTON_SEL = 5
} Button_TypeDef;

typedef enum 
{  
  BUTTON_MODE_GPIO = 0,
  BUTTON_MODE_EXTI = 1
} ButtonMode_TypeDef;

typedef enum 
{ 
  JOY_NONE = 0,
  JOY_SEL = 1,
  JOY_DOWN = 2,
  JOY_LEFT = 3,
  JOY_RIGHT = 4,
  JOY_UP = 5
} JOYState_TypeDef
;

typedef enum 
{
  COM1 = 0,
  COM2 = 1
} COM_TypeDef;  
/**
  * @}
  */ 

/** @defgroup STM32L152_EVAL_LOW_LEVEL_Exported_Constants
  * @{
  */ 

/** 
  * @brief  Define for STM32L152_EVAL board  
  */ 
#if !defined (USE_STM32L152_EVAL)
 #define USE_STM32L152_EVAL
#endif

/** @addtogroup STM32L152_EVAL_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             4

#define LED1_PIN                         GPIO_Pin_0
#define LED1_GPIO_PORT                   GPIOD
#define LED1_GPIO_CLK                    RCC_AHBPeriph_GPIOD  
  
#define LED2_PIN                         GPIO_Pin_1
#define LED2_GPIO_PORT                   GPIOD
#define LED2_GPIO_CLK                    RCC_AHBPeriph_GPIOD  
  
#define LED3_PIN                         GPIO_Pin_4
#define LED3_GPIO_PORT                   GPIOD
#define LED3_GPIO_CLK                    RCC_AHBPeriph_GPIOD  
  
#define LED4_PIN                         GPIO_Pin_5
#define LED4_GPIO_PORT                   GPIOD
#define LED4_GPIO_CLK                    RCC_AHBPeriph_GPIOD

/**
  * @}
  */ 
  
/** @addtogroup STM32L152_EVAL_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                          6 
/* On STM32L152-EVAL board, the KEY button is connected to PA.00 and it can
   be use as Wakeup pin button. */

/**
 * @brief Key push-button
 */
#define KEY_BUTTON_PIN                   GPIO_Pin_0
#define KEY_BUTTON_GPIO_PORT             GPIOA
#define KEY_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOA
#define KEY_BUTTON_EXTI_LINE             EXTI_Line0
#define KEY_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOA
#define KEY_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource0
#define KEY_BUTTON_EXTI_IRQn             EXTI0_IRQn

/**
 * @brief Joystick Right push-button
 */
#define RIGHT_BUTTON_PIN                 GPIO_Pin_11
#define RIGHT_BUTTON_GPIO_PORT           GPIOE
#define RIGHT_BUTTON_GPIO_CLK            RCC_AHBPeriph_GPIOE
#define RIGHT_BUTTON_EXTI_LINE           EXTI_Line11
#define RIGHT_BUTTON_EXTI_PORT_SOURCE    EXTI_PortSourceGPIOE
#define RIGHT_BUTTON_EXTI_PIN_SOURCE     EXTI_PinSource11
#define RIGHT_BUTTON_EXTI_IRQn           EXTI15_10_IRQn

/**
 * @brief Joystick Left push-button
 */
#define LEFT_BUTTON_PIN                  GPIO_Pin_12
#define LEFT_BUTTON_GPIO_PORT            GPIOE
#define LEFT_BUTTON_GPIO_CLK             RCC_AHBPeriph_GPIOE
#define LEFT_BUTTON_EXTI_LINE            EXTI_Line12
#define LEFT_BUTTON_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOE
#define LEFT_BUTTON_EXTI_PIN_SOURCE      EXTI_PinSource12
#define LEFT_BUTTON_EXTI_IRQn            EXTI15_10_IRQn  

/**
 * @brief Joystick Up push-button
 */
#define UP_BUTTON_PIN                    GPIO_Pin_9
#define UP_BUTTON_GPIO_PORT              GPIOE
#define UP_BUTTON_GPIO_CLK               RCC_AHBPeriph_GPIOE
#define UP_BUTTON_EXTI_LINE              EXTI_Line9
#define UP_BUTTON_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOE
#define UP_BUTTON_EXTI_PIN_SOURCE        EXTI_PinSource9
#define UP_BUTTON_EXTI_IRQn              EXTI9_5_IRQn  

/**
 * @brief Joystick Down push-button
 */  
#define DOWN_BUTTON_PIN                  GPIO_Pin_10
#define DOWN_BUTTON_GPIO_PORT            GPIOE
#define DOWN_BUTTON_GPIO_CLK             RCC_AHBPeriph_GPIOE
#define DOWN_BUTTON_EXTI_LINE            EXTI_Line10
#define DOWN_BUTTON_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOE
#define DOWN_BUTTON_EXTI_PIN_SOURCE      EXTI_PinSource10
#define DOWN_BUTTON_EXTI_IRQn            EXTI15_10_IRQn  

/**
 * @brief Joystick Sel push-button
 */
#define SEL_BUTTON_PIN                   GPIO_Pin_8
#define SEL_BUTTON_GPIO_PORT             GPIOE
#define SEL_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOE
#define SEL_BUTTON_EXTI_LINE             EXTI_Line8
#define SEL_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOE
#define SEL_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource8
#define SEL_BUTTON_EXTI_IRQn             EXTI9_5_IRQn 

/**
  * @}
  */ 

/** @addtogroup STM32L152_EVAL_LOW_LEVEL_COM
  * @{
  */
#define COMn                             2

/**
 * @brief Definition for COM port1, connected to USART2
 */ 
#define EVAL_COM1                        USART2
#define EVAL_COM1_CLK                    RCC_APB1Periph_USART2

#define EVAL_COM1_TX_PIN                 GPIO_Pin_5
#define EVAL_COM1_TX_GPIO_PORT           GPIOD
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHBPeriph_GPIOD
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource5
#define EVAL_COM1_TX_AF                  GPIO_AF_USART2

#define EVAL_COM1_RX_PIN                 GPIO_Pin_6
#define EVAL_COM1_RX_GPIO_PORT           GPIOD
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHBPeriph_GPIOD
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource6
#define EVAL_COM1_RX_AF                  GPIO_AF_USART2

#define EVAL_COM1_RTS_PIN                GPIO_Pin_4
#define EVAL_COM1_RTS_GPIO_PORT          GPIOD
#define EVAL_COM1_RTS_GPIO_CLK           RCC_AHBPeriph_GPIOD
#define EVAL_COM1_RTS_SOURCE             GPIO_PinSource4
#define EVAL_COM1_RTS_AF                 GPIO_AF_USART2

#define EVAL_COM1_CTS_PIN                GPIO_Pin_3
#define EVAL_COM1_CTS_GPIO_PORT          GPIOD
#define EVAL_COM1_CTS_GPIO_CLK           RCC_AHBPeriph_GPIOD
#define EVAL_COM1_CTS_SOURCE             GPIO_PinSource3
#define EVAL_COM1_CTS_AF                 GPIO_AF_USART2

#define EVAL_COM1_IRQn                   USART2_IRQn

/**
 * @brief Definition for COM port2, connected to USART3
 */ 
#define EVAL_COM2                        USART3
#define EVAL_COM2_CLK                    RCC_APB1Periph_USART3

#define EVAL_COM2_TX_PIN                 GPIO_Pin_10
#define EVAL_COM2_TX_GPIO_PORT           GPIOC
#define EVAL_COM2_TX_GPIO_CLK            RCC_AHBPeriph_GPIOC
#define EVAL_COM2_TX_SOURCE              GPIO_PinSource10
#define EVAL_COM2_TX_AF                  GPIO_AF_USART3

#define EVAL_COM2_RX_PIN                 GPIO_Pin_11
#define EVAL_COM2_RX_GPIO_PORT           GPIOC
#define EVAL_COM2_RX_GPIO_CLK            RCC_AHBPeriph_GPIOC
#define EVAL_COM2_RX_SOURCE              GPIO_PinSource11
#define EVAL_COM2_RX_AF                  GPIO_AF_USART3

#define EVAL_COM2_IRQn                   USART3_IRQn

/**
  * @}
  */ 

/** @addtogroup STM32L152_EVAL_LOW_LEVEL_SD_FLASH
  * @{
  */ 
/**
  * @brief  SD Card SPI Interface
  */  
#define SD_SPI                           SPI2
#define SD_SPI_CLK                       RCC_APB1Periph_SPI2
#define SD_SPI_SCK_PIN                   GPIO_Pin_13                 /* PB.13 */
#define SD_SPI_SCK_GPIO_PORT             GPIOB                       /* GPIOB */
#define SD_SPI_SCK_GPIO_CLK              RCC_AHBPeriph_GPIOB
#define SD_SPI_SCK_SOURCE                GPIO_PinSource13
#define SD_SPI_SCK_AF                    GPIO_AF_SPI2
#define SD_SPI_MISO_PIN                  GPIO_Pin_14                 /* PB.14 */
#define SD_SPI_MISO_GPIO_PORT            GPIOB                       /* GPIOB */
#define SD_SPI_MISO_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define SD_SPI_MISO_SOURCE               GPIO_PinSource14
#define SD_SPI_MISO_AF                   GPIO_AF_SPI2
#define SD_SPI_MOSI_PIN                  GPIO_Pin_15                 /* PB.15 */
#define SD_SPI_MOSI_GPIO_PORT            GPIOB                       /* GPIOB */
#define SD_SPI_MOSI_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define SD_SPI_MOSI_SOURCE               GPIO_PinSource15
#define SD_SPI_MOSI_AF                   GPIO_AF_SPI2
#define SD_CS_PIN                        GPIO_Pin_7                  /* PD.07 */
#define SD_CS_GPIO_PORT                  GPIOD                       /* GPIOD */
#define SD_CS_GPIO_CLK                   RCC_AHBPeriph_GPIOD
#define SD_DETECT_PIN                    GPIO_Pin_6                  /* PE.06 */
#define SD_DETECT_EXTI_LINE              EXTI_Line6
#define SD_DETECT_EXTI_PIN_SOURCE        EXTI_PinSource6

#define SD_DETECT_GPIO_PORT              GPIOE                       /* GPIOE */
#define SD_DETECT_GPIO_CLK               RCC_AHBPeriph_GPIOE
#define SD_DETECT_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOE
#define SD_DETECT_EXTI_IRQn              EXTI9_5_IRQn 
/**
  * @}
  */ 
  
/** @addtogroup STM32L152_EVAL_LOW_LEVEL_TSENSOR_I2C
  * @{
  */
/**
  * @brief  LM75 Temperature Sensor I2C Interface pins
  */  
#define LM75_I2C                         I2C1
#define LM75_I2C_CLK                     RCC_APB1Periph_I2C1
#define LM75_I2C_SCL_PIN                 GPIO_Pin_6                  /* PB.06 */
#define LM75_I2C_SCL_GPIO_PORT           GPIOB                       /* GPIOB */
#define LM75_I2C_SCL_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define LM75_I2C_SCL_SOURCE              GPIO_PinSource6
#define LM75_I2C_SCL_AF                  GPIO_AF_I2C1
#define LM75_I2C_SDA_PIN                 GPIO_Pin_7                  /* PB.07 */
#define LM75_I2C_SDA_GPIO_PORT           GPIOB                       /* GPIOB */
#define LM75_I2C_SDA_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define LM75_I2C_SDA_SOURCE              GPIO_PinSource7
#define LM75_I2C_SDA_AF                  GPIO_AF_I2C1
#define LM75_I2C_SMBUSALERT_PIN          GPIO_Pin_5                  /* PB.05 */
#define LM75_I2C_SMBUSALERT_GPIO_PORT    GPIOB                       /* GPIOB */
#define LM75_I2C_SMBUSALERT_GPIO_CLK     RCC_AHBPeriph_GPIOB
#define LM75_I2C_SMBUSALERT_SOURCE       GPIO_PinSource5
#define LM75_I2C_SMBUSALERT_AF           GPIO_AF_I2C1
#define LM75_I2C_DR                      ((uint32_t)0x40005410)

#define LM75_DMA_CLK                     RCC_AHBPeriph_DMA1
#define LM75_DMA_TX_CHANNEL              DMA1_Channel6
#define LM75_DMA_RX_CHANNEL              DMA1_Channel7
#define LM75_DMA_TX_TCFLAG               DMA1_FLAG_TC6
#define LM75_DMA_RX_TCFLAG               DMA1_FLAG_TC7

/**
  * @}
  */  

/** @addtogroup STM32L152_EVAL_LOW_LEVEL_I2C_EE
  * @{
  */
/**
  * @brief  I2C EEPROM Interface pins
  */  

#define sEE_I2C                          I2C1
#define sEE_I2C_CLK                      RCC_APB1Periph_I2C1
#define sEE_I2C_SCL_PIN                  GPIO_Pin_6                  /* PB.06 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SCL_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define sEE_I2C_SCL_SOURCE               GPIO_PinSource6
#define sEE_I2C_SCL_AF                   GPIO_AF_I2C1
#define sEE_I2C_SDA_PIN                  GPIO_Pin_7                  /* PB.07 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SDA_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define sEE_I2C_SDA_SOURCE               GPIO_PinSource7
#define sEE_I2C_SDA_AF                   GPIO_AF_I2C1
#define sEE_M24C64_32

#define sEE_I2C_DMA                      DMA1   
#define sEE_I2C_DMA_CHANNEL_TX           DMA1_Channel6
#define sEE_I2C_DMA_CHANNEL_RX           DMA1_Channel7 
#define sEE_I2C_DMA_FLAG_TX_TC           DMA1_IT_TC6   
#define sEE_I2C_DMA_FLAG_TX_GL           DMA1_IT_GL6 
#define sEE_I2C_DMA_FLAG_RX_TC           DMA1_IT_TC7 
#define sEE_I2C_DMA_FLAG_RX_GL           DMA1_IT_GL7    
#define sEE_I2C_DMA_CLK                  RCC_AHBPeriph_DMA1
#define sEE_I2C_DR_Address               ((uint32_t)0x40005410)
#define sEE_USE_DMA
   
#define sEE_I2C_DMA_TX_IRQn              DMA1_Channel6_IRQn
#define sEE_I2C_DMA_RX_IRQn              DMA1_Channel7_IRQn
#define sEE_I2C_DMA_TX_IRQHandler        DMA1_Channel6_IRQHandler
#define sEE_I2C_DMA_RX_IRQHandler        DMA1_Channel7_IRQHandler  
#define sEE_I2C_DMA_PREPRIO              0
#define sEE_I2C_DMA_SUBPRIO              0
 
#define sEE_DIRECTION_TX                 0
#define sEE_DIRECTION_RX                 1 

/* Time constant for the delay caclulation allowing to have a millisecond 
   incrementing counter. This value should be equal to (System Clock / 1000).
   ie. if system clock = 32MHz then sEE_TIME_CONST should be 32. */
#define sEE_TIME_CONST          32 

/**
  * @}
  */ 

/**
  * @}
  */ 
  
/** @defgroup STM32L152_EVAL_LOW_LEVEL_Exported_Macros
  * @{
  */  
/**
  * @}
  */ 


/** @defgroup STM32L152_EVAL_LOW_LEVEL_Exported_Functions
  * @{
  */
void STM_EVAL_LEDInit(Led_TypeDef Led);
void STM_EVAL_LEDOn(Led_TypeDef Led);
void STM_EVAL_LEDOff(Led_TypeDef Led);
void STM_EVAL_LEDToggle(Led_TypeDef Led);
void STM_EVAL_PBInit(Button_TypeDef Button, ButtonMode_TypeDef Button_Mode);
uint32_t STM_EVAL_PBGetState(Button_TypeDef Button);
void STM_EVAL_COMInit(COM_TypeDef COM, USART_InitTypeDef* USART_InitStruct); 
void SD_LowLevel_DeInit(void);
void SD_LowLevel_Init(void); 
void LM75_LowLevel_DeInit(void);
void LM75_LowLevel_Init(void);  
void sEE_LowLevel_DeInit(void);
void sEE_LowLevel_Init(void);
void sEE_LowLevel_DMAConfig(uint32_t pBuffer, uint32_t BufferSize, uint32_t Direction);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32L152_EVAL_H */
/**
  * @}
  */ 

/**
  * @}
  */ 

/**
  * @}
  */

/**
  * @}
  */  

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
