/**
  ******************************************************************************
  * @file    stm3210e_eval_i2c_tsensor.c
  * @author  MCD Application Team
  * @version V5.1.0
  * @date    18-January-2013
  * @brief   This file provides a set of functions needed to manage the I2C LM75 
  *          temperature sensor mounted on STM3210E-EVAL board. 
  *          It implements a high level communication layer for read and write 
  *          from/to this sensor. The needed STM32 hardware resources (I2C and 
  *          GPIO) are defined in stm3210e_eval.h file, and the initialization is 
  *          performed in LM75_LowLevel_Init() function declared in stm3210e_eval.c 
  *          file.
  *            
  *          Note:
  *          -----    
  *          This driver uses the DMA method to send and receive data on I2C bus,
  *          which allows higher efficiency and reliability  of the communication.
  *                 
  *          You can easily tailor this driver to any other development board, 
  *          by just adapting the defines for hardware resources and 
  *          LM75_LowLevel_Init() function.
  *
  *     +-----------------------------------------------------------------+
  *     |                        Pin assignment                           |                 
  *     +---------------------------------------+-----------+-------------+
  *     |  STM32 I2C Pins                       |   STLM75  |   Pin       |
  *     +---------------------------------------+-----------+-------------+
  *     | LM75_I2C_SDA_PIN/ SDA                 |   SDA     |    1        |
  *     | LM75_I2C_SCL_PIN/ SCL                 |   SCL     |    2        |
  *     | LM75_I2C_SMBUSALERT_PIN/ SMBUS ALERT  |   OS/INT  |    3        |
  *     | .                                     |   GND     |    4  (0V)  |
  *     | .                                     |   GND     |    5  (0V)  |
  *     | .                                     |   GND     |    6  (0V)  |
  *     | .                                     |   GND     |    7  (0V)  |
  *     | .                                     |   VDD     |    8  (3.3V)|
  *     +---------------------------------------+-----------+-------------+
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
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
#include "stm3210e_eval_i2c_tsensor.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM3210E_EVAL
  * @{
  */
  
/** @addtogroup STM3210E_EVAL_I2C_TSENSOR
  * @brief      This file includes the LM75 Temperature Sensor driver of 
  *             STM3210E-EVAL board.
  * @{
  */ 

/** @defgroup STM3210E_EVAL_I2C_TSENSOR_Private_Types
  * @{
  */ 
/**
  * @}
  */

/** @defgroup STM3210E_EVAL_I2C_TSENSOR_Private_Defines
  * @{
  */ 
#define LM75_SD_SET                0x01 /*!< Set SD bit in the configuration register */
#define LM75_SD_RESET              0xFE /*!< Reset SD bit in the configuration register */
/**
  * @}
  */ 

/** @defgroup STM3210E_EVAL_I2C_TSENSOR_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 
  
/** @defgroup STM3210E_EVAL_I2C_TSENSOR_Private_Variables
  * @{
  */ 
  
__IO uint32_t  LM75_Timeout = LM75_LONG_TIMEOUT; 
/**
  * @}
  */ 

/** @defgroup STM3210E_EVAL_I2C_TSENSOR_Private_Function_Prototypes
  * @{
  */ 
static void LM75_DMA_Config(LM75_DMADirection_TypeDef Direction, uint8_t* buffer, uint8_t NumData);

/**
  * @}
  */ 


/** @defgroup STM3210E_EVAL_I2C_TSENSOR_Private_Functions
  * @{
  */ 

/**
  * @brief  DeInitializes the LM75_I2C.
  * @param  None
  * @retval None
  */
void LM75_DeInit(void)
{
  LM75_LowLevel_DeInit();
}

/**
  * @brief  Initializes the LM75_I2C.
  * @param  None
  * @retval None
  */
void LM75_Init(void)
{
  I2C_InitTypeDef   I2C_InitStructure;
  
  LM75_LowLevel_Init();
  
  I2C_DeInit(LM75_I2C);

  /*!< LM75_I2C Init */
  I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusHost;
  I2C_InitStructure.I2C_DutyCycle = I2C_DutyCycle_2;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_ClockSpeed = LM75_I2C_SPEED;
  I2C_Init(LM75_I2C, &I2C_InitStructure);

  /*!< Enable SMBus Alert interrupt */
  I2C_ITConfig(LM75_I2C, I2C_IT_ERR, ENABLE);

  /*!< LM75_I2C Init */
  I2C_Cmd(LM75_I2C, ENABLE);
}


/**
  * @brief  Configure the DMA Peripheral used to handle communication via I2C.
  * @param  None
  * @retval None
  */

