/**
  ******************************************************************************
  * @file    stm32303c_eval_i2c_ee_cpal.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    23-October-2012
  * @brief   This file provides a set of functions needed to manage I2C  
  *          EEPROM memory. 
  *            
  *          ===================================================================
  *          Notes: 
  *           - This driver is intended for STM32F30x families devices only.
  *           - The I2C EEPROM memory (M24LR64) is available in RF EEPROM daughter 
  *             board (ANT7-M24LR-A) provided with the EVAL board, to use this 
  *             driver you have to connect the ANT7-M24LR-A to CN1 connector.   
  *          ===================================================================  
  *
  *          It implements a high level communication layer for read and write 
  *          from/to this memory. 
  *        
  *          @note This file has been updated to use the CPAL library drivers 
  *                 instead of the Standard peripheral drivers.
  *
  *          @note In this driver, basic read and write functions (sEE_ReadBuffer() 
  *                and sEE_WritePage()) use the DMA or Interrupt  to perform the 
  *                data transfer to/from EEPROM memory.
  *                Thus, after calling these two functions, user application may 
  *                perform other tasks while data transfer is ongoing. 
  *                The application should then monitor the variable holding 
  *                the state of EEPROM in order to determine when the transfer is
  *                completed . Stopping transfer tasks are performed into DMA or I2C                  
  *                interrupt handlers (which are integrated into this driver).
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

/* Includes ------------------------------------------------------------------*/
#include "stm32303c_eval_i2c_ee_cpal.h"


/* Private typedef -----------------------------------------------------------*/

/*========= sEE_Write_TypeDef =========*/
/* sEE Write parameter structure definition */   

typedef struct  
{
  __IO uint32_t sEEDataNum;             /*!< The number of data that will be written in next transfer */ 
  
  uint32_t sEEWriteAddr;                /*!< Physical memory address of EEPROM where data will be written */ 
  
  __IO uint8_t *sEEpBuffer;             /*!< The address of the buffer from which data transfer should start */
  
  __IO uint16_t sEENumOfPage;           /*!< The number of page that will be written  */  
  
  __IO uint8_t sEENumOfSingle;          /*!< The number of single data that will be written */ 
  
  __IO uint8_t sEENextWrite;            /*!< This member indicates there is remaining transfers  */   
  
} sEE_WriteTypeDef; 

/* Private defines -----------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/*========= Local Structures declaration =========*/

#ifdef CPAL_USE_I2C1 
 sEE_InitTypeDef sEE1_DevStructure = {&I2C1_DevStructure, 0, 0, 0, sEE_STATE_IDLE};          /* Initialize All structure parameters to 0 */  
#endif /* CPAL_USE_I2C1 */

#ifdef CPAL_USE_I2C2 
 sEE_InitTypeDef sEE2_DevStructure = {&I2C2_DevStructure, 0, 0, 0, sEE_STATE_IDLE};          /* Initialize All structure parameters to 0 */  
#endif /* CPAL_USE_I2C2 */

sEE_InitTypeDef* sEE_DevStructures[CPAL_I2C_DEV_NUM] =
{
#ifdef CPAL_USE_I2C1 
  &sEE1_DevStructure,
#else
  pNULL,
#endif

#ifdef CPAL_USE_I2C2 
  &sEE2_DevStructure,
#else
  pNULL,
#endif
};

#ifdef CPAL_USE_I2C1 
 sEE_WriteTypeDef sEE1_WriteStructure = {0, 0, pNULL, 0, 0, 0};          /* Initialize All structure parameters to 0 */  
#endif /* CPAL_USE_I2C1 */

#ifdef CPAL_USE_I2C2 
 sEE_WriteTypeDef sEE2_WriteStructure = {0, 0, pNULL, 0, 0, 0};          /* Initialize All structure parameters to 0 */  
#endif /* CPAL_USE_I2C2 */

sEE_WriteTypeDef* sEE_WriteStructures[CPAL_I2C_DEV_NUM] =
{
#ifdef CPAL_USE_I2C1 
  &sEE1_WriteStructure,
#else
  pNULL,
#endif

#ifdef CPAL_USE_I2C2 
  &sEE2_WriteStructure,
#else
  pNULL,
#endif
};

