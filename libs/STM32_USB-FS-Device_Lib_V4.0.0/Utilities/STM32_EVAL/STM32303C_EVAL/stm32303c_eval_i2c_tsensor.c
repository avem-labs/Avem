/**
  ******************************************************************************
  * @file    stm32303c_eval_i2c_tsensor.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    23-October-2012
  * @brief   This file provides a set of functions needed to manage the I2C TS751 
  *          temperature sensor mounted on STM32303C-EVAL board . 
  *          It implements a high level communication layer for read and write 
  *          from/to this sensor. The needed STM323F30x hardware resources (I2C and 
  *          GPIO) are defined in stm32303c_eval.h file, and the initialization is 
  *          performed in TS751_LowLevel_Init() function declared in stm32303c_eval.c 
  *          file.
  *          You can easily tailor this driver to any other development board, 
  *          by just adapting the defines for hardware resources and 
  *          TS751_LowLevel_Init() function.
  *
  *     +--------------------------------------------------------------------+
  *     |                        Pin assignment                              |                 
  *     +---------------------------------------+-----------+----------------+
  *     |  STM32F30x I2C Pins                   |   STTS751     |   Pin      |
  *     +---------------------------------------+-----------+----------------+
  *     | TS751_I2C_SDA_PIN/ SDA                 |   Addr/Therm |    1       |
  *     | TS751_I2C_SCL_PIN/ SCL                 |   GND        |    2 (0V)  |
  *     | TS751_I2C_SMBUSALERT_PIN/ SMBUS ALERT  |   VDD        |    3 (3.3V)|
  *     | Addr/Therm pin                         |   SCL        |    4       |
  *     | .                                      |   SMBUS ALERT|    5       |
  *     | .                                      |   SDA        |    6       |
  *     +---------------------------------------+-----------+----------------+
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
#include "stm32303c_eval_i2c_tsensor.h"

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
  * @brief      This file includes the TS751 Temperature Sensor driver of 
  *             STM32303C-EVAL boards.
  * @{
  */ 

/** @defgroup STM32303C_EVAL_I2C_TSENSOR_Private_Types
  * @{
  */ 
/**
  * @}
  */

/** @defgroup STM32303C_EVAL_I2C_TSENSOR_Private_Defines
  * @{
  */ 
#define TS751_SD_SET                0x01 /*!< Set SD bit in the configuration register */
#define TS751_SD_RESET              0xFE /*!< Reset SD bit in the configuration register */
/**
  * @}
  */ 

/** @defgroup STM32303C_EVAL_I2C_TSENSOR_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 
  
/** @defgroup STM32303C_EVAL_I2C_TSENSOR_Private_Variables
  * @{
  */ 
  
__IO uint32_t  TS751_Timeout = TS751_LONG_TIMEOUT; 
/**
  * @}
  */ 

/** @defgroup STM32303C_EVAL_I2C_TSENSOR_Private_Function_Prototypes
  * @{
  */ 

/**
  * @}
  */ 


/** @defgroup STM32303C_EVAL_I2C_TSENSOR_Private_Functions
  * @{
  */ 

/**
  * @brief  DeInitializes the TS751_I2C.
  * @param  None
  * @retval None
  */
void TS751_DeInit(void)
{
  TS751_LowLevel_DeInit();
}

/**
  * @brief  Initializes the TS751_I2C.
  * @param  None
  * @retval None
  */
void TS751_Init(void)
{  
  I2C_InitTypeDef  I2C_InitStructure;
  
  TS751_LowLevel_Init();
  
  /* TS751_I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_SMBusHost;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_Timing = TS751_I2C_TIMING;
  
  /* Apply TS751_I2C configuration after enabling it */
  I2C_Init(TS751_I2C, &I2C_InitStructure);
  
  /* TS751_I2C Peripheral Enable */
  I2C_Cmd(TS751_I2C, ENABLE);
}

/**
  * @brief  Checks the TS751 status.
  * @param  None
  * @retval ErrorStatus: TS751 Status (ERROR or SUCCESS).
  */
ErrorStatus TS751_GetStatus(void)
{
  uint32_t I2C_TimeOut = I2C_TIMEOUT;
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 0, I2C_AutoEnd_Mode, I2C_No_StartStop);
  
  /* Clear NACKF and STOPF */
  I2C_ClearFlag(TS751_I2C, I2C_ICR_NACKCF | I2C_ICR_STOPCF);
  
  /* Generate start */
  I2C_GenerateSTART(TS751_I2C, ENABLE);  
  
  /* Wait until timeout elapsed */
  while ((I2C_GetFlagStatus(TS751_I2C, I2C_ISR_STOPF) == RESET) && (I2C_TimeOut-- != 0)); 
  
  /* Check if Temp sensor is ready for use */
  if ((I2C_GetFlagStatus(TS751_I2C, I2C_ISR_NACKF) != RESET) || (I2C_TimeOut == 0))
  {      
    /* Clear NACKF and STOPF */
    I2C_ClearFlag(TS751_I2C, I2C_ICR_NACKCF | I2C_ICR_STOPCF);
    
    return ERROR;
  }
  else
  {
    /* Clear STOPF */
    I2C_ClearFlag(TS751_I2C, I2C_ICR_STOPCF);
    
    return SUCCESS;
  }    
}

