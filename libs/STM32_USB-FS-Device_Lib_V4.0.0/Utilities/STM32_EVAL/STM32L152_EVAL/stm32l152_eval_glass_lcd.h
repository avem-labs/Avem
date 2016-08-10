/**
  ******************************************************************************
  * @file    stm32l152_eval_glass_lcd.h
  * @author  MCD Application Team
  * @version V5.0.2
  * @date    09-March-2012
  * @brief   Header file for stm32l152_eval_glass_lcd.c module.
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
#ifndef __STM32L152_EVAL_GLASS_LCD_H
#define __STM32L152_EVAL_GLASS_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l152_eval.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM32L152_EVAL
  * @{
  */
  
/** @addtogroup STM32L152_EVAL_GLASS_LCD
  * @{
  */ 


/** @defgroup STM32L152_EVAL_GLASS_LCD_Exported_Types
  * @{
  */
typedef enum
{
  POINT_OFF = 0,
  POINT_ON = 1
}Point_Typedef;

typedef enum
{
  APOSTROPHE_OFF = 0,
  APOSTROPHE_ON = 1
}Apostrophe_Typedef;   
/**
  * @}
  */ 

/** @defgroup STM32L152_EVAL_GLASS_LCD_Exported_Constants
  * @{
  */ 

/**
  * @}
  */
  
/** @defgroup STM32L152_EVAL_GLASS_LCD_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32L152_EVAL_GLASS_LCD_Exported_Functions
  * @{
  */
void LCD_GLASS_Init(void);
void LCD_GLASS_DisplayChar(uint8_t* ch, Point_Typedef point, Apostrophe_Typedef apostrophe,uint8_t position);
void LCD_GLASS_DisplayString(uint8_t* ptr);
void LCD_GLASS_WriteChar(uint8_t* ch, Point_Typedef point, Apostrophe_Typedef apostrophe,uint8_t position);
void LCD_GLASS_ClearChar(uint8_t position);
void LCD_GLASS_Clear(void);
void LCD_GLASS_ScrollString(uint8_t* ptr, uint16_t nScroll, uint16_t ScrollSpeed);

/**
  * @}
  */
  
#ifdef __cplusplus
}
#endif

#endif /* __STM32L152_EVAL_GLASS_LCD_H */

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
 
/**
  * @}
  */   
  
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