#ifdef CPAL_USE_I2C1 
CPAL_TransferTypeDef  sEE1_TXTransfer = { 
                        /* Initialize TX Transfer structure */
                        pNULL,
                        0,
                        0,
                        0};

CPAL_TransferTypeDef  sEE1_RXTransfer = {
                        /* Initialize RX Transfer structure */
                        pNULL,
                        0,
                        0,
                        0};
#endif /* CPAL_USE_I2C1 */

#ifdef CPAL_USE_I2C2 
CPAL_TransferTypeDef  sEE2_TXTransfer = { 
                        /* Initialize TX Transfer structure */
                        pNULL,
                        0,
                        0,
                        0};

CPAL_TransferTypeDef  sEE2_RXTransfer = {
                        /* Initialize RX Transfer structure */
                        pNULL,
                        0,
                        0,
                        0};
#endif /* CPAL_USE_I2C2 */

CPAL_TransferTypeDef* sEE_TXTransfer[CPAL_I2C_DEV_NUM] =
{
#ifdef CPAL_USE_I2C1 
  &sEE1_TXTransfer,
#else
  pNULL,
#endif

#ifdef CPAL_USE_I2C2 
  &sEE2_TXTransfer,
#else
  pNULL,
#endif
};


CPAL_TransferTypeDef* sEE_RXTransfer[CPAL_I2C_DEV_NUM] =
{
#ifdef CPAL_USE_I2C1 
  &sEE1_RXTransfer,
#else
  pNULL,
#endif

#ifdef CPAL_USE_I2C2 
  &sEE2_RXTransfer,
#else
  pNULL,
#endif
};

/**
  * @}
  */ 

/* Private function prototypes -----------------------------------------------*/


static uint32_t sEE_WritePage(sEE_InitTypeDef* sEEInitStruct, uint8_t* pBuffer, \
                              uint16_t WriteAddr, uint32_t NumByteToWrite);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  DeInitialize peripherals used by the I2C EEPROM driver.
  * @param  sEEInitStruct : Pointer to sEE Device structure
  * @retval None
  */
void sEE_DeInit(sEE_InitTypeDef* sEEInitStruct)
{ 
  /* Deinitialize CPAL peripheral */
  CPAL_I2C_DeInit(sEEInitStruct->sEE_CPALStructure);
}

/**
  * @brief  Initialize peripherals used by the I2C EEPROM driver.
  * @param  sEEInitStruct : Pointer to sEE Device structure
  * @retval None
  */
void sEE_Init(sEE_InitTypeDef* sEEInitStruct)
{ 
  /* Initialize CPAL peripheral */
  CPAL_I2C_Init(sEEInitStruct->sEE_CPALStructure);  
}

/**
  * @brief  Initialize sEE CPAL Structure used by the I2C EEPROM driver.
  * @param  sEEInitStruct : Pointer to sEE Device structure
  * @retval None
  */
void sEE_StructInit(sEE_InitTypeDef* sEEInitStruct)
{   
  /* Set CPAL structure parameters to their default values */  
  CPAL_I2C_StructInit(sEEInitStruct->sEE_CPALStructure);
  
  /* Set I2C clock speed */
  sEEInitStruct->sEE_CPALStructure->pCPAL_I2C_Struct->I2C_Timing = sEE_I2C_TIMING;
  
  sEEInitStruct->sEE_CPALStructure->wCPAL_Options = 0;

#ifdef sEE_IT
  /* Select Interrupt programming model and disable all options */
  sEEInitStruct->sEE_CPALStructure->CPAL_ProgModel =  CPAL_PROGMODEL_INTERRUPT; 
#else
  /* Select DMA programming model and activate TX_DMA_TC and RX_DMA_TC interrupts */
  sEEInitStruct->sEE_CPALStructure->CPAL_ProgModel =  CPAL_PROGMODEL_DMA;
#endif /* sEE_IT */
  
}

/**
  * @brief  Reads a block of data from the EEPROM.
  * @param  sEEInitStruct : Pointer to sEE Device structure
  * @param  pBuffer : pointer to the buffer that receives the data read from 
  *         the EEPROM.
  * @param  ReadAddr : EEPROM's internal address to read from.
  * @param  NumByteToRead : pointer to the variable holding number of bytes to 
  *         read from the EEPROM. 
  * 
  *        @note The variable pointed by NumByteToRead is reset to 0 when all the 
  *              data are read from the EEPROM. Application should monitor this 
  *              variable in order know when the transfer is complete.
  * 
  * @note When number of data to be read is higher than 1, this function just 
  *       configure the communication and enable the DMA channel to transfer data.
  *       Meanwhile, the user application may perform other tasks.
  *       When number of data to be read is 1, then the DMA is not used.
  * 
  * @retval None
  */
