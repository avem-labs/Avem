/**
  ******************************************************************************
  * @file    stm32373c_eval.h
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2012
  * @brief   This file contains definitions for STM32373C_EVAL's Leds, push-buttons
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
#ifndef __STM32373C_EVAL_H
#define __STM32373C_EVAL_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f37x.h"
#include "stm32_eval_legacy.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */

/** @addtogroup STM32373C_EVAL
  * @{
  */
      
/** @addtogroup STM32373C_EVAL_LOW_LEVEL
  * @{
  */ 

/** @defgroup STM32373C_EVAL_LOW_LEVEL_Exported_Types
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
  BUTTON_WAKEUP = 0,
  BUTTON_TAMPER = 1,
  BUTTON_KEY = 2,
  BUTTON_RIGHT = 3,
  BUTTON_LEFT = 4,
  BUTTON_UP = 5,
  BUTTON_DOWN = 6,
  BUTTON_SEL = 7
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

/** @defgroup STM32373C_EVAL_LOW_LEVEL_Exported_Constants
  * @{
  */ 

/** 
  * @brief  Define for STM32373C_EVAL board  
  */ 
#if !defined (USE_STM32373C_EVAL)
 #define USE_STM32373C_EVAL
#endif

/** @addtogroup STM32373C_EVAL_LOW_LEVEL_LED
  * @{
  */
#define LEDn                             4

#define LED1_PIN                         GPIO_Pin_0
#define LED1_GPIO_PORT                   GPIOC
#define LED1_GPIO_CLK                    RCC_AHBPeriph_GPIOC
  
#define LED2_PIN                         GPIO_Pin_1
#define LED2_GPIO_PORT                   GPIOC
#define LED2_GPIO_CLK                    RCC_AHBPeriph_GPIOC
  
#define LED3_PIN                         GPIO_Pin_2
#define LED3_GPIO_PORT                   GPIOC
#define LED3_GPIO_CLK                    RCC_AHBPeriph_GPIOC
  
#define LED4_PIN                         GPIO_Pin_3
#define LED4_GPIO_PORT                   GPIOC
#define LED4_GPIO_CLK                    RCC_AHBPeriph_GPIOC

/**
  * @}
  */ 

/** @addtogroup STM32373C_EVAL_LOW_LEVEL_BUTTON
  * @{
  */  
#define BUTTONn                          7

/**
 * @brief Tamper push-button
 */
#define TAMPER_BUTTON_PIN                GPIO_Pin_0
#define TAMPER_BUTTON_GPIO_PORT          GPIOA
#define TAMPER_BUTTON_GPIO_CLK           RCC_AHBPeriph_GPIOA
#define TAMPER_BUTTON_EXTI_LINE          EXTI_Line0
#define TAMPER_BUTTON_EXTI_PORT_SOURCE   EXTI_PortSourceGPIOA
#define TAMPER_BUTTON_EXTI_PIN_SOURCE    EXTI_PinSource0
#define TAMPER_BUTTON_EXTI_IRQn          EXTI0_IRQn
/**
 * @brief Key push-button
 */
#define KEY_BUTTON_PIN                   GPIO_Pin_2
#define KEY_BUTTON_GPIO_PORT             GPIOA
#define KEY_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOA
#define KEY_BUTTON_EXTI_LINE             EXTI_Line2
#define KEY_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOA
#define KEY_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource2
#define KEY_BUTTON_EXTI_IRQn             EXTI2_TS_IRQn

/**
 * @brief Joystick Right push-button
 */
#define RIGHT_BUTTON_PIN                 GPIO_Pin_9
#define RIGHT_BUTTON_GPIO_PORT           GPIOF
#define RIGHT_BUTTON_GPIO_CLK            RCC_AHBPeriph_GPIOF
#define RIGHT_BUTTON_EXTI_LINE           EXTI_Line9
#define RIGHT_BUTTON_EXTI_PORT_SOURCE    EXTI_PortSourceGPIOF
#define RIGHT_BUTTON_EXTI_PIN_SOURCE     EXTI_PinSource9
#define RIGHT_BUTTON_EXTI_IRQn           EXTI9_5_IRQn

/**
 * @brief Joystick Left push-button
 */
#define LEFT_BUTTON_PIN                  GPIO_Pin_4
#define LEFT_BUTTON_GPIO_PORT            GPIOF
#define LEFT_BUTTON_GPIO_CLK             RCC_AHBPeriph_GPIOF
#define LEFT_BUTTON_EXTI_LINE            EXTI_Line4
#define LEFT_BUTTON_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOF
#define LEFT_BUTTON_EXTI_PIN_SOURCE      EXTI_PinSource4
#define LEFT_BUTTON_EXTI_IRQn            EXTI4_IRQn  

/**
 * @brief Joystick Up push-button
 */
