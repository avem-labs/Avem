/**
  ******************************************************************************
  * @file    stm32373c_eval_i2c_tsensor_cpal.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2012
  * @brief   This file provides a set of functions needed to manage the I2C LM75 
  *          temperature sensor mounted on STM32373C-EVAL board . 
  *          It implements a high level communication layer for read and write 
  *          from/to this sensor. 
  *            
  *          @note This file is a modified version of stm32373c_eval_i2c_tsensor.c driver;
  *                I2C CPAL library drivers are used instead of the Standard Peripherals
  *                I2C driver.
  *                 
  *
  *     +-----------------------------------------------------------------+
  *     |                        Pin assignment                           |                 
  *     +---------------------------------------+-----------+-------------+
  *     |  STM32 I2C Pins                       |   STLM75  |   Pin       |
  *     +---------------------------------------+-----------+-------------+
  *     | LM75_I2C_SDA_PIN/ SDA                 |   SDA     |    1        |
  *     | LM75_I2C_SCL_PIN/ SCL                 |   SCL     |    2        |
  *     |                                       |   OS/INT  |    3        |
  *     | .                                     |   GND     |    4  (0V)  |
  *     | .                                     |   GND     |    5  (0V)  |
  *     | .                                     |   GND     |    6  (0V)  |
  *     | .                                     |   GND     |    7  (0V)  |
  *     | .                                     |   VDD     |    8  (3.3V)|
  *     +---------------------------------------+-----------+-------------+
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
#include "stm32373c_eval_i2c_tsensor_cpal.h"

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define LM75_SD_SET      0x01 /*!< Set SD bit in the configuration register */
#define LM75_SD_RESET    0xFE /*!< Reset SD bit in the configuration register */

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
  
CPAL_TransferTypeDef  LM75_RXTransfer = { 
                        /* Initialize TX Transfer structure */
                        pNULL,
                        0,
                        0,
                        0};

CPAL_TransferTypeDef  LM75_TXTransfer = {
                        /* Initialize RX Transfer structure */
                        pNULL,
                        0,
                        0,
                        0};

uint8_t LM75_Buffer[2] = {0x00,0x00}; 

extern CPAL_InitTypeDef LM75_DevStructure;


__IO uint32_t  LM75_Timeout = LM75_TIMEOUT; 

/* Private function prototypes -----------------------------------------------*/
static void LM75_StructInit(void);
static void LM75_Init(void);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Configure the LM75_I2C.
  * @param  None
  * @retval None
  */
void LM75_Config(void)
{
  LM75_StructInit ();
  LM75_Init();
}

/**
  * @brief  Deinitialize the LM75_I2C.
  * @param  None
  * @retval None
  */
void LM75_DeInit(void)
{
    /* Initialize CPAL peripheral */
  CPAL_I2C_DeInit(&LM75_DevStructure);
}

/**
  * @brief  Initializes the LM75_I2C.
  * @param  None
  * @retval None
  */
static void LM75_Init(void)
{
  /* Initialize CPAL peripheral */
  CPAL_I2C_Init(&LM75_DevStructure);
}

/**
  * @brief  Initializes the LM75_I2C.
  * @param  None
  * @retval None
  */
static void LM75_StructInit(void)
{
  /* Set CPAL structure parameters to their default values */  
  CPAL_I2C_StructInit(&LM75_DevStructure);
    
  /* Set I2C clock speed */
  LM75_DevStructure.pCPAL_I2C_Struct->I2C_Timing = LM75_I2C_TIMING;
  			 
#ifdef LM75_IT
  /* Select Interrupt programming model */
  LM75_DevStructure.CPAL_ProgModel = CPAL_PROGMODEL_INTERRUPT;
#else
  /* Select DMA programming model */
  LM75_DevStructure.CPAL_ProgModel = CPAL_PROGMODEL_DMA;
#endif /* LM75_IT */  
  
  /* Disable all options */
  LM75_DevStructure.wCPAL_Options  = 0;
  
  /* point to CPAL_TransferTypeDef structure */
  LM75_DevStructure.pCPAL_TransferTx = &LM75_TXTransfer;
  LM75_DevStructure.pCPAL_TransferRx = &LM75_RXTransfer;
}