static void LM75_DMA_Config(LM75_DMADirection_TypeDef Direction, uint8_t* buffer, uint8_t NumData)
{
  DMA_InitTypeDef DMA_InitStructure;
  
  RCC_AHBPeriphClockCmd(LM75_DMA_CLK, ENABLE);
  
  /* Initialize the DMA_PeripheralBaseAddr member */
  DMA_InitStructure.DMA_PeripheralBaseAddr = LM75_I2C_DR;
  /* Initialize the DMA_MemoryBaseAddr member */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)buffer;
   /* Initialize the DMA_PeripheralInc member */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  /* Initialize the DMA_MemoryInc member */
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  /* Initialize the DMA_PeripheralDataSize member */
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
  /* Initialize the DMA_MemoryDataSize member */
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
  /* Initialize the DMA_Mode member */
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
  /* Initialize the DMA_Priority member */
  DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
  /* Initialize the DMA_M2M member */
  DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
  
  /* If using DMA for Reception */
  if (Direction == LM75_DMA_RX)
  {
    /* Initialize the DMA_DIR member */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    
    /* Initialize the DMA_BufferSize member */
    DMA_InitStructure.DMA_BufferSize = NumData;
    
    DMA_DeInit(LM75_DMA_RX_CHANNEL);
    
    DMA_Init(LM75_DMA_RX_CHANNEL, &DMA_InitStructure);
  }
   /* If using DMA for Transmission */
  else if (Direction == LM75_DMA_TX)
  { 
    /* Initialize the DMA_DIR member */
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    
    /* Initialize the DMA_BufferSize member */
    DMA_InitStructure.DMA_BufferSize = NumData;
    
    DMA_DeInit(LM75_DMA_TX_CHANNEL);
    
    DMA_Init(LM75_DMA_TX_CHANNEL, &DMA_InitStructure);
  }
}


/**
  * @brief  Checks the LM75 status.
  * @param  None
  * @retval ErrorStatus: LM75 Status (ERROR or SUCCESS).
  */
ErrorStatus LM75_GetStatus(void)
{
  uint32_t I2C_TimeOut = I2C_TIMEOUT;

  /*!< Clear the LM75_I2C AF flag */
  I2C_ClearFlag(LM75_I2C, I2C_FLAG_AF);

  /*!< Enable LM75_I2C acknowledgement if it is already disabled by other function */
  I2C_AcknowledgeConfig(LM75_I2C, ENABLE);

  /*---------------------------- Transmission Phase ---------------------------*/

  /*!< Send LM75_I2C START condition */
  I2C_GenerateSTART(LM75_I2C, ENABLE);

  /*!< Test on LM75_I2C EV5 and clear it */
  while ((!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_SB)) && I2C_TimeOut)  /*!< EV5 */
  {
    I2C_TimeOut--;
  }
  if (I2C_TimeOut == 0)
  {
    return ERROR;
  }
  
  I2C_TimeOut = I2C_TIMEOUT;

  /*!< Send STLM75 slave address for write */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);

  while ((!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) && I2C_TimeOut)/* EV6 */
  {
    I2C_TimeOut--;
  }

  if ((I2C_GetFlagStatus(LM75_I2C, I2C_FLAG_AF) != 0x00) || (I2C_TimeOut == 0))
  {
    return ERROR;
  }
  else
  {
    return SUCCESS;
  }
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
  uint8_t LM75_BufferRX[2] ={0,0};
  uint16_t tmp = 0;    
  
   /* Test on BUSY Flag */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BUSY)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Configure DMA Peripheral */
  LM75_DMA_Config(LM75_DMA_RX, (uint8_t*)LM75_BufferRX, 2);  
  
  /* Enable DMA NACK automatic generation */
  I2C_DMALastTransferCmd(LM75_I2C, ENABLE);
  
  /* Enable the I2C peripheral */
  I2C_GenerateSTART(LM75_I2C, ENABLE);
  
  /* Test on SB Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_SB)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send device address for write */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);
  
  /* Test on ADDR Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send the device's internal address to write to */
  I2C_SendData(LM75_I2C, RegName);  
  
  /* Test on TXE FLag (data sent) */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while ((!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BTF)))  
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send START condition a second time */  
  I2C_GenerateSTART(LM75_I2C, ENABLE);
  
  /* Test on SB Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_SB)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send LM75 address for read */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Receiver);
  
  /* Test on ADDR Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))   
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Enable I2C DMA request */
  I2C_DMACmd(LM75_I2C,ENABLE);
  
  /* Enable DMA RX Channel */
  DMA_Cmd(LM75_DMA_RX_CHANNEL, ENABLE);
  
  /* Wait until DMA Transfer Complete */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (!DMA_GetFlagStatus(LM75_DMA_RX_TCFLAG))
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }        
  
  /* Send STOP Condition */
  I2C_GenerateSTOP(LM75_I2C, ENABLE);
  
  /* Disable DMA RX Channel */
  DMA_Cmd(LM75_DMA_RX_CHANNEL, DISABLE);
  
  /* Disable I2C DMA request */  
  I2C_DMACmd(LM75_I2C,DISABLE);
  
  /* Clear DMA RX Transfer Complete Flag */
  DMA_ClearFlag(LM75_DMA_RX_TCFLAG);
  
  /*!< Store LM75_I2C received data */
  tmp = (uint16_t)(LM75_BufferRX[0] << 8);
  tmp |= LM75_BufferRX[1];
  
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
  uint8_t LM75_BufferTX[2] ={0,0};
  LM75_BufferTX[0] = (uint8_t)(RegValue >> 8);
  LM75_BufferTX[1] = (uint8_t)(RegValue);
  
  /* Test on BUSY Flag */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BUSY)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Configure DMA Peripheral */
  LM75_DMA_Config(LM75_DMA_TX, (uint8_t*)LM75_BufferTX, 2);
  
  /* Enable the I2C peripheral */
  I2C_GenerateSTART(LM75_I2C, ENABLE);
  
  /* Test on SB Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_SB) == RESET) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);
  
  /* Test on ADDR Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Transmit the first address for r/w operations */
  I2C_SendData(LM75_I2C, RegName);
  
  /* Test on TXE FLag (data sent) */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while ((!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BTF)))  
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Enable I2C DMA request */
  I2C_DMACmd(LM75_I2C,ENABLE);
  
  /* Enable DMA TX Channel */
  DMA_Cmd(LM75_DMA_TX_CHANNEL, ENABLE);
  
  /* Wait until DMA Transfer Complete */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (!DMA_GetFlagStatus(LM75_DMA_TX_TCFLAG))
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }  
  
  /* Wait until BTF Flag is set before generating STOP */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BTF))  
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send STOP Condition */
  I2C_GenerateSTOP(LM75_I2C, ENABLE);
  
  /* Disable DMA TX Channel */
  DMA_Cmd(LM75_DMA_TX_CHANNEL, DISABLE);
  
  /* Disable I2C DMA request */  
  I2C_DMACmd(LM75_I2C,DISABLE);
  
  /* Clear DMA TX Transfer Complete Flag */
  DMA_ClearFlag(LM75_DMA_TX_TCFLAG);
  
  return LM75_OK;
}

