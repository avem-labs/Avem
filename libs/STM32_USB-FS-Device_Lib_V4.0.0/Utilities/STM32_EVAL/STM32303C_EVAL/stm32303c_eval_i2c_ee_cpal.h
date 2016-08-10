/**
  ******************************************************************************
  * @file    stm32303c_eval_i2c_ee_cpal.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    23-October-2012
  * @brief   This file contains all the functions prototypes for 
  *          the stm32303c_eval_i2c_ee_cpal.c firmware driver.
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
#ifndef __STM32303C_EVAL_I2C_EE_CPAL_H
#define __STM32303C_EVAL_I2C_EE_CPAL_H

#ifdef __cplusplus
 extern "C" {
#endif


/*===================================================================================
                                             User NOTES
=====================================================================================

---------------------------------
   How To use the EEPROM Driver:
---------------------------------
----- All EEPROM operations are controlled and monitored through a single
      structure. This structure holds all necessary parameters to manage EEPROM
      communication (pointer to CPAL I2C device structure, EEPROM address, 
      EEPROM page size, EEPROM memory addressing mode, EEPROM state).
         
----- User should follow these steps to use this driver correctly :

      -1-  STRUCTURE INITIALIZATION 
      Start by initializing the structure holding EEPROM Information. To perform 
      this action, the global variable sEEx_DevStructure declared in EEPROM driver 
      as sEE_InitTypeDef (sEE1_DevStructure for EEPROM connected with I2C1, 
      sEE2_DevStructure for EEPROM connected with I2C1 ...) must be used.

      sEE_InitTypeDef structure contains five parameters:

           *- CPAL_InitTypeDef* sEE_CPALStructure : Pointer on a CPAL Device structure 
                                                    relative to the device instantiated 
                                                    to communicate with EEPROM.

           *- uint32_t sEEAddress : Contains the EEPROM device Address.
  
           *- uint32_t sEEPageSize : Contains the page size of EEPROM Memory.

           *- uint8_t sEEMemoryAddrMode : Bit-field value specifying Memory Addressing Mode.

           *- __IO sEE_StateTypeDef sEEState : Holds the current State of the EEPROM device. 

       To configure this structure, user must initialize only three parameters 
       (sEEAddress, sEEPageSize, sEEMemoryAddrMode).
       
       Example:
         sEE1_DevStructure.sEEAddress = 0xA0; // set EEPROM address to 0xA0 
         sEE1_DevStructure.sEEPageSize = 32;  // set page size to 32
         sEE1_DevStructure.sEEMemoryAddrMode = sEE_OPT_16BIT_REG; // enable 16Bit memory addressing mode
                           
      -2- DEVICE CONFIGURATION 
      Call the function sEE_StructInit() to initialize I2Cx CPAL device structure 
      relative to EEPROM than call sEE_Init() to configure the selected device
      with the selected configuration.
      
      -3- READ / WRITE OPERATIONS 
      Call the function sEE_WriteBuffer() or sEE_ReadBuffer() to perform transfer operations. 
      These functions start data transfer and exit. sEE_WriteHandler() and sEE_ReadHandler()
      handle the remainder of the communication. sEE_WriteHandler() must be called in 
      CPAL_I2C_TXTC_UserCallback() and sEE_ReadHandler() must be called in CPAL_I2C_RXTC_UserCallback().
      These two callbacks are implemented in "cpal_usercallback.c" file.      
           
      Example of how to implement sEE_WriteHandler() in CPAL_I2C_TXTC_UserCallback():
      
            1** Comment "#define CPAL_I2C_TXTC_UserCallback   (void)" in cpal_conf.h.
            2** Implement CPAL_I2C_TXTC_UserCallback in "cpal_usercallback.c" file.
               
               void CPAL_I2C_TXTC_UserCallback(CPAL_InitTypeDef* pDevInitStruct)
               {
                 sEE_WriteHandler(pDevInitStruct->CPAL_Dev);
               }
      
      User should monitor transfer by testing the value of sEEState parameter of
      sEEx_DevStructure. When transfer is ongoing sEEState is equal to sEE_STATE_WRITING
      or sEE_STATE_READING. After transfer complete this parameter is set to sEE_STATE_IDLE.
      
      Example of how to wait until EEPROM communication finishes:
            
            while(sEE_GetEepromState(&sEE_DevStructure) != sEE_STATE_IDLE)
              {
                //Application may perform other tasks while transfer operation is ongoing 
              }
      
      -4- DEVICE DEINITIALIZATION 
      When transfer operations are finished, you may call sEE_DeInit() to disable I2Cx device 
      and related resources (GPIO, DMA , IT and NVIC) relative to used EEPROM. 
      
*****************************END OF User Notes**********************************************/
 
/* Includes ------------------------------------------------------------------*/
#include "stm32f30x_i2c_cpal.h"
   
/* Exported types ------------------------------------------------------------*/

