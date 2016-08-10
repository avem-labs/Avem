/**
  ******************************************************************************
  * @file    stm32l152d_eval.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    09-March-2012
  * @brief   This file contains definitions for STM32L152D_EVAL's Leds, push-buttons
  *          COM ports and Temperature Sensor LM75 (on I2C) hardware resources.  
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
#ifndef __STM32L152D_EVAL_H
#define __STM32L152D_EVAL_H

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
  
/** @addtogroup STM32L152D_EVAL
  * @{
  */ 

/** @addtogroup STM32L152D_EVAL_LOW_LEVEL
  * @{
  */ 
  
/** @defgroup STM32L152D_EVAL_LOW_LEVEL_Exported_Types
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

/** @defgroup STM32L152D_EVAL_LOW_LEVEL_Exported_Constants
  * @{
  */ 

/** 
  * @brief  Define for STM32L152D_EVAL board  
  */ 
#if !defined (USE_STM32L152D_EVAL)
 #define USE_STM32L152D_EVAL
#endif
  
/** @addtogroup STM32L152D_EVAL_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             4

#define LED1_PIN                         GPIO_Pin_3
#define LED1_GPIO_PORT                   GPIOD
#define LED1_GPIO_CLK                    RCC_AHBPeriph_GPIOD  
  
#define LED2_PIN                         GPIO_Pin_7
#define LED2_GPIO_PORT                   GPIOD
#define LED2_GPIO_CLK                    RCC_AHBPeriph_GPIOD  

#define LED3_PIN                         GPIO_Pin_14  
#define LED3_GPIO_PORT                   GPIOG
#define LED3_GPIO_CLK                    RCC_AHBPeriph_GPIOG  

#define LED4_PIN                         GPIO_Pin_15
#define LED4_GPIO_PORT                   GPIOG
#define LED4_GPIO_CLK                    RCC_AHBPeriph_GPIOG

/**
  * @}
  */
  
/** @addtogroup STM32L152D_EVAL_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                          6 
/* On STM32L152D-EVAL board, the KEY button is connected to PA.00 and it can
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
#define RIGHT_BUTTON_PIN                 GPIO_Pin_7
#define RIGHT_BUTTON_GPIO_PORT           GPIOG
#define RIGHT_BUTTON_GPIO_CLK            RCC_AHBPeriph_GPIOG
#define RIGHT_BUTTON_EXTI_LINE           EXTI_Line7
#define RIGHT_BUTTON_EXTI_PORT_SOURCE    EXTI_PortSourceGPIOG
#define RIGHT_BUTTON_EXTI_PIN_SOURCE     EXTI_PinSource7
#define RIGHT_BUTTON_EXTI_IRQn           EXTI9_5_IRQn

/**
 * @brief Joystick Left push-button
 */    
#define LEFT_BUTTON_PIN                  GPIO_Pin_6
#define LEFT_BUTTON_GPIO_PORT            GPIOG
#define LEFT_BUTTON_GPIO_CLK             RCC_AHBPeriph_GPIOG
#define LEFT_BUTTON_EXTI_LINE            EXTI_Line6
#define LEFT_BUTTON_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOG
#define LEFT_BUTTON_EXTI_PIN_SOURCE      EXTI_PinSource6
#define LEFT_BUTTON_EXTI_IRQn            EXTI9_5_IRQn  

/**
 * @brief Joystick Up push-button
 */
#define UP_BUTTON_PIN                    GPIO_Pin_11
#define UP_BUTTON_GPIO_PORT              GPIOG
#define UP_BUTTON_GPIO_CLK               RCC_AHBPeriph_GPIOG
#define UP_BUTTON_EXTI_LINE              EXTI_Line11
#define UP_BUTTON_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOG
#define UP_BUTTON_EXTI_PIN_SOURCE        EXTI_PinSource11
#define UP_BUTTON_EXTI_IRQn              EXTI15_10_IRQn  

/**
 * @brief Joystick Down push-button
 */   
#define DOWN_BUTTON_PIN                  GPIO_Pin_8
#define DOWN_BUTTON_GPIO_PORT            GPIOG
#define DOWN_BUTTON_GPIO_CLK             RCC_AHBPeriph_GPIOG
#define DOWN_BUTTON_EXTI_LINE            EXTI_Line8
#define DOWN_BUTTON_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOG
#define DOWN_BUTTON_EXTI_PIN_SOURCE      EXTI_PinSource8
#define DOWN_BUTTON_EXTI_IRQn            EXTI9_5_IRQn  

