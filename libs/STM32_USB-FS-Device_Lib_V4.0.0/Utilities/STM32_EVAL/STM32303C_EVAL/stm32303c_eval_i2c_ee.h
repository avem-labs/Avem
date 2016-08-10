/**
  ******************************************************************************
  * @file    stm32303c_eval_i2c_ee.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    23-October-2012
  * @brief   This file contains all the functions prototypes for 
  *          the stm32303c_eval_i2c_ee.c firmware driver.
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
#ifndef __STM32303C_EVAL_I2C_EE_H
#define __STM32303C_EVAL_I2C_EE_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32303c_eval.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM32303C_EVAL
  * @{
  */
  
/** @addtogroup STM32303C_EVAL_I2C_EE
  * @{
  */  

/** @defgroup STM32303C_EVAL_I2C_EE_Exported_Types
  * @{
  */ 

/**
  * @}
  */
  
/** @defgroup STM32303C_EVAL_I2C_EE_Exported_Constants
  * @{
  */

/* Select which EEPROM will be used with this driver */
#define sEE_M24LR64 
/* #define sEE_M24M01 */ 
   
/* Uncomment the following line to use the default sEE_TIMEOUT_UserCallback() 
   function implemented in stm32303c_eval_i2c_ee.c file.
   sEE_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occure during communication (waiting on an event that doesn't occur, bus 
   errors, busy devices ...). */   
/* #define USE_DEFAULT_TIMEOUT_CALLBACK */
   
#if !defined (sEE_M24C08) && !defined (sEE_M24C64_32) && !defined (sEE_M24LR64) && !defined (sEE_M24M01)
/* Use the defines below the choose the EEPROM type */
#define sEE_M24M01  /* Support the device: M24M01. */
/* #define sEE_M24C08*/  /* Support the device: M24C08. */
/* note: Could support: M24C01, M24C02, M24C04 and M24C16 if the blocks and 
   HW address are correctly defined*/
/*#define sEE_M24C64_32*/  /* Support the devices: M24C32 and M24C64 */
/*#define sEE_M24LR64*/ /*Support the devices: M24LR64 */
#endif

#ifdef sEE_M24C64_32
/* For M24C32 and M24C64 devices, E0,E1 and E2 pins are all used for device 
  address selection (ne need for additional address lines). According to the 
  Harware connection on the board. */

 #define sEE_HW_ADDRESS         0xA0   /* E0 = E1 = E2 = 0 */ 

#elif defined (sEE_M24C08)
/* The M24C08W contains 4 blocks (128byte each) with the adresses below: E2 = 0 
   EEPROM Addresses defines */
 #define sEE_HW_ADDRESS     0xA0   /* E2 = 0 */ 
 /*#define sEE_HW_ADDRESS     0xA2*/ /* E2 = 0 */  
 /*#define sEE_HW_ADDRESS     0xA4*/ /* E2 = 0 */
 /*#define sEE_HW_ADDRESS     0xA6*/ /* E2 = 0 */

#elif defined (sEE_M24LR64)
 #define sEE_HW_ADDRESS         0xA0

#elif defined (sEE_M24M01)
/* The sEE_M24M01 contains 2 blocks (64Kbytes each) with the adresses below 
   EEPROM Addresses defines */
 #define sEE_HW_ADDRESS     0xA4      /* Block 1 : E1= 1; E2 = 0; A16 = 0 */
 /*#define sEE_HW_ADDRESS     0xA6*/  /* Block 2 : E1= 1; E2 = 0; A16 = 1 */

#endif /* sEE_M24C64_32 */

/* I2C TIMING Register define when I2C clock source is SYSCLK */
/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 72 MHz */
/* When using sEE_M24M01 set TIMING to 0x00C4092A to reach 1 MHz speed (Rise time = 26ns, Fall time = 2ns) */
/* When using sEE_M24LR64 set TIMING to 0xC062121F to reach 100 KHz speed (Rise time = 640ns, Fall time = 20ns) */

#define sEE_I2C_TIMING             0xC062121F
   

#if defined (sEE_M24C08)
 #define sEE_PAGESIZE           16
#elif defined (sEE_M24C64_32)
 #define sEE_PAGESIZE           32
#elif defined (sEE_M24LR64)
 #define sEE_PAGESIZE           4
#elif defined (sEE_M24M01)
 #define sEE_PAGESIZE           128   
#endif
   
/* Maximum Timeout values for flags and events waiting loops. These timeouts are
   not based on accurate values, they just guarantee that the application will 
   not remain stuck if the I2C communication is corrupted.
   You may modify these timeout values depending on CPU frequency and application
   conditions (interrupts routines ...). */   
#define sEE_FLAG_TIMEOUT         ((uint32_t)0x1000)
#define sEE_LONG_TIMEOUT         ((uint32_t)(10 * sEE_FLAG_TIMEOUT))

/* Maximum number of trials for sEE_WaitEepromStandbyState() function */
#define sEE_MAX_TRIALS_NUMBER     300
      
#define sEE_OK                    0
#define sEE_FAIL                  1   

/**
  * @}
  */ 
  
/** @defgroup STM32303C_EVAL_I2C_EE_Exported_Macros
  * @{
  */    
/**
  * @}
  */ 

/** @defgroup STM32303C_EVAL_I2C_EE_Exported_Functions
  * @{
  */ 
void     sEE_DeInit(void);
void     sEE_Init(void);
uint32_t sEE_ReadBuffer(uint8_t* pBuffer, uint16_t ReadAddr, uint16_t* NumByteToRead);
uint32_t sEE_WritePage(uint8_t* pBuffer, uint16_t WriteAddr, uint8_t* NumByteToWrite);
void     sEE_WriteBuffer(uint8_t* pBuffer, uint16_t WriteAddr, uint16_t NumByteToWrite);
uint32_t sEE_WaitEepromStandbyState(void);

/* USER Callbacks: These are functions for which prototypes only are declared in
   EEPROM driver and that should be implemented into user applicaiton. */  
/* sEE_TIMEOUT_UserCallback() function is called whenever a timeout condition 
   occure during communication (waiting on an event that doesn't occur, bus 
   errors, busy devices ...).
   You can use the default timeout callback implementation by uncommenting the 
   define USE_DEFAULT_TIMEOUT_CALLBACK in stm32303c_eval_i2c_ee.h file.
   Typically the user implementation of this callback should reset I2C peripheral
   and re-initialize communication or in worst case reset all the application. */
uint32_t sEE_TIMEOUT_UserCallback(void);

#ifdef __cplusplus
}
#endif

#endif /* __STM32303C_EVAL_I2C_EE_H */
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