static uint32_t LM75_Status (void)
{
  LM75_DevStructure.pCPAL_TransferTx = &LM75_TXTransfer;     
  LM75_DevStructure.pCPAL_TransferTx->wAddr1 = (uint32_t)LM75_ADDR;
  
  return CPAL_I2C_IsDeviceReady(&LM75_DevStructure);
}

/**
  * @brief  Checks the LM75 status.
  * @param  None
  * @retval ErrorStatus: LM75 Status (ERROR or SUCCESS).
  */
ErrorStatus LM75_GetStatus(void)
{  
  /* Test if LM75 is ready */
  while ((LM75_Status() == CPAL_FAIL) && LM75_Timeout)  
  {
    LM75_Timeout--;
  }
  
  /* If LM75 is not responding return ERROR */
  if (LM75_Timeout == 0)
  {
    return ERROR;
  }
  
  /* In other case return SUCCESS */
  return SUCCESS;  
}
/**
  * @brief  Read the specified register from the LM75.
  * @param  RegName: specifies the LM75 register to be read.
  *              This member can be one of the following values:  
  *                  - LM75_REG_TEMP: temperature register
  *                  - LM75_REG_TOS: Over-limit temperature register
  *                  - LM75_REG_THYS: Hysteresis temperature register
  * @retval LM75 register value.
  */
uint16_t LM75_ReadReg(uint8_t RegName)
{   
  uint16_t tmp = 0;
  
  LM75_Buffer[0] = 0;
  LM75_Buffer[1] = 0;
  
  /* Disable all options */
  LM75_DevStructure.wCPAL_Options = 0;

  /* point to CPAL_TransferTypeDef structure */
  LM75_DevStructure.pCPAL_TransferRx = &LM75_RXTransfer;
  
  /* Configure transfer parameters */  
  LM75_DevStructure.pCPAL_TransferRx->wNumData = 2;
  LM75_DevStructure.pCPAL_TransferRx->pbBuffer = LM75_Buffer ;
  LM75_DevStructure.pCPAL_TransferRx->wAddr1   = (uint32_t)LM75_ADDR;
  LM75_DevStructure.pCPAL_TransferRx->wAddr2   = (uint32_t)RegName;
  
  /* Read Operation */
  if(CPAL_I2C_Read(&LM75_DevStructure) == CPAL_PASS)
  {
    while ((LM75_DevStructure.CPAL_State != CPAL_STATE_READY) && (LM75_DevStructure.CPAL_State != CPAL_STATE_ERROR) )
    { }
  }
  
  /* Store LM75_I2C received data */
  tmp = (uint16_t)(LM75_Buffer[0] << 8);
  tmp |= LM75_Buffer[1];
  
  /* return a Reg value */
  return (uint16_t)tmp;  
}

/**
  * @brief  Write to the specified register of the LM75.
  * @param  RegName: specifies the LM75 register to be written.
  *              This member can be one of the following values:    
  *                  - LM75_REG_TOS: Over-limit temperature register
  *                  - LM75_REG_THYS: Hysteresis temperature register
  * @param  RegValue: value to be written to LM75 register.  
  * @retval None
  */
uint8_t LM75_WriteReg(uint8_t RegName, uint16_t RegValue)
{   
  LM75_Buffer[0] = (uint8_t)(RegValue >> 8);
  LM75_Buffer[1] = (uint8_t)(RegValue);
     
  /* Disable all options */
  LM75_DevStructure.wCPAL_Options = 0;
  
  /* point to CPAL_TransferTypeDef structure */
  LM75_DevStructure.pCPAL_TransferTx = &LM75_TXTransfer;
  
  /* Configure transfer parameters */  
  LM75_DevStructure.pCPAL_TransferTx->wNumData = 2;
  LM75_DevStructure.pCPAL_TransferTx->pbBuffer = LM75_Buffer ;
  LM75_DevStructure.pCPAL_TransferTx->wAddr1   = (uint32_t)LM75_ADDR;
  LM75_DevStructure.pCPAL_TransferTx->wAddr2   = (uint32_t)RegName;
  
  /* Write Operation */
  if(CPAL_I2C_Write(&LM75_DevStructure) == CPAL_PASS)
  {
    while ((LM75_DevStructure.CPAL_State != CPAL_STATE_READY) && (LM75_DevStructure.CPAL_State != CPAL_STATE_ERROR) )
    { }
    
    if (LM75_DevStructure.CPAL_State == CPAL_STATE_ERROR)
    {
      return LM75_FAIL;
    }
  }
  else
  {
    return LM75_FAIL;
  }
  
  return LM75_OK;
}