/**
 * @brief Joystick Sel push-button
 */  
#define SEL_BUTTON_PIN                   GPIO_Pin_13
#define SEL_BUTTON_GPIO_PORT             GPIOG
#define SEL_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOG
#define SEL_BUTTON_EXTI_LINE             EXTI_Line13
#define SEL_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOG
#define SEL_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource13
#define SEL_BUTTON_EXTI_IRQn             EXTI15_10_IRQn          

/**
  * @}
  */ 

/** @addtogroup STM32L152D_EVAL_LOW_LEVEL_COM
  * @{
  */
#define COMn                             1

/**
 * @brief Definition for COM port1, connected to USART1
 */ 
#define EVAL_COM1                        USART1
#define EVAL_COM1_CLK                    RCC_APB2Periph_USART1

#define EVAL_COM1_TX_PIN                 GPIO_Pin_9
#define EVAL_COM1_TX_GPIO_PORT           GPIOA
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource9
#define EVAL_COM1_TX_AF                  GPIO_AF_USART1

#define EVAL_COM1_RX_PIN                 GPIO_Pin_10
#define EVAL_COM1_RX_GPIO_PORT           GPIOA
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource10
#define EVAL_COM1_RX_AF                  GPIO_AF_USART1

#define EVAL_COM1_IRQn                   USART1_IRQn

/**
  * @}
  */ 

/** @addtogroup STM32L152D_EVAL_LOW_LEVEL_SD_FLASH
  * @{
  */
/**
  * @brief  SD FLASH SDIO Interface
  */ 
#define SD_DETECT_PIN                    GPIO_Pin_7                  /* PC.07 */
#define SD_DETECT_EXTI_LINE              EXTI_Line7
#define SD_DETECT_EXTI_PIN_SOURCE        EXTI_PinSource7

#define SD_DETECT_GPIO_PORT              GPIOC                       /* GPIOC */
#define SD_DETECT_GPIO_CLK               RCC_AHBPeriph_GPIOC
#define SD_DETECT_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOC
#define SD_DETECT_EXTI_IRQn              EXTI9_5_IRQn


#define SDIO_FIFO_ADDRESS                ((uint32_t)0x40012C80)
/** 
  * @brief  SDIO Intialization Frequency (400KHz max)
  */
#define SDIO_INIT_CLK_DIV                ((uint8_t)0x76)
/** 
  * @brief  SDIO Data Transfer Frequency (24MHz max) 
  */
#define SDIO_TRANSFER_CLK_DIV            ((uint8_t)0x1)


#define SD_SDIO_DMA                      DMA2
#define SD_SDIO_DMA_CLK                  RCC_AHBPeriph_DMA2
#define SD_SDIO_DMA_CHANNEL              DMA2_Channel4
#define SD_SDIO_DMA_FLAG_TC              DMA2_FLAG_TC4
#define SD_SDIO_DMA_FLAG_TE              DMA2_FLAG_TE4
#define SD_SDIO_DMA_FLAG_HT              DMA2_FLAG_HT4
#define SD_SDIO_DMA_FLAG_GL              DMA2_FLAG_GL4
#define SD_SDIO_DMA_IRQn                 DMA2_Channel4_IRQn
#define SD_SDIO_DMA_IRQHANDLER           DMA2_Channel4_IRQHandler
/**
  * @}
  */ 
  
/** @addtogroup STM32L152D_EVAL_LOW_LEVEL_TSENSOR_I2C
  * @{
  */
/**
  * @brief  LM75 Temperature Sensor I2C Interface pins
  */  
#define LM75_I2C                         I2C1
#define LM75_I2C_CLK                     RCC_APB1Periph_I2C1
#define LM75_I2C_SCL_PIN                 GPIO_Pin_8                  /* PB.08 */
#define LM75_I2C_SCL_GPIO_PORT           GPIOB                       /* GPIOB */
#define LM75_I2C_SCL_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define LM75_I2C_SCL_SOURCE              GPIO_PinSource8
#define LM75_I2C_SCL_AF                  GPIO_AF_I2C1
#define LM75_I2C_SDA_PIN                 GPIO_Pin_9                  /* PB.09 */
#define LM75_I2C_SDA_GPIO_PORT           GPIOB                       /* GPIOB */
#define LM75_I2C_SDA_GPIO_CLK            RCC_AHBPeriph_GPIOB
#define LM75_I2C_SDA_SOURCE              GPIO_PinSource9
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
  