#define UP_BUTTON_PIN                    GPIO_Pin_10
#define UP_BUTTON_GPIO_PORT              GPIOF
#define UP_BUTTON_GPIO_CLK               RCC_AHBPeriph_GPIOF
#define UP_BUTTON_EXTI_LINE              EXTI_Line10
#define UP_BUTTON_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOF
#define UP_BUTTON_EXTI_PIN_SOURCE        EXTI_PinSource10
#define UP_BUTTON_EXTI_IRQn              EXTI15_10_IRQn  

/**
 * @brief Joystick Down push-button
 */  
#define DOWN_BUTTON_PIN                  GPIO_Pin_2
#define DOWN_BUTTON_GPIO_PORT            GPIOF
#define DOWN_BUTTON_GPIO_CLK             RCC_AHBPeriph_GPIOF
#define DOWN_BUTTON_EXTI_LINE            EXTI_Line2
#define DOWN_BUTTON_EXTI_PORT_SOURCE     EXTI_PortSourceGPIOF
#define DOWN_BUTTON_EXTI_PIN_SOURCE      EXTI_PinSource2
#define DOWN_BUTTON_EXTI_IRQn            EXTI2_TS_IRQn  

/**
 * @brief Joystick Sel push-button
 */
#define SEL_BUTTON_PIN                   GPIO_Pin_6
#define SEL_BUTTON_GPIO_PORT             GPIOE
#define SEL_BUTTON_GPIO_CLK              RCC_AHBPeriph_GPIOE
#define SEL_BUTTON_EXTI_LINE             EXTI_Line6
#define SEL_BUTTON_EXTI_PORT_SOURCE      EXTI_PortSourceGPIOE
#define SEL_BUTTON_EXTI_PIN_SOURCE       EXTI_PinSource6
#define SEL_BUTTON_EXTI_IRQn             EXTI9_5_IRQn 

/**
  * @}
  */ 


/** @addtogroup STM32373C_EVAL_LOW_LEVEL_COM
  * @{
  */
#define COMn                             1

/**
 * @brief Definition for COM port1, connected to USART2
 */ 
#define EVAL_COM1                        USART2
#define EVAL_COM1_CLK                    RCC_APB1Periph_USART2

#define EVAL_COM1_TX_PIN                 GPIO_Pin_5
#define EVAL_COM1_TX_GPIO_PORT           GPIOD
#define EVAL_COM1_TX_GPIO_CLK            RCC_AHBPeriph_GPIOD
#define EVAL_COM1_TX_SOURCE              GPIO_PinSource5
#define EVAL_COM1_TX_AF                  GPIO_AF_7

#define EVAL_COM1_RX_PIN                 GPIO_Pin_6
#define EVAL_COM1_RX_GPIO_PORT           GPIOD
#define EVAL_COM1_RX_GPIO_CLK            RCC_AHBPeriph_GPIOD
#define EVAL_COM1_RX_SOURCE              GPIO_PinSource6
#define EVAL_COM1_RX_AF                  GPIO_AF_7

#define EVAL_COM1_CTS_PIN                GPIO_Pin_3
#define EVAL_COM1_CTS_GPIO_PORT          GPIOD
#define EVAL_COM1_CTS_GPIO_CLK           RCC_AHBPeriph_GPIOD
#define EVAL_COM1_CTS_SOURCE             GPIO_PinSource3
#define EVAL_COM1_CTS_AF                 GPIO_AF_7

#define EVAL_COM1_RTS_PIN                GPIO_Pin_4
#define EVAL_COM1_RTS_GPIO_PORT          GPIOD
#define EVAL_COM1_RTS_GPIO_CLK           RCC_AHBPeriph_GPIOD
#define EVAL_COM1_RTS_SOURCE             GPIO_PinSource4
#define EVAL_COM1_RTS_AF                 GPIO_AF_7
   
#define EVAL_COM1_IRQn                   USART2_IRQn
   

/**
  * @}
  */

/** @addtogroup STM32373C_EVAL_LOW_LEVEL_SD_SPI
  * @{
  */
/**
  * @brief  SD SPI Interface pins
  */
#define SD_SPI                           SPI3
#define SD_SPI_CLK                       RCC_APB1Periph_SPI3
#define SD_SPI_SCK_PIN                   GPIO_Pin_10                  /* PC.10 */
#define SD_SPI_SCK_GPIO_PORT             GPIOC                        /* GPIOC */
#define SD_SPI_SCK_GPIO_CLK              RCC_AHBPeriph_GPIOC
#define SD_SPI_SCK_SOURCE                GPIO_PinSource10
#define SD_SPI_SCK_AF                    GPIO_AF_6