uint32_t sEE_ReadBuffer(sEE_InitTypeDef* sEEInitStruct, uint8_t* pBuffer, \
                        uint16_t ReadAddr, uint32_t NumByteToRead)
{  
  if (sEEInitStruct->sEEState == sEE_STATE_IDLE)
  {
    sEEInitStruct->sEEState = sEE_STATE_READING;
    
    sEEInitStruct->sEE_CPALStructure->wCPAL_Options = 0;

 /* Enable 16Bit memory register option on CPAL */
  if (sEEInitStruct->sEEMemoryAddrMode & sEE_OPT_16BIT_REG)
  {
    sEEInitStruct->sEE_CPALStructure->wCPAL_Options = CPAL_OPT_16BIT_REG;
  }
  
   /* Enable no memory addressing mode option on CPAL */
  if (sEEInitStruct->sEEMemoryAddrMode & sEE_OPT_NO_MEM_ADDR)
  {
    sEEInitStruct->sEE_CPALStructure->wCPAL_Options |= CPAL_OPT_NO_MEM_ADDR;
  }
    
    sEEInitStruct->sEE_CPALStructure->pCPAL_TransferRx   = sEE_RXTransfer[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]; 
    
    sEEInitStruct->sEE_CPALStructure->pCPAL_TransferRx->wNumData = (uint32_t)(NumByteToRead);
    sEEInitStruct->sEE_CPALStructure->pCPAL_TransferRx->pbBuffer = pBuffer;
    sEEInitStruct->sEE_CPALStructure->pCPAL_TransferRx->wAddr1   = (uint32_t)((uint8_t)sEEInitStruct->sEEAddress);
    sEEInitStruct->sEE_CPALStructure->pCPAL_TransferRx->wAddr2   = (uint32_t)((uint16_t)ReadAddr);
    
    return CPAL_I2C_Read(sEEInitStruct->sEE_CPALStructure);  
  }
  else
  {
    return CPAL_FAIL;
  }
}

/**
  * @brief  Writes buffer of data to the I2C EEPROM.
  * @param  sEEInitStruct : Pointer to sEE Device structure
  * @param  pBuffer : pointer to the buffer  containing the data to be written 
  *         to the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : number of bytes to write to the EEPROM.
  * @retval None
  */