/**
  * @brief  Read Temperature register of LM75: double temperature value.
  * @param  None
  * @retval LM75 measured temperature value.
  */
uint16_t LM75_ReadTemp(void)
{   
  uint16_t tmp = 0;
  
  LM75_Buffer[0] = 0;
  LM75_Buffer[1] = 0;
  
  /* Disable all options */
  LM75_DevStructure.wCPAL_Options = 0;

  /* point to CPAL_TransferTypeDef structure */
  LM75_DevStructure.pCPAL_TransferRx = &LM75_RXTransfer;
  
  /* Configure transfer parameters */  
  LM75_DevStructure.pCPAL_TransferRx->wNumData = 2;
  LM75_DevStructure.pCPAL_TransferRx->pbBuffer = LM75_Buffer ;
  LM75_DevStructure.pCPAL_TransferRx->wAddr1   = (uint32_t)LM75_ADDR;
  LM75_DevStructure.pCPAL_TransferRx->wAddr2   = (uint32_t)LM75_REG_TEMP;
  
  /* Read Operation */
  if(CPAL_I2C_Read(&LM75_DevStructure) == CPAL_PASS)
  {
    while ((LM75_DevStructure.CPAL_State != CPAL_STATE_READY) && (LM75_DevStructure.CPAL_State != CPAL_STATE_ERROR) )
    { }
  }
  
  /* Store LM75_I2C received data */
  tmp = (uint16_t)(LM75_Buffer[0] << 8);
  tmp |= LM75_Buffer[1];    
  
  /* Return Temperature value */
  return (uint16_t)(tmp >> 7);
}

/**
  * @brief  Read the configuration register from the LM75.
  * @param  None
  * @retval LM75 configuration register value.
  */
uint8_t LM75_ReadConfReg(void)
{    
  LM75_Buffer[0] = 0;
  
  /* Disable all options */
  LM75_DevStructure.wCPAL_Options = 0;
  
  /* point to CPAL_TransferTypeDef structure */
  LM75_DevStructure.pCPAL_TransferRx = &LM75_RXTransfer;
  
  /* Configure transfer parameters */  
  LM75_DevStructure.pCPAL_TransferRx->wNumData = 1;
  LM75_DevStructure.pCPAL_TransferRx->pbBuffer = LM75_Buffer ;
  LM75_DevStructure.pCPAL_TransferRx->wAddr1   = (uint32_t)LM75_ADDR;
  LM75_DevStructure.pCPAL_TransferRx->wAddr2   = (uint32_t)LM75_REG_CONF;
  
  /* Read Operation */
  if(CPAL_I2C_Read(&LM75_DevStructure) == CPAL_PASS)
  {
    while ((LM75_DevStructure.CPAL_State != CPAL_STATE_READY) && (LM75_DevStructure.CPAL_State != CPAL_STATE_ERROR) )
    { }
  }
  
  /* Return Temperature value */
  return (uint8_t)LM75_Buffer[0];
}

/**
  * @brief  Write to the configuration register of the LM75.
  * @param  RegValue: specifies the value to be written to LM75 configuration 
  *         register.
  * @retval None
  */