/**
  * @brief  Read the specified register from the TS751.
  * @param  RegName: specifies the TS751 register to be read.
  *              This member can be one of the following values:  
  *                  - TS751_REG_TEMP: temperature register
  *                  - TS751_REG_TOS: Over-limit temperature register
  *                  - TS751_REG_THYS: Hysteresis temperature register
  * @retval TS751 register value.
  */
uint8_t TS751_ReadReg(uint8_t RegName)
{   
  uint8_t TS751_BufferRX[1] ={0}; 
  
  /* Test on BUSY Flag */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_BUSY) != RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Send Register address */
  I2C_SendData(TS751_I2C, (uint8_t)RegName);
  
  /* Wait until TC flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TC) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }  
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  
  /* Wait until RXNE flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_RXNE) == RESET)    
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Read data from RXDR */
  TS751_BufferRX[0]= I2C_ReceiveData(TS751_I2C);
  
  /* Wait until STOPF flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_STOPF) == RESET)   
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Clear STOPF flag */
  I2C_ClearFlag(TS751_I2C, I2C_ICR_STOPCF);
  
  /* return a Reg value */
  return TS751_BufferRX[0];  
}

/**
  * @brief  Write to the specified register of the TS751.
  * @param  RegName: specifies the TS751 register to be written.
  *              This member can be one of the following values:    
  *                  - TS751_REG_TOS: Over-limit temperature register
  *                  - TS751_REG_THYS: Hysteresis temperature register
  * @param  RegValue: value to be written to TS751 register.  
  * @retval None
  */
uint8_t TS751_WriteReg(uint8_t RegName, uint16_t RegValue)
{  
  uint8_t TS751_BufferTX[1] ={0};
  
  TS751_BufferTX[0] = (uint8_t)(RegValue);
  
  /* Test on BUSY Flag */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_BUSY) != RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;  
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TXIS) == RESET)   
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Send Register address */
  I2C_SendData(TS751_I2C, (uint8_t)RegName);
  
  /* Wait until TCR flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TCR) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);
  
  /* Wait until TXIS flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }  
  
  /* Write data to TXDR */
  I2C_SendData(TS751_I2C, (TS751_BufferTX[0]));
  
  
  /* Wait until STOPF flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_STOPF) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }   
  
  /* Clear STOPF flag */
  I2C_ClearFlag(TS751_I2C, I2C_ICR_STOPCF);
  
  return TS751_OK;
}
/**
  * @brief  Read Temperature register of TS751: double temperature value.
  * @param  None
  * @retval TS751 measured temperature value.
  */
uint8_t TS751_AlerteResponseAddressRead(void)
{   
  uint8_t TS751_BufferRX[1] ={0};
  
  /**************** Read Alerte Response Address ***********************/
  /* Test on BUSY Flag */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_BUSY) != RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  /* Configure slave address, nbytes, reload, end mode and start generation */
  I2C_TransferHandling(TS751_I2C, 0x18, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read); 
  /* Wait until RXNE flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;    
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_RXNE) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  } 
  /* Read data from RXDR */
  TS751_BufferRX[0]= I2C_ReceiveData(TS751_I2C); 
  
  /* Wait until STOPF flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_STOPF) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  /* Clear STOPF flag */
  I2C_ClearFlag(TS751_I2C, I2C_ICR_STOPCF | I2C_ICR_NACKCF);
  
  /* return ARA value */
  return TS751_BufferRX[0]; 
}
  

/**
  * @brief  Read Temperature register of TS751: double temperature value.
  * @param  None
  * @retval TS751 measured temperature value.
  */
uint16_t TS751_ReadTemp(void)
{   
  uint8_t TS751_BufferRX[2] ={0,0};
  uint16_t tmp = 0;
  
  /**************** Read Temperature value high Register ***********************/
  /* Test on BUSY Flag */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_BUSY) != RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write); 
  /* Wait until TXIS flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  } 
  /* Send Register address */
  I2C_SendData(TS751_I2C, (uint8_t)TS751_REG_TEMPH); 
  /* Wait until TC flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TC) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }   
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read); 
  /* Wait until RXNE flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;    
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_RXNE) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  } 
  /* Read data from RXDR */
  TS751_BufferRX[0]= I2C_ReceiveData(TS751_I2C); 
  /* Wait until STOPF flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_STOPF) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  /* Clear STOPF flag */
  I2C_ClearFlag(TS751_I2C, I2C_ICR_STOPCF);
  
  
  /****************** Read Temperature value low Register ************************/  
  /* Test on BUSY Flag */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_BUSY) != RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  } 
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write); 
  /* Wait until TXIS flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  } 
  /* Send Register address */
  I2C_SendData(TS751_I2C, (uint8_t)TS751_REG_TEMPL);
  
  /* Wait until TC flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TC) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }   
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  
  /* Wait until RXNE flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;    
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_RXNE) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  } 
  /* Read data from RXDR */
  TS751_BufferRX[1]= I2C_ReceiveData(TS751_I2C);  
  /* Wait until STOPF flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_STOPF) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  } 
  /* Clear STOPF flag */
  I2C_ClearFlag(TS751_I2C, I2C_ICR_STOPCF);
  
  /*************************** Store Data ******************************/
  /*!< Store TS751_I2C received data */
  tmp = (uint16_t)(TS751_BufferRX[0] << 8);
  tmp |= TS751_BufferRX[1];      
  /*!< Return Temperature value */
  return (uint16_t)(tmp >> 4);
}

