/**
  ******************************************************************************
  * @file    stm32303c_eval_i2c_tsensor.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    23-October-2012
  * @brief   This file contains all the functions prototypes for the 
  *          stm32303c_eval_i2c_tsensor.c firmware driver.
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
#ifndef __STM32303C_EVAL_I2C_TSENSOR_H
#define __STM32303C_EVAL_I2C_TSENSOR_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32303c_eval.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32303C_EVAL
  * @{
  */ 

/** @addtogroup Common
  * @{
  */
  
/** @addtogroup STM32303C_EVAL_I2C_TSENSOR
  * @{
  */  

/** @defgroup STM32303C_EVAL_I2C_TSENSOR_Exported_Types
  * @{
  */
   
/** 
  * @brief  TSENSOR Status  
  */ 
typedef enum
{
  TS751_OK = 0,
  TS751_FAIL
}TS751_Status_TypDef;

/**
  * @}
  */
  
/** @defgroup STM32303C_EVAL_I2C_TSENSOR_Exported_Constants
  * @{
  */ 
    
/* Uncomment the following line to use Timeout_User_Callback TS751_TimeoutUserCallback(). 
   If This Callback is enabled, it should be implemented by user in main function .
   TS751_TimeoutUserCallback() function is called whenever a timeout condition 
   occure during communication (waiting on an event that doesn't occur, bus 
   errors, busy devices ...). */   
/*#define USE_TIMEOUT_USER_CALLBACK*/
    
/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define TS751_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define TS751_LONG_TIMEOUT         ((uint32_t)(10 * TS751_FLAG_TIMEOUT))    
    

/**
  * @brief  Block Size
  */
#define TS751_REG_TEMPH      0x00  /*!< Temperature Register of TS751 */
#define TS751_REG_TEMPL      0x02  /*!< Temperature Register of TS751 */
#define TS751_REG_CONF       0x03  /*!< Configuration Register of TS751 */
#define TS751_REG_THYS       0x21  /*!< Temperature Register of TS751 */
#define TS751_REG_TOS        0x20  /*!< Over-temp Shutdown threshold Register of TS751 */
#define I2C_TIMEOUT         ((uint32_t)0x3FFFF) /*!< I2C Time out */
#define TS751_ADDR           0x90   /*!< TS751 address */


/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 72 MHz */
/* set TIMING to 0xC062121F to reach 100 KHz speed (Rise time = 640ns, Fall time = 20ns) */

#define TS751_I2C_TIMING              0xC062121F
  

/**
  * @}
  */

/** @defgroup STM32303C_EVAL_I2C_TSENSOR_Exported_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32303C_EVAL_I2C_TSENSOR_Exported_Functions
  * @{
  */ 
void TS751_DeInit(void);
void TS751_Init(void);
ErrorStatus TS751_GetStatus(void);
uint8_t TS751_AlerteResponseAddressRead(void);
uint16_t TS751_ReadTemp(void);
uint8_t TS751_ReadReg(uint8_t RegName);
uint8_t TS751_WriteReg(uint8_t RegName, uint16_t RegValue);
uint8_t TS751_ReadConfReg(void);
uint8_t TS751_WriteConfReg(uint8_t RegValue);
uint8_t TS751_ShutDown(FunctionalState NewState);

/** 
  * @brief  Timeout user callback function. This function is called when a timeout
  *         condition occurs during communication with TS751. Only protoype
  *         of this function is decalred in TS751 driver. Its implementation
  *         may be done into user application. This function may typically stop
  *         current operations and reset the I2C peripheral and TS751.
  *         To enable this function use uncomment the define USE_TIMEOUT_USER_CALLBACK
  *         at the top of this file.          
  */
#ifdef USE_TIMEOUT_USER_CALLBACK 
 uint8_t TS751_TIMEOUT_UserCallback(void);
#else
 #define TS751_TIMEOUT_UserCallback()  TS751_FAIL
#endif /* USE_TIMEOUT_USER_CALLBACK */
 
#ifdef __cplusplus
}
#endif

#endif /* __STM32303C_EVAL_I2C_TSENSOR_H */
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