#define SD_SPI_MISO_PIN                  GPIO_Pin_11                  /* PC.11 */
#define SD_SPI_MISO_GPIO_PORT            GPIOC                        /* GPIOC */
#define SD_SPI_MISO_GPIO_CLK             RCC_AHBPeriph_GPIOC
#define SD_SPI_MISO_SOURCE               GPIO_PinSource11
#define SD_SPI_MISO_AF                   GPIO_AF_6

#define SD_SPI_MOSI_PIN                  GPIO_Pin_12                  /* PC.12 */
#define SD_SPI_MOSI_GPIO_PORT            GPIOC                        /* GPIOC */
#define SD_SPI_MOSI_GPIO_CLK             RCC_AHBPeriph_GPIOC
#define SD_SPI_MOSI_SOURCE               GPIO_PinSource12
#define SD_SPI_MOSI_AF                   GPIO_AF_6

#define SD_CS_PIN                        GPIO_Pin_2                  /* PE.02 */
#define SD_CS_GPIO_PORT                  GPIOE                       /* GPIOE */
#define SD_CS_GPIO_CLK                   RCC_AHBPeriph_GPIOE

   
#define SD_DETECT_PIN                    GPIO_Pin_3                  /* PE.03  */
#define SD_DETECT_EXTI_LINE              EXTI_Line3
#define SD_DETECT_EXTI_PIN_SOURCE        EXTI_PinSource3
#define SD_DETECT_GPIO_PORT              GPIOE                       /* GPIOE */
#define SD_DETECT_GPIO_CLK               RCC_AHBPeriph_GPIOE
#define SD_DETECT_EXTI_PORT_SOURCE       EXTI_PortSourceGPIOE
#define SD_DETECT_EXTI_IRQn              EXTI3_IRQn

/**
  * @}
  */
  
/** @addtogroup STM32373C_EVAL_LOW_LEVEL_TSENSOR_I2C
  * @{
  */
/**
  * @brief  LM75 Temperature Sensor I2C Interface pins
  */
#define LM75_I2C                         I2C2
#define LM75_I2C_CLK                     RCC_APB1Periph_I2C2
   
#define LM75_I2C_SCL_PIN                 GPIO_Pin_9                  /* PA.09 */
#define LM75_I2C_SCL_GPIO_PORT           GPIOA                       /* GPIOA */
#define LM75_I2C_SCL_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define LM75_I2C_SCL_SOURCE              GPIO_PinSource9
#define LM75_I2C_SCL_AF                  GPIO_AF_4
   
#define LM75_I2C_SDA_PIN                 GPIO_Pin_10                 /* PA.10 */
#define LM75_I2C_SDA_GPIO_PORT           GPIOA                       /* GPIOA */
#define LM75_I2C_SDA_GPIO_CLK            RCC_AHBPeriph_GPIOA
#define LM75_I2C_SDA_SOURCE              GPIO_PinSource10
#define LM75_I2C_SDA_AF                  GPIO_AF_4
   
#define LM75_I2C_SMBUSALERT_PIN          GPIO_Pin_8                  /* PA.08 */
#define LM75_I2C_SMBUSALERT_GPIO_PORT    GPIOA                       /* GPIOA */
#define LM75_I2C_SMBUSALERT_GPIO_CLK     RCC_AHBPeriph_GPIOA
#define LM75_I2C_SMBUSALERT_SOURCE       GPIO_PinSource8
#define LM75_I2C_SMBUSALERT_AF           GPIO_AF_4
/**
  * @}
  */
   
/** @addtogroup STM32373C_EVAL_LOW_LEVEL_I2C_EE
  * @{
  */   
/**
  * @brief  I2C EEPROM Interface pins
  */  
#define sEE_I2C                          I2C2
#define sEE_I2C_CLK                      RCC_APB1Periph_I2C2
   
#define sEE_I2C_SCL_PIN                  GPIO_Pin_9                  /* PA.09 */
#define sEE_I2C_SCL_GPIO_PORT            GPIOA                       /* GPIOA */
#define sEE_I2C_SCL_GPIO_CLK             RCC_AHBPeriph_GPIOA
#define sEE_I2C_SCL_SOURCE               GPIO_PinSource9
#define sEE_I2C_SCL_AF                   GPIO_AF_4

#define sEE_I2C_SDA_PIN                  GPIO_Pin_10                 /* PA.10 */
#define sEE_I2C_SDA_GPIO_PORT            GPIOA                       /* GPIOA */
#define sEE_I2C_SDA_GPIO_CLK             RCC_AHBPeriph_GPIOA
#define sEE_I2C_SDA_SOURCE               GPIO_PinSource10
#define sEE_I2C_SDA_AF                   GPIO_AF_4

/**
  * @}
  */   
   
/** @defgroup STM32373C_EVAL_LOW_LEVEL_Exported_Functions
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
void sEE_I2C_LowLevel_DeInit(void);
void sEE_I2C_LowLevel_Init(void); 
/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32373C_EVAL_H */
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