/**
  * @brief  Read Temperature register of LM75: double temperature value.
  * @param  None
  * @retval LM75 measured temperature value.
  */
uint16_t LM75_ReadTemp(void)
{   
  uint8_t LM75_BufferRX[2] ={0,0};
  uint16_t tmp = 0;
  
  /* Test on BUSY Flag */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BUSY)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Configure DMA Peripheral */
  LM75_DMA_Config(LM75_DMA_RX, (uint8_t*)LM75_BufferRX, 2);  
  
  /* Enable DMA NACK automatic generation */
  I2C_DMALastTransferCmd(LM75_I2C, ENABLE);
  
  /* Enable the I2C peripheral */
  I2C_GenerateSTART(LM75_I2C, ENABLE);
  
  /* Test on SB Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_SB)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send device address for write */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);
  
  /* Test on ADDR Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send the device's internal address to write to */
  I2C_SendData(LM75_I2C, LM75_REG_TEMP);  
  
  /* Test on TXE FLag (data sent) */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while ((!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BTF)))  
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send START condition a second time */  
  I2C_GenerateSTART(LM75_I2C, ENABLE);
  
  /* Test on SB Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_SB)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send LM75 address for read */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Receiver);
  
  /* Test on ADDR Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))   
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Enable I2C DMA request */
  I2C_DMACmd(LM75_I2C,ENABLE);
  
  /* Enable DMA RX Channel */
  DMA_Cmd(LM75_DMA_RX_CHANNEL, ENABLE);
  
  /* Wait until DMA Transfer Complete */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (!DMA_GetFlagStatus(LM75_DMA_RX_TCFLAG))
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }        
  
  /* Send STOP Condition */
  I2C_GenerateSTOP(LM75_I2C, ENABLE);
  
  /* Disable DMA RX Channel */
  DMA_Cmd(LM75_DMA_RX_CHANNEL, DISABLE);
  
  /* Disable I2C DMA request */  
  I2C_DMACmd(LM75_I2C,DISABLE);
  
  /* Clear DMA RX Transfer Complete Flag */
  DMA_ClearFlag(LM75_DMA_RX_TCFLAG);
  
  /*!< Store LM75_I2C received data */
  tmp = (uint16_t)(LM75_BufferRX[0] << 8);
  tmp |= LM75_BufferRX[1];    
  
  /*!< Return Temperature value */
  return (uint16_t)(tmp >> 7);
}