uint8_t LM75_WriteConfReg(uint8_t RegValue)
{   
  LM75_Buffer[0] = (uint8_t)(RegValue);
    
  /* Disable all options */
  LM75_DevStructure.wCPAL_Options = 0;
  
  /* point to CPAL_TransferTypeDef structure */
  LM75_DevStructure.pCPAL_TransferTx = &LM75_TXTransfer;
  
  /* Configure transfer parameters */  
  LM75_DevStructure.pCPAL_TransferTx->wNumData = 1;
  LM75_DevStructure.pCPAL_TransferTx->pbBuffer = LM75_Buffer ;
  LM75_DevStructure.pCPAL_TransferTx->wAddr1   = (uint32_t)LM75_ADDR;
  LM75_DevStructure.pCPAL_TransferTx->wAddr2   = (uint32_t)LM75_REG_CONF;
  
  /* Write Operation */
  if(CPAL_I2C_Write(&LM75_DevStructure) == CPAL_PASS)
  {
    while ((LM75_DevStructure.CPAL_State != CPAL_STATE_READY) && (LM75_DevStructure.CPAL_State != CPAL_STATE_ERROR) )
    { }
    
    if (LM75_DevStructure.CPAL_State == CPAL_STATE_ERROR)
    {
      return LM75_FAIL;
    }
  }
  else
  {
    return LM75_FAIL;
  }
  
  return LM75_OK;
  
}

/**
  * @brief  Enables or disables the LM75.
  * @param  NewState: specifies the LM75 new status. This parameter can be ENABLE
  *         or DISABLE.  
  * @retval None
  */
uint8_t LM75_ShutDown(FunctionalState NewState)
{   
  __IO uint8_t RegValue = 0;    
  
  LM75_Buffer[0] = 0;
  		  
  /* Disable all options */
  LM75_DevStructure.wCPAL_Options = 0;

  /* point to CPAL_TransferTypeDef structure */
  LM75_DevStructure.pCPAL_TransferRx = &LM75_RXTransfer;
  
  /* Configure transfer parameters */  
  LM75_DevStructure.pCPAL_TransferRx->wNumData = 1;
  LM75_DevStructure.pCPAL_TransferRx->pbBuffer = LM75_Buffer ;
  LM75_DevStructure.pCPAL_TransferRx->wAddr1   = (uint32_t)LM75_ADDR;
  LM75_DevStructure.pCPAL_TransferRx->wAddr2   = (uint32_t)LM75_REG_CONF;
  
  /* Read Operation */
  if(CPAL_I2C_Read(&LM75_DevStructure) == CPAL_PASS)
  {
    while ((LM75_DevStructure.CPAL_State != CPAL_STATE_READY) && (LM75_DevStructure.CPAL_State != CPAL_STATE_ERROR) )
    { }
  }
  
  /* Get received data */
  RegValue = (uint8_t)LM75_Buffer[0];
  
  /*---------------------------- Transmission Phase ---------------------------*/
  
  /* Enable or disable SD bit */
  if (NewState != DISABLE)
  {
    /* Enable LM75 */
    LM75_Buffer[0] = RegValue & LM75_SD_RESET;
  }
  else
  {
    /* Disable LM75 */
    LM75_Buffer[0] = RegValue | LM75_SD_SET;
  }  
  
  /* point to CPAL_TransferTypeDef structure */
  LM75_DevStructure.pCPAL_TransferTx = &LM75_TXTransfer;
  
  /* Configure transfer parameters */  
  LM75_DevStructure.pCPAL_TransferTx->wNumData = 1;
  LM75_DevStructure.pCPAL_TransferTx->pbBuffer = LM75_Buffer ;
  LM75_DevStructure.pCPAL_TransferTx->wAddr1   = (uint32_t)LM75_ADDR;
  LM75_DevStructure.pCPAL_TransferTx->wAddr2   = (uint32_t)LM75_REG_CONF;
  
  /* Write Operation */
  if(CPAL_I2C_Write(&LM75_DevStructure) == CPAL_PASS)
  {
    while ((LM75_DevStructure.CPAL_State != CPAL_STATE_READY) && (LM75_DevStructure.CPAL_State != CPAL_STATE_ERROR) )
    { }
    
    if (LM75_DevStructure.CPAL_State == CPAL_STATE_ERROR)
    {
      return LM75_FAIL;
    }
  }
  else
  {
    return LM75_FAIL;
  }
  
  return LM75_OK;
}

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