/**
  * @brief  Read the configuration register from the TS751.
  * @param  None
  * @retval TS751 configuration register value.
  */
uint8_t TS751_ReadConfReg(void)
{   
  uint8_t TS751_BufferRX[1] ={0}; 
  
  /* Test on BUSY Flag */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_BUSY) != RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Send Register address */
  I2C_SendData(TS751_I2C, (uint8_t)TS751_REG_CONF);
  
  /* Wait until TC flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TC) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }  
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  
  /* Wait until RXNE flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;  
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_RXNE) == RESET)  
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Read data from RXDR */
  TS751_BufferRX[0]= I2C_ReceiveData(TS751_I2C);  
  
  /* Wait until STOPF flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_STOPF) == RESET) 
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Clear STOPF flag */
  I2C_ClearFlag(TS751_I2C, I2C_ICR_STOPCF);
  
  /*!< Return Register value */
  return (uint8_t)TS751_BufferRX[0];
}

/**
  * @brief  Write to the configuration register of the TS751.
  * @param  RegValue: specifies the value to be written to TS751 configuration 
  *         register.
  * @retval None
  */
uint8_t TS751_WriteConfReg(uint8_t RegValue)
{   
  uint8_t TS751_BufferTX = 0;  
  
  TS751_BufferTX = (uint8_t)(RegValue);
  
  /* Test on BUSY Flag */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_BUSY) != RESET) 
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;  
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Send Register address */
  I2C_SendData(TS751_I2C, (uint8_t)TS751_REG_CONF);
  
  /* Wait until TCR flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TCR) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);
  
  /* Wait until TXIS flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }  
  
  /* Write data to TXDR */
  I2C_SendData(TS751_I2C, (uint8_t)TS751_BufferTX);
  
  /* Wait until STOPF flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_STOPF) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }   
  
  /* Clear STOPF flag */
  I2C_ClearFlag(TS751_I2C, I2C_ICR_STOPCF);
    
  return TS751_OK;  
}

/**
  * @brief  Enables or disables the TS751.
  * @param  NewState: specifies the TS751 new status. This parameter can be ENABLE
  *         or DISABLE.  
  * @retval None
  */
uint8_t TS751_ShutDown(FunctionalState NewState)
{   
  uint8_t TS751_BufferRX[2] ={0,0};
  uint8_t TS751_BufferTX = 0;
  __IO uint8_t RegValue = 0;    
  
  /* Test on BUSY Flag */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_BUSY) != RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Send Register address */
  I2C_SendData(TS751_I2C, (uint8_t)TS751_REG_CONF);
  
  /* Wait until TC flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TC) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }  
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  
  /* Wait until RXNE flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;   
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_RXNE) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Read data from RXDR */
  TS751_BufferRX[0]= I2C_ReceiveData(TS751_I2C); 
  
  /* Wait until STOPF flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_STOPF) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Clear STOPF flag */
  I2C_ClearFlag(TS751_I2C, I2C_ICR_STOPCF);
  
  /*!< Get received data */
  RegValue = (uint8_t)TS751_BufferRX[0];
  
  /*---------------------------- Transmission Phase ---------------------------*/
  
  /*!< Enable or disable SD bit */
  if (NewState != DISABLE)
  {
    /*!< Enable TS751 */
    TS751_BufferTX = RegValue & TS751_SD_RESET;
  }
  else
  {
    /*!< Disable TS751 */
    TS751_BufferTX = RegValue | TS751_SD_SET;
  }  
  
  /* Test on BUSY Flag */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_BUSY) != RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_Reload_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;  
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Send Register address */
  I2C_SendData(TS751_I2C, (uint8_t)TS751_REG_CONF);
  
  /* Wait until TCR flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TCR) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(TS751_I2C, TS751_ADDR, 1, I2C_AutoEnd_Mode, I2C_No_StartStop);
  
  /* Wait until TXIS flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }  
  
  /* Write data to TXDR */
  I2C_SendData(TS751_I2C, (uint8_t)TS751_BufferTX);
  
  /* Wait until STOPF flag is set */
  TS751_Timeout = TS751_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(TS751_I2C, I2C_ISR_STOPF) == RESET)
  {
    if((TS751_Timeout--) == 0) return TS751_TIMEOUT_UserCallback();
  }   
  
  /* Clear STOPF flag */
  I2C_ClearFlag(TS751_I2C, I2C_ICR_STOPCF);
  
  return TS751_OK;
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