/*========= sEE_State_Enum =========*/
/* sEE global State enumeration contains the current state of EEPROM. 
   Before starting each operation this state is tested. After each operation 
   sEE_State is updated with the new value resulting from the relative operation.*/

 typedef enum
{ 
  sEE_STATE_IDLE        = 0x01,         /*!<This state indicates that the EEPROM device is in idle state */ 

  sEE_STATE_WRITING     = 0x02,         /*!<This state indicates that write operation is ongoing */

  sEE_STATE_READING     = 0x03,         /*!<This state indicates that read operation is ongoing */
  
  sEE_STATE_ERROR       = 0x04,         /*!<This state indicates that an error is occurred during 
                                            last operation */
  
}sEE_StateTypeDef;


/*========= CPAL_sEE_TypeDef =========*/
/* sEE Device structure definition */
typedef struct  
{
  CPAL_InitTypeDef* sEE_CPALStructure;  /*!< Pointer on a CPAL Device structure relative to the device 
                                             instantiated to communicate with EEPROM */
  
  uint16_t sEEPageSize;                 /*!< Contains the page size of EEPROM Memory*/ 
  
  uint8_t sEEAddress;                   /*!< Contains the EEPROM device Address */
  
  
  uint8_t sEEMemoryAddrMode;            /*!< Bit-field value specifying Memory Addressing Mode. Can be 
                                             any combination of following values: 
                                             sEE_Memory_Addressing_Mode_Defines */ 
  
  __IO sEE_StateTypeDef sEEState;       /*!< Holds the current State of the EEPROM device. The state 
                                             parameter can be one of the following values: sEE_State_Enum  */
  
} sEE_InitTypeDef;   


/*========= sEE_Global_Device_Structures =========*/
/* sEE Global Device Structures are the Global default structures which 
   are used to handle sEE configuration and status.*/ 
   
extern sEE_InitTypeDef* sEE_DevStructures[];

#ifdef CPAL_USE_I2C1 
extern sEE_InitTypeDef sEE1_DevStructure;
#endif /* CPAL_USE_I2C1 */

#ifdef CPAL_USE_I2C2 
extern sEE_InitTypeDef sEE2_DevStructure;
#endif /* CPAL_USE_I2C2 */

/* Exported constants --------------------------------------------------------*/

/*============== TIMING Configuration ==========================*/
/* I2C TIMING Register define */
/* I2C TIMING is calculated in case of the I2C Clock source is the SYSCLK = 72 MHz */
/* When using sEE_M24M01 set TIMING to 0x00C4092A to reach 1 MHz speed (Rise time = 26ns, Fall time = 2ns) */
/* When using sEE_M24LR64 set TIMING to 0xC062121F to reach 100 KHz speed (Rise time = 640ns, Fall time = 20ns) */

#define sEE_I2C_TIMING             0xC062121F


/*============== Programming model Configuration ==========================*/
/* Select interrupt programming model : By default DMA programming model is selected.
 To select interrupt programming model uncomment this define */
//#define sEE_IT

/*========= sEE_Memory_Addressing_Mode_Defines =========*/
/* sEE Memory_Addressing_Mode defines can be affected to sEEMemoryAddrMode which is 
   a bit-field argument so any combination of these parameters can be selected. 
   If one option is not selected then the relative feature is disabled.*/
   

#define sEE_OPT_NO_MEM_ADDR            ((uint8_t)0x01)  /*!<Enable No Memory addressing mode for read operation : only EEPROM 
                                                            device address sent. No Register/Physical address to be sent after 
                                                            addressing phase */  

#define sEE_OPT_16BIT_REG              ((uint8_t)0x02)  /*!<Enable 16-Bit Register/Physical addressing mode (two bytes, 
                                                            MSB first). This option is supported only when sEE_OPT_NO_MEM_ADDR 
                                                            option is not set */ 



/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

void sEE_DeInit(sEE_InitTypeDef* sEEInitStruct);
void sEE_Init(sEE_InitTypeDef* sEEInitStruct);
void sEE_StructInit(sEE_InitTypeDef* sEEInitStruct);
uint32_t sEE_WriteBuffer(sEE_InitTypeDef* sEEInitStruct, uint8_t* pBuffer, uint16_t WriteAddr, uint32_t NumByteToWrite);
uint32_t sEE_WriteHandler(CPAL_DevTypeDef Device);
uint32_t sEE_ReadBuffer(sEE_InitTypeDef* sEEInitStruct, uint8_t* pBuffer, uint16_t ReadAddr, uint32_t NumByteToRead);
uint32_t sEE_ReadHandler(CPAL_DevTypeDef Device);
uint32_t sEE_WaitEepromStandbyState(CPAL_DevTypeDef Device);
uint32_t sEE_GetEepromState(sEE_InitTypeDef* sEEInitStruct);


#ifdef __cplusplus
}
#endif

#endif /* __STM32303C_EVAL_I2C_EE_CPAL_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