/** @addtogroup STM32L152D_EVAL_LOW_LEVEL_SPI_EE
  * @{
  */
/**
  * @brief  sEE SPI Interface pins
  */
#define sEE_SPI                          SPI1
#define sEE_SPI_CLK                      RCC_APB2Periph_SPI1
   
#define sEE_SPI_SCK_PIN                  GPIO_Pin_5                  /* PA.05 */
#define sEE_SPI_SCK_GPIO_PORT            GPIOA                       /* GPIOA */
#define sEE_SPI_SCK_GPIO_CLK             RCC_AHBPeriph_GPIOA
#define sEE_SPI_SCK_SOURCE               GPIO_PinSource5
#define sEE_SPI_SCK_AF                   GPIO_AF_SPI1

#define sEE_SPI_MISO_PIN                 GPIO_Pin_14                 /* PE.14 */
#define sEE_SPI_MISO_GPIO_PORT           GPIOE                       /* GPIOE */
#define sEE_SPI_MISO_GPIO_CLK            RCC_AHBPeriph_GPIOE
#define sEE_SPI_MISO_SOURCE              GPIO_PinSource14
#define sEE_SPI_MISO_AF                  GPIO_AF_SPI1

#define sEE_SPI_MOSI_PIN                 GPIO_Pin_15                 /* PE.15 */
#define sEE_SPI_MOSI_GPIO_PORT           GPIOE                       /* GPIOE */
#define sEE_SPI_MOSI_GPIO_CLK            RCC_AHBPeriph_GPIOE
#define sEE_SPI_MOSI_SOURCE              GPIO_PinSource15
#define sEE_SPI_MOSI_AF                  GPIO_AF_SPI1

#define sEE_SPI_CS_PIN                   GPIO_Pin_5                  /* PC.05 */
#define sEE_SPI_CS_GPIO_PORT             GPIOC                       /* GPIOC */
#define sEE_SPI_CS_GPIO_CLK              RCC_AHBPeriph_GPIOC   

/**
  * @brief  sEE SPI Interface Type
  */
#define sEE_M95040   
   
/**
  * @}
  */

/** @addtogroup STM32L152D_EVAL_LOW_LEVEL_I2C_EE
  * @{
  */
/**
  * @brief  I2C EEPROM Interface pins
  */  
#define sEE_I2C                          I2C1
#define sEE_I2C_CLK                      RCC_APB1Periph_I2C1
   
#define sEE_I2C_SCL_PIN                  GPIO_Pin_8                  /* PB.08 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SCL_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define sEE_I2C_SCL_SOURCE               GPIO_PinSource8
#define sEE_I2C_SCL_AF                   GPIO_AF_I2C1

#define sEE_I2C_SDA_PIN                  GPIO_Pin_9                  /* PB.09 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOB                       /* GPIOB */
#define sEE_I2C_SDA_GPIO_CLK             RCC_AHBPeriph_GPIOB
#define sEE_I2C_SDA_SOURCE               GPIO_PinSource9
#define sEE_I2C_SDA_AF                   GPIO_AF_I2C1

#define sEE_M24LR64

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
#define sEE_TIME_CONST                   32 
   
/**
  * @}
  */
   
/** @defgroup STM32L152D_EVAL_LOW_LEVEL_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32L152D_EVAL_LOW_LEVEL_Exported_Functions
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
void SD_LowLevel_DMA_TxConfig(uint32_t *BufferSRC, uint32_t BufferSize);
void SD_LowLevel_DMA_RxConfig(uint32_t *BufferDST, uint32_t BufferSize);
void LM75_LowLevel_DeInit(void);
void LM75_LowLevel_Init(void); 
void sEE_SPI_LowLevel_DeInit(void);
void sEE_SPI_LowLevel_Init(void); 
void sEE_LowLevel_DeInit(void);
void sEE_LowLevel_Init(void); 
void sEE_LowLevel_DMAConfig(uint32_t pBuffer, uint32_t BufferSize, uint32_t Direction);
/**
  * @}
  */
#ifdef __cplusplus
}
#endif
  
#endif /* __STM32L152D_EVAL_H */
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