/**
  * @brief  Read the configuration register from the LM75.
  * @param  None
  * @retval LM75 configuration register value.
  */
uint8_t LM75_ReadConfReg(void)
{   
  uint8_t LM75_BufferRX[2] ={0,0}; 
  
  /* Test on BUSY Flag */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BUSY)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Configure DMA Peripheral */
  LM75_DMA_Config(LM75_DMA_RX, (uint8_t*)LM75_BufferRX, 2);  
  
  /* Enable DMA NACK automatic generation */
  I2C_DMALastTransferCmd(LM75_I2C, ENABLE);
  
  /* Enable the I2C peripheral */
  I2C_GenerateSTART(LM75_I2C, ENABLE);
  
  /* Test on SB Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_SB)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send device address for write */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);
  
  /* Test on ADDR Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send the device's internal address to write to */
  I2C_SendData(LM75_I2C, LM75_REG_CONF);  
  
  /* Test on TXE FLag (data sent) */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while ((!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BTF)))  
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send START condition a second time */  
  I2C_GenerateSTART(LM75_I2C, ENABLE);
  
  /* Test on SB Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_SB)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send LM75 address for read */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Receiver);
  
  /* Test on ADDR Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))   
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Enable I2C DMA request */
  I2C_DMACmd(LM75_I2C,ENABLE);
  
  /* Enable DMA RX Channel */
  DMA_Cmd(LM75_DMA_RX_CHANNEL, ENABLE);
  
  /* Wait until DMA Transfer Complete */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (!DMA_GetFlagStatus(LM75_DMA_RX_TCFLAG))
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }        
  
  /* Send STOP Condition */
  I2C_GenerateSTOP(LM75_I2C, ENABLE);
  
  /* Disable DMA RX Channel */
  DMA_Cmd(LM75_DMA_RX_CHANNEL, DISABLE);
  
  /* Disable I2C DMA request */  
  I2C_DMACmd(LM75_I2C,DISABLE);
  
  /* Clear DMA RX Transfer Complete Flag */
  DMA_ClearFlag(LM75_DMA_RX_TCFLAG);
  
  /*!< Return Temperature value */
  return (uint8_t)LM75_BufferRX[0];
}

/**
  * @brief  Write to the configuration register of the LM75.
  * @param  RegValue: sepecifies the value to be written to LM75 configuration 
  *         register.
  * @retval None
  */