uint32_t sEE_WriteBuffer(sEE_InitTypeDef* sEEInitStruct, uint8_t* pBuffer, \
                         uint16_t WriteAddr, uint32_t NumByteToWrite)
{   
  uint32_t  DataNum = 0;
  uint16_t count = 0;
  uint16_t Addr = 0;
  
  if (sEEInitStruct->sEEState == sEE_STATE_IDLE)
  {
    sEEInitStruct->sEEState = sEE_STATE_WRITING;
    
    sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEEDataNum = 0;
    sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEEWriteAddr = 0;
    sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEEpBuffer = pNULL;
    sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEENumOfPage = 0;
    sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEENextWrite = 0;
    sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEENumOfSingle = 0;
    
    /* if one data will be written */
    if (NumByteToWrite == 1)
    { 
      /* Transfer complete */
      sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEENextWrite = 0;
      
      /* update number od data for write */
      DataNum = NumByteToWrite;
      
      if(sEE_WritePage(sEEInitStruct, pBuffer, WriteAddr, DataNum) != CPAL_PASS)
      {    
        return CPAL_FAIL;
      }      
    }
    /* Use Write page */
    else
    {            
      /* if Address aligned reset count value to 0 */
      Addr = WriteAddr % sEEInitStruct->sEEPageSize;
      
      if (Addr == 0)
      {
        count = 0;
      }
      else
      { 
       count = sEEInitStruct->sEEPageSize - Addr;
       
       if (NumByteToWrite <= count)
        {
          count = NumByteToWrite;
        }        
      }
      
      /* Get Number of page for write and number of single byte */
      sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEENumOfPage = \
                                 (uint16_t)((NumByteToWrite - count) / sEEInitStruct->sEEPageSize);
      sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEENumOfSingle = \
                                  (uint8_t)((NumByteToWrite - count) % sEEInitStruct->sEEPageSize);
           
      /* If WriteAddr is sEE_PAGESIZE is not aligned  */
      if (Addr != 0) 
      {
        /* Update Number of data to write */
        sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEEDataNum = count; 
      }    
      /* If WriteAddr is sEE_PAGESIZE is aligned  */
      else
      {
        /* if only single byte must be written */
        if (sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEENumOfPage == 0)
        {
          /* update number of data to write */
          sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEEDataNum = \
            sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEENumOfSingle;
          
          /* reset number of single */
          sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEENumOfSingle = 0;
        }
        else
        {
          /* update number of data to write */
          sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEEDataNum =  (uint32_t)((uint16_t)sEEInitStruct->sEEPageSize); 
          
          /* update number of page */
          sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEENumOfPage--;
        }      
      }
      
      /* update global variable */
      DataNum = sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEEDataNum;
      sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEEWriteAddr = (uint32_t)((uint16_t)WriteAddr);
      sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEEpBuffer = pBuffer;
            
      /* If there are remaining data to transfer */
      if ((sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEENumOfPage != 0) 
          || (sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEENumOfSingle != 0))
      {     
        /* update global variable */
        sEE_WriteStructures[sEEInitStruct->sEE_CPALStructure->CPAL_Dev]->sEENextWrite = 1;         
      } 
           
      /* Write data on EEPROM */
      if (sEE_WritePage(sEEInitStruct, pBuffer, WriteAddr, DataNum) != CPAL_PASS)
      {
        return CPAL_FAIL;
      }     
      
    }  
    return CPAL_PASS;
  }
  else
  {
    return CPAL_FAIL;
  }
}

/**
  * @brief  Handle EEPROM Write operation 
  * @param  Device : sEE CPAL device instance
  * @retval None
  */
uint32_t sEE_WriteHandler(CPAL_DevTypeDef Device)      
{  
  uint32_t  DataNum = 0;
  
  /* wait until EEPROM ready for transfer */
  while (sEE_WaitEepromStandbyState(Device) == CPAL_FAIL);
  
  /* if there are remaining data for write */
  if (sEE_WriteStructures[Device]->sEENextWrite != 0)
  {  
    sEE_WriteStructures[Device]->sEEWriteAddr += sEE_WriteStructures[Device]->sEEDataNum;
    sEE_WriteStructures[Device]->sEEpBuffer += sEE_WriteStructures[Device]->sEEDataNum;
    sEE_WriteStructures[Device]->sEENextWrite = 0;
        
    /* if page must be written in EEPROM */
    if(sEE_WriteStructures[Device]->sEENumOfPage != 0)
    {
      sEE_WriteStructures[Device]->sEEDataNum = (uint32_t)((uint16_t)sEE_DevStructures[Device]->sEEPageSize);
      sEE_WriteStructures[Device]->sEENumOfPage--;
    }
    /* if single byte must be written in EEPROM */
    else if (sEE_WriteStructures[Device]->sEENumOfSingle != 0)
    {
      sEE_WriteStructures[Device]->sEEDataNum = (uint32_t)((uint8_t)sEE_WriteStructures[Device]->sEENumOfSingle);
      sEE_WriteStructures[Device]->sEENumOfSingle = 0;
      sEE_WriteStructures[Device]->sEENextWrite = 0;
    }
    
    /* update number of date for write */
    DataNum = sEE_WriteStructures[Device]->sEEDataNum;
    
     /* if another data must be written */
    if ((sEE_WriteStructures[Device]->sEENumOfPage != 0) 
        || (sEE_WriteStructures[Device]->sEENumOfSingle != 0))
    {      
      sEE_WriteStructures[Device]->sEENextWrite = 1;
    }   
    
    /* write data in EEPROM */
    sEE_WritePage(sEE_DevStructures[Device],(uint8_t*)sEE_WriteStructures[Device]->sEEpBuffer, \
                                            sEE_WriteStructures[Device]->sEEWriteAddr, DataNum); 
  }
  else
  {
    if (sEE_DevStructures[Device]->sEEState != sEE_STATE_ERROR)
    {
      /* Reset EEPROM State */
      sEE_DevStructures[Device]->sEEState = sEE_STATE_IDLE;
    }
  }
  
  return CPAL_PASS;
}