uint8_t LM75_WriteConfReg(uint8_t RegValue)
{   
  uint8_t LM75_BufferTX = 0;  
  LM75_BufferTX = (uint8_t)(RegValue);
  
  /* Test on BUSY Flag */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BUSY)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Configure DMA Peripheral */
  LM75_DMA_Config(LM75_DMA_TX, (uint8_t*)(&LM75_BufferTX), 1);
  
  /* Enable the I2C peripheral */
  I2C_GenerateSTART(LM75_I2C, ENABLE);
  
  /* Test on SB Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_SB) == RESET) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);
  
  /* Test on ADDR Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Transmit the first address for r/w operations */
  I2C_SendData(LM75_I2C, LM75_REG_CONF);
  
  /* Test on TXE FLag (data sent) */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while ((!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BTF)))  
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Enable I2C DMA request */
  I2C_DMACmd(LM75_I2C,ENABLE);
  
  /* Enable DMA TX Channel */
  DMA_Cmd(LM75_DMA_TX_CHANNEL, ENABLE);
  
  /* Wait until DMA Transfer Complete */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (!DMA_GetFlagStatus(LM75_DMA_TX_TCFLAG))
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }  
  
  /* Wait until BTF Flag is set before generating STOP */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while ((!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BTF)))  
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send STOP Condition */
  I2C_GenerateSTOP(LM75_I2C, ENABLE);
  
  /* Disable DMA TX Channel */
  DMA_Cmd(LM75_DMA_TX_CHANNEL, DISABLE);
  
  /* Disable I2C DMA request */  
  I2C_DMACmd(LM75_I2C,DISABLE);
  
  /* Clear DMA TX Transfer Complete Flag */
  DMA_ClearFlag(LM75_DMA_TX_TCFLAG);  
  
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
  uint8_t LM75_BufferRX[2] ={0,0};
  uint8_t LM75_BufferTX = 0;
  __IO uint8_t RegValue = 0;    
  
  /* Test on BUSY Flag */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BUSY)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Configure DMA Peripheral */
  LM75_DMA_Config(LM75_DMA_RX, (uint8_t*)LM75_BufferRX, 2);  
  
  /* Enable DMA NACK automatic generation */
  I2C_DMALastTransferCmd(LM75_I2C, ENABLE);
  
  /* Enable the I2C peripheral */
  I2C_GenerateSTART(LM75_I2C, ENABLE);
  
  /* Test on SB Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_SB)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send device address for write */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);
  
  /* Test on ADDR Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send the device's internal address to write to */
  I2C_SendData(LM75_I2C, LM75_REG_CONF);  
  
  /* Test on TXE FLag (data sent) */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while ((!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BTF)))  
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send START condition a second time */  
  I2C_GenerateSTART(LM75_I2C, ENABLE);
  
  /* Test on SB Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_SB)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send LM75 address for read */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Receiver);
  
  /* Test on ADDR Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED))   
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Enable I2C DMA request */
  I2C_DMACmd(LM75_I2C,ENABLE);
  
  /* Enable DMA RX Channel */
  DMA_Cmd(LM75_DMA_RX_CHANNEL, ENABLE);
  
  /* Wait until DMA Transfer Complete */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (!DMA_GetFlagStatus(LM75_DMA_RX_TCFLAG))
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }        
  
  /* Send STOP Condition */
  I2C_GenerateSTOP(LM75_I2C, ENABLE);
  
  /* Disable DMA RX Channel */
  DMA_Cmd(LM75_DMA_RX_CHANNEL, DISABLE);
  
  /* Disable I2C DMA request */  
  I2C_DMACmd(LM75_I2C,DISABLE);
  
  /* Clear DMA RX Transfer Complete Flag */
  DMA_ClearFlag(LM75_DMA_RX_TCFLAG);
  
  /*!< Get received data */
  RegValue = (uint8_t)LM75_BufferRX[0];
  
  /*---------------------------- Transmission Phase ---------------------------*/
  
  /*!< Enable or disable SD bit */
  if (NewState != DISABLE)
  {
    /*!< Enable LM75 */
    LM75_BufferTX = RegValue & LM75_SD_RESET;
  }
  else
  {
    /*!< Disable LM75 */
    LM75_BufferTX = RegValue | LM75_SD_SET;
  }  
  
  /* Test on BUSY Flag */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BUSY)) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Configure DMA Peripheral */
  LM75_DMA_Config(LM75_DMA_TX, (uint8_t*)(&LM75_BufferTX), 1);
  
  /* Enable the I2C peripheral */
  I2C_GenerateSTART(LM75_I2C, ENABLE);
  
  /* Test on SB Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_SB) == RESET) 
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Transmit the slave address and enable writing operation */
  I2C_Send7bitAddress(LM75_I2C, LM75_ADDR, I2C_Direction_Transmitter);
  
  /* Test on ADDR Flag */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while (!I2C_CheckEvent(LM75_I2C, I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED))
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Transmit the first address for r/w operations */
  I2C_SendData(LM75_I2C, LM75_REG_CONF);
  
  /* Test on TXE FLag (data sent) */
  LM75_Timeout = LM75_FLAG_TIMEOUT;
  while ((!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_TXE)) && (!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BTF)))  
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Enable I2C DMA request */
  I2C_DMACmd(LM75_I2C,ENABLE);
  
  /* Enable DMA TX Channel */
  DMA_Cmd(LM75_DMA_TX_CHANNEL, ENABLE);
  
  /* Wait until DMA Transfer Complete */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while (!DMA_GetFlagStatus(LM75_DMA_TX_TCFLAG))
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }  
  
  /* Wait until BTF Flag is set before generating STOP */
  LM75_Timeout = LM75_LONG_TIMEOUT;
  while ((!I2C_GetFlagStatus(LM75_I2C,I2C_FLAG_BTF)))  
  {
    if((LM75_Timeout--) == 0) return LM75_TIMEOUT_UserCallback();
  }
  
  /* Send STOP Condition */
  I2C_GenerateSTOP(LM75_I2C, ENABLE);
  
  /* Disable DMA TX Channel */
  DMA_Cmd(LM75_DMA_TX_CHANNEL, DISABLE);
  
  /* Disable I2C DMA request */  
  I2C_DMACmd(LM75_I2C,DISABLE);
  
  /* Clear DMA TX Transfer Complete Flag */
  DMA_ClearFlag(LM75_DMA_TX_TCFLAG);  
  
  return LM75_OK;
}

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