/**
  * @brief  Handle EEPROM Read operation 
  * @param  Device : sEE CPAL device instance
  * @retval None
  */
uint32_t sEE_ReadHandler(CPAL_DevTypeDef Device)      
{  
  if (sEE_DevStructures[Device]->sEEState != sEE_STATE_ERROR)
  {
    /* Reset EEPROM State */
    sEE_DevStructures[Device]->sEEState = sEE_STATE_IDLE;
  }
  
  return CPAL_PASS;
}


/**
  * @brief  Writes more than one byte to the EEPROM with a single WRITE cycle.
  * @note   The number of byte can't exceed the EEPROM page size.
  * @param  sEEInitStruct : Pointer to sEE Device structure
  * @param  pBuffer : pointer to the buffer containing the data to be written to 
  *         the EEPROM.
  * @param  WriteAddr : EEPROM's internal address to write to.
  * @param  NumByteToWrite : pointer to the variable holding number of bytes to 
  *         written to the EEPROM. 
  * 
  *        @note The variable pointed by NumByteToWrite is reset to 0 when all the 
  *              data are read from the EEPROM. Application should monitor this 
  *              variable in order know when the transfer is complete.
  * 
  * @note When number of data to be written is higher than 1, this function just 
  *       configure the communication and enable the DMA channel to transfer data.
  *       Meanwhile, the user application may perform other tasks.
  *       When number of data to be written is 1, then the DMA is not used.
  * 
  * @retval None
  */
static uint32_t sEE_WritePage(sEE_InitTypeDef* sEEInitStruct, uint8_t* pBuffer, \
                              uint16_t WriteAddr, uint32_t NumByteToWrite)
{     
  sEEInitStruct->sEE_CPALStructure->wCPAL_Options = 0;
  
  /* Enable 16Bit memory register option on CPAL */
  if (sEEInitStruct->sEEMemoryAddrMode & sEE_OPT_16BIT_REG)
  {
    sEEInitStruct->sEE_CPALStructure->wCPAL_Options = CPAL_OPT_16BIT_REG;
  }
  
  sEEInitStruct->sEE_CPALStructure->pCPAL_TransferTx   = sEE_TXTransfer[sEEInitStruct->sEE_CPALStructure->CPAL_Dev];  
  
  /* Configure transfer parameters */  
  sEEInitStruct->sEE_CPALStructure->pCPAL_TransferTx->wNumData = (uint32_t)(NumByteToWrite);
  sEEInitStruct->sEE_CPALStructure->pCPAL_TransferTx->pbBuffer = pBuffer;
  sEEInitStruct->sEE_CPALStructure->pCPAL_TransferTx->wAddr1   = (uint32_t)((uint8_t)sEEInitStruct->sEEAddress);
  sEEInitStruct->sEE_CPALStructure->pCPAL_TransferTx->wAddr2   = (uint32_t)((uint16_t)WriteAddr);
  
  /* Write Operation */
  return CPAL_I2C_Write(sEEInitStruct->sEE_CPALStructure);
  
}

/**
  * @brief  Wait for EEPROM Standby state
  * @param  Device : sEE CPAL device instance
  * @retval None
  */
uint32_t sEE_WaitEepromStandbyState(CPAL_DevTypeDef Device)      
{   
  sEE_DevStructures[Device]->sEE_CPALStructure->pCPAL_TransferTx = sEE_TXTransfer[Device];     
  sEE_DevStructures[Device]->sEE_CPALStructure->pCPAL_TransferTx->wAddr1 = \
                            (uint32_t)((uint8_t)sEE_DevStructures[Device]->sEEAddress);
  
  return CPAL_I2C_IsDeviceReady(sEE_DevStructures[Device]->sEE_CPALStructure);
}


/**
  * @brief  Wait for EEPROM Standby state
  * @param  sEEInitStruct : Pointer to sEE Device structure
  * @retval None
  */
uint32_t sEE_GetEepromState(sEE_InitTypeDef* sEEInitStruct)      
{       
  return sEEInitStruct->sEEState;
}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
