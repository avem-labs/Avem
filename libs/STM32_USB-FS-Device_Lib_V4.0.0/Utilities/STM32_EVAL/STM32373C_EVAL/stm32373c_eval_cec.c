/**
  ******************************************************************************
  * @file    stm32373c_eval_cec.c
  * @author  MCD Application Team
  * @version V1.0.0
  * @date    20-September-2012
  * @brief   This file provides all the STM32373C-EVAL HDMI-CEC firmware functions.
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
#include "stm32373c_eval_cec.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */

/** @addtogroup STM32373C_EVAL
  * @{
  */

/** @defgroup STM32373C_EVAL_CEC
  * @brief This file includes the CEC Stack driver for HDMI-CEC Module
  *        of STM32373C-EVAL board
  * @{
  */

/** @defgroup STM32373C_EVAL_CEC_Private_Types
  * @{
  */

/**
  * @}
  */

/** @defgroup STM32373C_EVAL_CEC_Private_Defines
  * @{
  */

/* Uncomment the define to use the LSE as a source clock */
/* #define LSE_SOURCE_CLK */

/**
  * @}
  */

/** @defgroup STM32373C_EVAL_CEC_Private_Macros
  * @{
  */
/**
  * @}
  */

/** @defgroup STM32373C_EVAL_CEC_Private_Variables
  * @{
  */

__IO uint32_t ReceivedFrame = 0;
__IO uint32_t SendFrame = 0;
uint32_t BufferCount = 0, TxCounter = 0, RxCounter = 0;
__IO uint8_t BufferPointer[15];
__IO uint32_t ReceiveStatus = 0;
__IO uint32_t SendStatus = 0;
__IO uint8_t TransErrorCode = 0;
uint8_t MyLogicalAddress = 0;
uint16_t MyPhysicalAddress = 0;
__IO uint8_t DeviceType = 0;
#ifdef HDMI_CEC_USE_DDC
uint8_t pBuffer[256];
__IO uint16_t NumByteToRead = 255;
#endif

__IO uint8_t CECDevicesNumber = 0;
HDMI_CEC_Message HDMI_CEC_TX_MessageStructPrivate;
HDMI_CEC_Message HDMI_CEC_RX_MessageStructPrivate;
HDMI_CEC_Message HDMI_CEC_TX_MessageStructure;

__IO uint8_t FeatureOpcode = 0;
__IO uint8_t AbortReason = 0;
__IO uint8_t DeviceCount = 0;

HDMI_CEC_Map HDMI_CEC_MapStruct;
HDMI_CEC_Map HDMI_CEC_DeviceMap[14];

/* CEC follower addresses */
uint8_t* HDMI_CEC_Follower_String[13][2] =
  {
    {(uint8_t *)"         TV         ", (uint8_t *)"0"},
    {(uint8_t *)" Recording Device 1 ", (uint8_t *)"0"},
    {(uint8_t *)" Recording Device 2 ", (uint8_t *)"0"},
    {(uint8_t *)"      Tuner 1       ", (uint8_t *)"0"},
    {(uint8_t *)"  Playback Device 1 ", (uint8_t *)"0"},
    {(uint8_t *)"    Audio System    ", (uint8_t *)"0"},
    {(uint8_t *)"      Tuner 2       ", (uint8_t *)"0"},
    {(uint8_t *)"      Tuner 3       ", (uint8_t *)"0"},
    {(uint8_t *)"  Playback Device 2 ", (uint8_t *)"0"},
    {(uint8_t *)" Recording Device 3 ", (uint8_t *)"0"},
    {(uint8_t *)"      Tuner 4       ", (uint8_t *)"0"},
    {(uint8_t *)"  Playback Device 3 ", (uint8_t *)"0"},
    {(uint8_t *)"      Broadcast     ", (uint8_t *)"1"}
  };

/* EDID & CEA861 Extension */
const uint8_t EDID_Structure[256] =
{
  /*-------------------------- Block0: 128Bytes ------------------------------*/
  /* Header */
  0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x00,
  /* Vendor & Product Identification */
  0x4C, 0x2D, 0x57, 0x03, 0x00, 0x00, 0x00, 0x00, 0x22, 0x11,
  /* EDID Structure Version & Revision */
  0x01, 0x03, 
  /* Basic Display Parameters & Features */
  0x80, 0x10, 0x09, 0x78, 0x0A, 
  /* Color Characteristics */
  0x1D, 0xED, 0xA7, 0x53, 0x34, 0xAC, 0x25, 0x12, 0x47, 0x4A,
  /* Established Timings */
  0x20, 0x00, 0x00,
  /* Standard Timings */
  0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
  0x01, 0x01, 0x01,
  /* Byte Data Blocks*/
  0x02, 0x3A, 0x80, 0x18, 0x71, 0x38, 0x2D, 0x40, 0x58, 0x2C, 0x45, 0x00, 0xA0, 
  0x5A, 0x00, 0x00, 0x00, 0x1E, 0x01, 0x1D, 0x00, 0x72, 0x51, 0xD0, 0x1E, 0x20, 
  0x6E, 0x28, 0x55, 0x00, 0xA0, 0x5A, 0x00, 0x00, 0x00, 0x1E, 0x00, 0x00, 0x00, 
  0xFD, 0x00, 0x17, 0x3D, 0x1A, 0x44, 0x17, 0x00, 0x0A, 0x20, 0x20, 0x20, 0x20, 
  0x20, 0x20, 0x00, 0x00, 0x00, 0xFC, 0x00, 0x53, 0x41, 0x4D, 0x53, 0x55, 0x4E, 
  0x47, 0x0A, 0x20, 0x20, 0x20, 0x20, 0x20,
  /* Extension Block Count */
  0x01,
  /* Checksum C */
  0xF3,
  /* ------------------------ Block1: 128Bytes------------------------------- */
  /* Refer to CEA 861 Standard for definitions */
  /* CEA 861 EXTENSION Block – Version 3 */
  0x02, 0x03,
  /* Byte number within this block where the 18-byte DTDs begin */
  0x21,
  /* Number of DTDs present */
  0x00,
  /* Start of Data Block Collection */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  /* IEEE Registration Identifier */
  0x03, 0x0C, 0x00,
  /* Vendor Specific Data: Physical Address */
  0x10, 0x00,
  /* Byte Data Blocks */
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00
};

/**
  * @}
  */

/** @defgroup STM32373C_EVAL_CEC_Private_Function_Prototypes
  * @{
  */
static HDMI_CEC_Error SinkPhysicalAddressDiscovery(void);
static HDMI_CEC_Error SourcePhysicalAddressDiscovery(void);
static HDMI_CEC_Error LogicalAddressAllocation(void);

/**
  * @}
  */

/** @defgroup STM32373C_EVAL_CEC_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the HDMI CEC.
  * @param  None
  * @retval HDMI_CEC_Error: CEC Error code
  */
HDMI_CEC_Error HDMI_CEC_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  CEC_InitTypeDef CEC_InitStructure;
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;
  uint8_t sendcount = 0;
  uint32_t  idx = 0;
  
#ifdef LSE_SOURCE_CLK
  /* LSE */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  PWR_BackupAccessCmd(ENABLE);
  
  RCC_LSEConfig(RCC_LSE_ON);
  while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}
  
  RCC_CECCLKConfig(RCC_CECCLK_LSE);
#endif
  
  for(idx=0; idx<12; idx++)
  {
    HDMI_CEC_Follower_String[idx][1] = (uint8_t *)"0";
  }
  
  /* Enable CEC and PWR clocks */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_CEC | RCC_APB1Periph_PWR, ENABLE);
  
  /* Enable CEC_LINE_GPIO clocks */
  RCC_AHBPeriphClockCmd(HDMI_CEC_LINE_GPIO_CLK, ENABLE);
  
  /* Configure CEC_LINE_GPIO as Output open drain */
  GPIO_InitStructure.GPIO_Pin = HDMI_CEC_LINE_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ;
  GPIO_Init(HDMI_CEC_LINE_GPIO_PORT, &GPIO_InitStructure);
  
  GPIO_PinAFConfig(HDMI_CEC_LINE_GPIO_PORT,HDMI_CEC_LINE_PIN_SOURCE,HDMI_CEC_GPIO_AF);
  
  /* CEC DeInit */
  CEC_DeInit();
  
  /* Configure CEC */
  CEC_InitStructure.CEC_SignalFreeTime = CEC_SignalFreeTime_Standard/*CEC_SignalFreeTime_1T*/;
  CEC_InitStructure.CEC_RxTolerance = CEC_RxTolerance_Standard;
  CEC_InitStructure.CEC_StopReception = CEC_StopReception_Off;
  CEC_InitStructure.CEC_BitRisingError = CEC_BitRisingError_Off;
  CEC_InitStructure.CEC_LongBitPeriodError = CEC_LongBitPeriodError_Off;
  CEC_InitStructure.CEC_BRDNoGen = CEC_BRDNoGen_Off;
  CEC_InitStructure.CEC_SFTOption = CEC_SFTOption_Off;
  CEC_Init(&CEC_InitStructure);
  
  /* Disable Listen Mode */
  CEC_ListenModeCmd(DISABLE);
  
  /* Enable CEC */
  CEC_Cmd(ENABLE);
  
  if (HDMI_CEC_ROOT == 0x1)
  {
    /* Logical Address Allocation */
    sendcount = 0;
    errorstatus = LogicalAddressAllocation();
    
    while ((errorstatus != HDMI_CEC_OK) && sendcount < 0x5)
    {
      sendcount++;
      errorstatus = LogicalAddressAllocation();
    }
    
    if (errorstatus != HDMI_CEC_OK)
    {
      /* Device Unregistered */
      return(errorstatus);
    }
    
    HDMI_CEC_CheckConnectedDevices();
    
    /* Set the CEC initiator address */
    CEC_OwnAddressConfig(MyLogicalAddress);
    
    /* Sink Physical Address discovery */
    errorstatus = SinkPhysicalAddressDiscovery();
    
    if (errorstatus != HDMI_CEC_OK)
    {
      /* Device not connected (Physical Address lost) */
      return(errorstatus);
    }
  }
  else
  {
    /* Source Physical Address discovery */
    errorstatus = SourcePhysicalAddressDiscovery();
    
    if (errorstatus != HDMI_CEC_OK)
    {
      /* Device not connected (Physical Address lost) */
      return(errorstatus);
    }
    
    /* Logical Address Allocation */
    sendcount = 0;
    errorstatus = LogicalAddressAllocation();
    
    while ((errorstatus != HDMI_CEC_OK) && sendcount < 0x5)
    {
      sendcount++;
      errorstatus = LogicalAddressAllocation();
    }
    
    if (errorstatus != HDMI_CEC_OK)
    {
      /* Device Unregistered */
      return(errorstatus);
    }
    
    HDMI_CEC_CheckConnectedDevices();
    
    /* Set the CEC initiator address */
    CEC_OwnAddressConfig(MyLogicalAddress);
  }
  
  /* Activate CEC interrupts associated to the set of TX flags */
  CEC_ITConfig(CEC_IT_TXEND|CEC_IT_TXBR,ENABLE);
  
  /* Activate CEC interrupts associated to the set of RX flags */
  CEC_ITConfig(CEC_IT_RXEND|CEC_IT_RXBR,ENABLE);
  
  /* Activate CEC interrupts associated to the set of TX error */
  CEC_ITConfig(CEC_IT_TXACKE|CEC_IT_TXERR|CEC_IT_TXUDR|CEC_IT_ARBLST,ENABLE);
  
  /* Activate CEC interrupts associated to the set of RX error */
  CEC_ITConfig(CEC_IT_RXACKE|CEC_IT_LBPE|CEC_IT_SBPE|CEC_IT_BRE|CEC_IT_RXOVR,ENABLE);
  
  /* Report physical address */
  errorstatus = HDMI_CEC_ReportPhysicalAddress();
  sendcount = 0;
  RxCounter = 0;
  
  while ((errorstatus != HDMI_CEC_OK) && sendcount < 0x5)
  {
    sendcount++;
    errorstatus = HDMI_CEC_ReportPhysicalAddress();
  }
  
  if (errorstatus != HDMI_CEC_OK)
  {
    /* Device Unregistred */
    return(errorstatus);
  }
  return errorstatus;
}

/**
  * @brief  Transmit message by taking data from typedef struct CEC_Meassage
  * @param  CEC_TX_MessageStructure: pointer to an CEC_Message structure that contains
  *         the message to be sent.
  * @retval HDMI_CEC_Error: CEC Error code
  */
HDMI_CEC_Error HDMI_CEC_TransmitMessage(HDMI_CEC_Message *HDMI_CEC_TX_MessageStructure)
{
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;
  __IO uint32_t count = 0, j = 0;

  SendFrame = 0;
  SendStatus = 0;
  TxCounter = 0;
  BufferCount = 0;

  HDMI_CEC_TX_MessageStructPrivate = *HDMI_CEC_TX_MessageStructure;

  /* Initialize BufferPointer */
  for (j = 0; j < 15; j++)
  {
    BufferPointer[j] = 0;
  }

  BufferPointer[0] =  HDMI_CEC_TX_MessageStructPrivate.Opcode;

  for (BufferCount = 1; BufferCount < HDMI_CEC_TX_MessageStructPrivate.TxMessageLength + 1; BufferCount++)
  {
    BufferPointer[BufferCount] = HDMI_CEC_TX_MessageStructPrivate.Operande[BufferCount - 1];
  }

  CEC_ClearFlag(CEC_FLAG_TXBR|CEC_FLAG_TXEND);

  /* Write single Data in the TX Buffer to Transmit through the CEC peripheral */
  CEC_SendData(HDMI_CEC_TX_MessageStructPrivate.Header);

  /* Initiate Message Transmission */
  CEC_StartOfMessage();

  while ((SendFrame == 0) && (count < HDMI_CEC_TIMEOUT_VALUE))
  {
    count++;
  }
  if (count >= HDMI_CEC_TIMEOUT_VALUE)
  {
    errorstatus = HDMI_CEC_TIMEOUT;
    return(errorstatus);
  }

  if (SendStatus == 0)
  {
    errorstatus = (HDMI_CEC_Error) TransErrorCode;
  }
  return errorstatus;
}

/**
  * @brief  Get the ISR register status.
  * @param  None
  * @retval HDMI_CEC_Error: CEC Error code
  */
HDMI_CEC_Error HDMI_CEC_GetErrorStatus (void)
{
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;

  /* RX Overrun error case*/
  if (CEC_GetFlagStatus(CEC_FLAG_RXOVR) != RESET)
  {
    errorstatus = HDMI_CEC_RX_OVERRUN;
    CEC_ClearFlag(CEC_FLAG_RXOVR);
  }
  /* Bit Rising error case */
  if (CEC_GetFlagStatus(CEC_FLAG_BRE) != RESET)
  {
    errorstatus = HDMI_CEC_RX_BIT_RISING;
    CEC_ClearFlag(CEC_FLAG_BRE);
  }
  /* Short Bit Period error case */
  if (CEC_GetFlagStatus(CEC_FLAG_SBPE) != RESET)
  {
    errorstatus = HDMI_CEC_RX_SHORT_BIT_PERIOD;
    CEC_ClearFlag(CEC_FLAG_SBPE);
  }
  /* Long Bit Period error case */
  if (CEC_GetFlagStatus(CEC_FLAG_LBPE) != RESET)
  {
    errorstatus = HDMI_CEC_RX_LONG_BIT_PERIOD;
    CEC_ClearFlag(CEC_FLAG_LBPE);
  }
  /* RX Acknowledge error case */
  if (CEC_GetFlagStatus(CEC_FLAG_RXACKE) != RESET)
  {
    errorstatus = HDMI_CEC_RX_ACKNOWLEDGE;
    CEC_ClearFlag(CEC_FLAG_RXACKE);
  }
  /* Arbitration Lost error case */
  if (CEC_GetFlagStatus(CEC_FLAG_ARBLST) != RESET)
  {
    errorstatus = HDMI_CEC_ARBITRATION_LOST;
    CEC_ClearFlag(CEC_FLAG_ARBLST);
  }
  /* TX underrun error case */
  if (CEC_GetFlagStatus(CEC_FLAG_TXUDR) != RESET)
  {
    errorstatus = HDMI_CEC_TX_UNDERRUN;
    CEC_ClearFlag(CEC_FLAG_TXUDR);
  }
  /* TX error case */
  if (CEC_GetFlagStatus(CEC_FLAG_TXERR) != RESET)
  {
    errorstatus = HDMI_CEC_TX_ERROR;
    CEC_ClearFlag(CEC_FLAG_TXERR);
  }
  /* TX Acknowledge error case */
  if (CEC_GetFlagStatus(CEC_FLAG_TXACKE) != RESET)
  {
    errorstatus = HDMI_CEC_TX_ACKNOWLEDGE;
    CEC_ClearFlag(CEC_FLAG_TXACKE);
  } 
  return errorstatus;
}

/**
  * @brief  Allows to process all the interrupts that are high.
  * @param  None
  * @retval None
  */
void HDMI_CEC_ProcessIRQSrc(void)
{
  /********************** Reception *******************************************/
  /* Check if a reception error occured */
  if (CEC->ISR & (CEC_IT_RXACKE | CEC_IT_LBPE | CEC_IT_SBPE | CEC_IT_BRE | CEC_IT_RXOVR))
  {
    /* Set receive status bit (Error) */
    ReceiveStatus  = 0;
    ReceivedFrame  = 1;
    CEC->ISR = (CEC_IT_RXACKE | CEC_IT_LBPE | CEC_IT_SBPE | CEC_IT_BRE | CEC_IT_RXOVR);
  }

  if (CEC_GetITStatus(CEC_IT_RXBR))
  {
    /* Check if the byte received is a Header */
    if (RxCounter == 0)
    {
      HDMI_CEC_RX_MessageStructPrivate.Header = CEC_ReceiveData();
      RxCounter ++;
    }
    else if(RxCounter == 1)   /* Receive opcode in the reception buffer */
    {
      HDMI_CEC_RX_MessageStructPrivate.Opcode = CEC_ReceiveData();
      RxCounter++;
    }
    else /* Receive each byte except header and opcode in the reception buffer */
    {
      HDMI_CEC_RX_MessageStructPrivate.Operande[RxCounter-2] = CEC_ReceiveData();
      RxCounter++;
    }  
    CEC_ClearITPendingBit(CEC_IT_RXBR);
  }

  /* Check if the byte received is the last one of the message */
  if (CEC_GetITStatus(CEC_IT_RXEND))
  {
    if (RxCounter == 1)
    {

      HDMI_CEC_RX_MessageStructPrivate.RxMessageLength = RxCounter-1;
    }
    else
    {
      HDMI_CEC_RX_MessageStructPrivate.RxMessageLength = RxCounter-2;
      ReceivedFrame = 1;
    }

    /* Set the receive status */
    ReceiveStatus = SUCCESS;
    RxCounter = 0;
    
    /* Clear all reception flags */
    CEC_ClearITPendingBit(CEC_IT_RXEND);
  }

  /********************** Transmission ****************************************/
  /* Check if a transmission error occurred */
  if (CEC->ISR & (CEC_IT_TXACKE | CEC_IT_TXERR | CEC_IT_TXUDR | CEC_IT_ARBLST))
  {
    TransErrorCode = HDMI_CEC_GetErrorStatus();
    CEC->ISR = (CEC_IT_TXACKE | CEC_IT_TXERR | CEC_IT_TXUDR | CEC_IT_ARBLST);
    SendFrame = 1;
    SendStatus = 0;
  }

  /* Check if end of message bit is set in the data to be transmitted */
  if (CEC_GetITStatus(CEC_IT_TXEND))
  {
    CEC_ClearITPendingBit(CEC_IT_TXEND | CEC_IT_TXBR);
    SendFrame = 1;
    SendStatus = SUCCESS;
  }
  /* Check if data byte has been sent */
  else if (CEC_GetITStatus(CEC_IT_TXBR))
  {
    /* Set EOM bit if the byte to be transmitted is the last one of the Transmit Buffer */
    if (TxCounter == (HDMI_CEC_TX_MessageStructPrivate.TxMessageLength))
    {
      CEC_EndOfMessage();
      CEC_SendData(BufferPointer[TxCounter++]);
    }
    else
    {
      /* Put the byte in the TX Buffer */
      CEC_SendData(BufferPointer[TxCounter++]);
    }
    CEC_ClearITPendingBit(CEC_IT_TXBR);
  }
}

/**
  * @brief  Report physical address to all other devices thus allowing any
            device to create a map of the network.
  * @param  None
  * @retval HDMI_CEC_Error: CEC Error code.
  */
HDMI_CEC_Error HDMI_CEC_ReportPhysicalAddress(void)
{
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;
  HDMI_CEC_Message HDMI_CEC_TX_Message;

  HDMI_CEC_TX_Message.Header = ((MyLogicalAddress << 4) | 0xF);
  HDMI_CEC_TX_Message.Opcode = HDMI_CEC_OPCODE_REPORT_PHYSICAL_ADDRESS;
  HDMI_CEC_TX_Message.Operande[0] = MyPhysicalAddress >> 8;
  HDMI_CEC_TX_Message.Operande[1] = MyPhysicalAddress & 0xFF;
  HDMI_CEC_TX_Message.Operande[2] = DeviceType;
  HDMI_CEC_TX_Message.TxMessageLength = 0x03;

  errorstatus = HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_Message);

  return  errorstatus;
}

/**
  * @brief  Handle CEC command receive callback.
  *         When receiving the STANDBY Opcode command, the system is entered in
  *         Stop mode and when wakeup, the PLL is configured as system clock and
  *         the HSI is selected as PLL source.      
  * @param  None
  * @retval None
  */
void HDMI_CEC_CommandCallBack(void)
{
  uint8_t i = 0, sendcount = 0;
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;
  ErrorStatus HSEStartUpStatus;

  switch (HDMI_CEC_RX_MessageStructPrivate.Opcode)
  {
    case HDMI_CEC_OPCODE_REPORT_PHYSICAL_ADDRESS:
      HDMI_CEC_MapStruct.PhysicalAddress_A = HDMI_CEC_RX_MessageStructPrivate.Operande[1] >> 4;
      HDMI_CEC_MapStruct.PhysicalAddress_B = HDMI_CEC_RX_MessageStructPrivate.Operande[1] & 0x0F;
      HDMI_CEC_MapStruct.PhysicalAddress_C = HDMI_CEC_RX_MessageStructPrivate.Operande[0] >> 4;
      HDMI_CEC_MapStruct.PhysicalAddress_D = HDMI_CEC_RX_MessageStructPrivate.Operande[0] & 0x0F;
      HDMI_CEC_MapStruct.LogicalAddress = (HDMI_CEC_RX_MessageStructPrivate.Header >> 0x4) & 0x0F;
      HDMI_CEC_MapStruct.DeviceType = HDMI_CEC_RX_MessageStructPrivate.Operande[2];
      HDMI_CEC_DeviceMap[DeviceCount] = HDMI_CEC_MapStruct;
      HDMI_CEC_Follower_String[(HDMI_CEC_DeviceMap[DeviceCount].LogicalAddress)][1] = (uint8_t *)"1";
      DeviceCount++;
      break;

    case HDMI_CEC_OPCODE_STANDBY:

      /* Request to enter Stop mode */
      PWR_EnterSTOPMode(PWR_Regulator_ON, PWR_STOPEntry_WFI);

      /* Enable HSE */
      RCC_HSEConfig(RCC_HSE_ON);

      /* Wait till HSE is ready */
      HSEStartUpStatus = RCC_WaitForHSEStartUp();
      if (HSEStartUpStatus == SUCCESS)
      {
        /* Enable PLL */
        RCC_PLLCmd(ENABLE);
        /* Wait till PLL is ready */
        while (RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {}
        /* Select PLL as system clock source */
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);
        /* Wait till PLL is used as system clock source */
        while (RCC_GetSYSCLKSource() != 0x08)
        {}
      }
      break;

    case HDMI_CEC_OPCODE_GET_CEC_VERSION:
      /* Send the Used CEC version */
      HDMI_CEC_TX_MessageStructPrivate.Header = ((MyLogicalAddress << 4) | HDMI_CEC_RX_MessageStructPrivate.Header >> 4);
      HDMI_CEC_TX_MessageStructPrivate.Opcode = HDMI_CEC_OPCODE_CEC_VERSION;
      HDMI_CEC_TX_MessageStructPrivate.Operande[0] = HDMI_CEC_VERSION; /* CEC Version */
      HDMI_CEC_TX_MessageStructPrivate.TxMessageLength = 0x01;
      errorstatus = HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructPrivate);

      /* Retransmit message until 5 time */
      while ((errorstatus != HDMI_CEC_OK) && sendcount < 0x5)
      {
        sendcount++;
        errorstatus = HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructPrivate);
      }
      break;

    case HDMI_CEC_OPCODE_GIVE_PHYSICAL_ADDRESS:
      /* Send the Physical address */
      errorstatus = HDMI_CEC_ReportPhysicalAddress();
      sendcount = 0;
      /* Retransmit message until 5 time */
      while ((errorstatus != HDMI_CEC_OK) && sendcount < 0x5)
      {
        sendcount++;
        errorstatus = HDMI_CEC_ReportPhysicalAddress();
      }
      break;

    case HDMI_CEC_OPCODE_FEATURE_ABORT:
      /* The device doesn't support the requested message type, or that it cannot
      execute it at the present time. */
      FeatureOpcode = HDMI_CEC_RX_MessageStructPrivate.Operande[0];
      AbortReason = HDMI_CEC_RX_MessageStructPrivate.Operande[1];
      break;

    case HDMI_CEC_OPCODE_GIVE_OSD_NAME:
      /* Send the OSD name = STM32373C CEC */
      HDMI_CEC_TX_MessageStructPrivate.Header = ((MyLogicalAddress << 4) | HDMI_CEC_RX_MessageStructPrivate.Header >> 4);
      HDMI_CEC_TX_MessageStructPrivate.Opcode = HDMI_CEC_OPCODE_SET_OSD_NAME;
      /* STM32373C */
      HDMI_CEC_TX_MessageStructPrivate.Operande[0] = 0x53;
      HDMI_CEC_TX_MessageStructPrivate.Operande[1] = 0x54;
      HDMI_CEC_TX_MessageStructPrivate.Operande[2] = 0x4D;
      HDMI_CEC_TX_MessageStructPrivate.Operande[3] = 0x33;
      HDMI_CEC_TX_MessageStructPrivate.Operande[4] = 0x32;
      HDMI_CEC_TX_MessageStructPrivate.Operande[5] = 0x33;
      HDMI_CEC_TX_MessageStructPrivate.Operande[6] = 0x37;
      HDMI_CEC_TX_MessageStructPrivate.Operande[7] = 0x33;
      HDMI_CEC_TX_MessageStructPrivate.Operande[8] = 0x43;
      HDMI_CEC_TX_MessageStructPrivate.Operande[9] = 0x20;
      /* CEC */
      HDMI_CEC_TX_MessageStructPrivate.Operande[10] = 0x43;
      HDMI_CEC_TX_MessageStructPrivate.Operande[11] = 0x45;
      HDMI_CEC_TX_MessageStructPrivate.Operande[12] = 0x43;
      HDMI_CEC_TX_MessageStructPrivate.TxMessageLength = 13;
      errorstatus = HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructPrivate);
      sendcount = 0;
      /* Retransmit message until 5 time */
      while ((errorstatus != HDMI_CEC_OK) && sendcount < 0x5)
      {
        sendcount++;
        errorstatus = HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructPrivate);
      }
      break;

    case HDMI_CEC_OPCODE_ROUTING_CHANGE:
      for (i = 0;i < 0x14;i++)
      {
        if ((HDMI_CEC_DeviceMap[i].PhysicalAddress_A == (HDMI_CEC_RX_MessageStructPrivate.Operande[1] >> 4)) &&
            (HDMI_CEC_DeviceMap[i].PhysicalAddress_B == (HDMI_CEC_RX_MessageStructPrivate.Operande[1]&0x0F)) &&
            (HDMI_CEC_DeviceMap[i].PhysicalAddress_C == (HDMI_CEC_RX_MessageStructPrivate.Operande[0] >> 4)) &&
            (HDMI_CEC_DeviceMap[i].PhysicalAddress_D == (HDMI_CEC_RX_MessageStructPrivate.Operande[0]&0x0F)))
        {
          HDMI_CEC_MapStruct.LogicalAddress = (HDMI_CEC_RX_MessageStructPrivate.Header >> 0x4) & 0x0F;
          HDMI_CEC_MapStruct.DeviceType = HDMI_CEC_RX_MessageStructPrivate.Operande[2];
          HDMI_CEC_DeviceMap[i] = HDMI_CEC_MapStruct;
        }
      }
      break;

    default:
      /* Send Abort feature */
      HDMI_CEC_TX_MessageStructPrivate.Header = ((MyLogicalAddress << 4) | HDMI_CEC_RX_MessageStructPrivate.Header >> 4);
      HDMI_CEC_TX_MessageStructPrivate.Opcode = HDMI_CEC_OPCODE_FEATURE_ABORT;
      HDMI_CEC_TX_MessageStructPrivate.Operande[0] = 0x02; /* Defines command to be performed */
      HDMI_CEC_TX_MessageStructPrivate.Operande[1] = HDMI_CEC_REFUSED; /* Reason for abort feature */
      HDMI_CEC_TX_MessageStructPrivate.TxMessageLength = 0x02;
      errorstatus = HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructPrivate);
      sendcount = 0;
      /* Retransmit message until 5 time */
      while ((errorstatus != HDMI_CEC_OK) && sendcount < 0x5)
      {
        sendcount++;
        errorstatus = HDMI_CEC_TransmitMessage(&HDMI_CEC_TX_MessageStructPrivate);
      }
      break;
  }
}

/**
  * @brief  Check the connected CEC devices.
  * @param  None
  * @retval HDMI_CEC_Error
  */
HDMI_CEC_Error HDMI_CEC_CheckConnectedDevices(void)
{
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;
  uint32_t count = 0, i = 1;

  /*------------------------------- TV device --------------------------------*/
  CEC_EndOfMessage();

  CEC_SendData((MyLogicalAddress << 4) | 0x0);

  /* Start of message */
  CEC_StartOfMessage();

  /* Wait till the header message is sent */
  while (count < HDMI_CEC_TIMEOUT_VALUE)
  {
    if (CEC_GetFlagStatus(CEC_FLAG_TXACKE) != RESET)
    { 
      errorstatus = HDMI_CEC_GetErrorStatus();
      CEC_ClearFlag( CEC_FLAG_TXACKE);
      break;
    }
    else if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
    {
      errorstatus = HDMI_CEC_OK;
      break;
    }
    count++;
  }

  if (count >= HDMI_CEC_TIMEOUT_VALUE)
  {
    errorstatus = HDMI_CEC_TIMEOUT;
    return(errorstatus);
  }

  if (errorstatus == HDMI_CEC_OK)
  {
    HDMI_CEC_Follower_String[0][1] = (uint8_t *)"1";
    i++;
    errorstatus = HDMI_CEC_OK;
  }

  /* Clear CEC ISR register */
  CEC_ClearFlag(CEC_FLAG_TXBR|CEC_FLAG_TXEND);

  /*---------------------------- Recording device 1 --------------------------*/
  CEC_EndOfMessage();

  CEC_SendData((MyLogicalAddress << 4)|0x1);

  /* Start of message */
  CEC_StartOfMessage();

  /* Wait till the header message is sent */
  while (count < HDMI_CEC_TIMEOUT_VALUE)
  {
    if (CEC_GetFlagStatus(CEC_FLAG_TXACKE) != RESET)
    { 
      errorstatus = HDMI_CEC_GetErrorStatus();
      CEC_ClearFlag( CEC_FLAG_TXACKE);
      break;
    }
    else if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
    {
      errorstatus = HDMI_CEC_OK;
      break;
    }
    count++;
  }
  if (count >= HDMI_CEC_TIMEOUT_VALUE)
  {
    errorstatus = HDMI_CEC_TIMEOUT;
    return(errorstatus);
  }

  if (errorstatus == HDMI_CEC_OK)
  {
    HDMI_CEC_Follower_String[1][1] = (uint8_t *)"1";
    i++;
    errorstatus = HDMI_CEC_OK;
  }

  /* Clear CEC ISR register */
  CEC_ClearFlag(CEC_FLAG_TXBR|CEC_FLAG_TXEND);

  /*---------------------------- Recording device 2 --------------------------*/
  CEC_EndOfMessage();

  CEC_SendData((MyLogicalAddress << 4) | 0x2);

  /* Start of message */
  CEC_StartOfMessage();

  /* Wait till the header message is sent */
  while (count < HDMI_CEC_TIMEOUT_VALUE)
  {  
    if (CEC_GetFlagStatus(CEC_FLAG_TXACKE) != RESET)
    {
      errorstatus = HDMI_CEC_GetErrorStatus();
      CEC_ClearFlag( CEC_FLAG_TXACKE);
      break;
    }
    else if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
    {
      errorstatus = HDMI_CEC_OK;
      break;
    }
    count++;
  }

  if (count >= HDMI_CEC_TIMEOUT_VALUE)
  {
    errorstatus = HDMI_CEC_TIMEOUT;
    return(errorstatus);
  }

  if (errorstatus == HDMI_CEC_OK)
  {
    HDMI_CEC_Follower_String[2][1] = (uint8_t *)"1";
    i++;
    errorstatus = HDMI_CEC_OK;
  }

  /* Clear CEC ISR register */
  CEC_ClearFlag(CEC_FLAG_TXBR|CEC_FLAG_TXEND);

  /*------------------------------ Tuner 1 -----------------------------------*/
  CEC_EndOfMessage();

  CEC_SendData((MyLogicalAddress << 4) | 0x3);

  /* Start of message */
  CEC_StartOfMessage();

  /* Wait till the header message is sent */
  while (count < HDMI_CEC_TIMEOUT_VALUE)
  {
    if (CEC_GetFlagStatus(CEC_FLAG_TXACKE) != RESET)
    {
      errorstatus = HDMI_CEC_GetErrorStatus();
      CEC_ClearFlag( CEC_FLAG_TXACKE);
      break;
    }
    else if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
    {
      errorstatus = HDMI_CEC_OK;
      break;
    }
    count++;
  }
  if (count >= HDMI_CEC_TIMEOUT_VALUE)
  {
    errorstatus = HDMI_CEC_TIMEOUT;
    return(errorstatus);
  }

  if (errorstatus == HDMI_CEC_OK)
  {
    HDMI_CEC_Follower_String[3][1] = (uint8_t *)"1";
    i++;
    errorstatus = HDMI_CEC_OK;
  }
  /* Clear CEC ISR register */
  CEC_ClearFlag(CEC_FLAG_TXBR|CEC_FLAG_TXEND);

  /*---------------------------- Playback device 1 ---------------------------*/
  CEC_EndOfMessage();

  CEC_SendData((MyLogicalAddress << 4) | 0x4);

  /* Start of message */
  CEC_StartOfMessage();

  /* Wait till the header message is sent */
  while (count < HDMI_CEC_TIMEOUT_VALUE)
  {
    if (CEC_GetFlagStatus(CEC_FLAG_TXACKE) != RESET)
    {
      errorstatus = HDMI_CEC_GetErrorStatus();
      CEC_ClearFlag(CEC_FLAG_TXACKE);
      break;
    }
    else if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
    {
      errorstatus = HDMI_CEC_OK;
      break;
    }
    count++;
  }

  if (count >= HDMI_CEC_TIMEOUT_VALUE)
  {
    errorstatus = HDMI_CEC_TIMEOUT;
    return(errorstatus);
  }

  if (errorstatus == HDMI_CEC_OK)
  {
    HDMI_CEC_Follower_String[4][1] = (uint8_t *)"1";
    i++;
    errorstatus = HDMI_CEC_OK;
  }
  /* Clear CEC ISR register */
  CEC_ClearFlag(CEC_FLAG_TXBR|CEC_FLAG_TXEND);

  /*-------------------------------- Audio system ----------------------------*/
  CEC_EndOfMessage();

  CEC_SendData((MyLogicalAddress << 4) | 0x5); 
  
  /* Start of message */
  CEC_StartOfMessage();

  /* Wait till the header message is sent */
  while (count < HDMI_CEC_TIMEOUT_VALUE)
  {
    if (CEC_GetFlagStatus(CEC_FLAG_TXACKE) != RESET)
    {
      errorstatus = HDMI_CEC_GetErrorStatus();
      CEC_ClearFlag(CEC_FLAG_TXACKE);
      break;
    }
    else if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
    {
      errorstatus = HDMI_CEC_OK;
      break;
    }
    count++;
  }

  if (count >= HDMI_CEC_TIMEOUT_VALUE)
  {
    errorstatus = HDMI_CEC_TIMEOUT;
    return(errorstatus);
  }

  if (errorstatus == HDMI_CEC_OK)
  {
    HDMI_CEC_Follower_String[5][1] = (uint8_t *)"1";
    i++;
    errorstatus = HDMI_CEC_OK;
  }

  /* Clear CEC ISR register */
  CEC_ClearFlag(CEC_FLAG_TXBR|CEC_FLAG_TXEND);

  /*--------------------------------- Tuner 2 --------------------------------*/
  CEC_EndOfMessage();

  CEC_SendData((MyLogicalAddress << 4) | 0x6);

  /* Start of message */
  CEC_StartOfMessage();

  /* Wait till the header message is sent */
  while (count < HDMI_CEC_TIMEOUT_VALUE)
  {
    if (CEC_GetFlagStatus(CEC_FLAG_TXACKE) != RESET)
    {
      errorstatus = HDMI_CEC_GetErrorStatus();
      CEC_ClearFlag(CEC_FLAG_TXACKE);
      break;
    }
    else if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
    {
      errorstatus = HDMI_CEC_OK;
      break;
    }
    count++;
  }

  if (count >= HDMI_CEC_TIMEOUT_VALUE)
  {
    errorstatus = HDMI_CEC_TIMEOUT;
    return(errorstatus);
  }

  if (errorstatus == HDMI_CEC_OK)
  {
    HDMI_CEC_Follower_String[6][1] = (uint8_t *)"1";
    i++;
    errorstatus = HDMI_CEC_OK;
  }
  /* Clear CEC ISR register */
  CEC_ClearFlag(CEC_FLAG_TXBR|CEC_FLAG_TXEND);

  /*--------------------------------- Tuner 3 --------------------------------*/
  CEC_EndOfMessage();

  CEC_SendData((MyLogicalAddress << 4) | 0x7);

  /* Start of message */
  CEC_StartOfMessage();

  /* Wait till the header message is sent */
  while (count < HDMI_CEC_TIMEOUT_VALUE)
  {
    if (CEC_GetFlagStatus(CEC_FLAG_TXACKE) != RESET)
    { 
      errorstatus = HDMI_CEC_GetErrorStatus();
      CEC_ClearFlag(CEC_FLAG_TXACKE);
      break;
    }
    else if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
    {
      errorstatus = HDMI_CEC_OK;
      break;
    }
    count++;
  }
  if (count >= HDMI_CEC_TIMEOUT_VALUE)
  {
    errorstatus = HDMI_CEC_TIMEOUT;
    return(errorstatus);
  }

  if (errorstatus == HDMI_CEC_OK)
  {
    HDMI_CEC_Follower_String[7][1] = (uint8_t *)"1";
    i++;
    errorstatus = HDMI_CEC_OK;
  }
  
  /* Clear CEC ISR register */
  CEC_ClearFlag(CEC_FLAG_TXBR|CEC_FLAG_TXEND);

  /*---------------------------- Playback device 2 ---------------------------*/  
  CEC_EndOfMessage();

  CEC_SendData((MyLogicalAddress << 4) | 0x8);

  /* Start of message */
  CEC_StartOfMessage();

  /* Wait till the header message is sent */
  while(count < HDMI_CEC_TIMEOUT_VALUE)
  {
    if (CEC_GetFlagStatus(CEC_FLAG_TXACKE) != RESET)
    {
      errorstatus = HDMI_CEC_GetErrorStatus();
      CEC_ClearFlag(CEC_FLAG_TXACKE);
      break;
    }
    else if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
    {
      errorstatus = HDMI_CEC_OK;
      break;
    }
    count++;
  }

  if (count >= HDMI_CEC_TIMEOUT_VALUE)
  {
    errorstatus = HDMI_CEC_TIMEOUT;
    return(errorstatus);
  }

  if (errorstatus == HDMI_CEC_OK)
  {
    HDMI_CEC_Follower_String[8][1] = (uint8_t *)"1";
    i++;
    errorstatus = HDMI_CEC_OK;
  }
  /* Clear CEC ISR register */
  CEC_ClearFlag(CEC_FLAG_TXBR|CEC_FLAG_TXEND);

  /*---------------------------- Recording device 3 --------------------------*/
  CEC_EndOfMessage();

  CEC_SendData((MyLogicalAddress << 4) | 0x9);

  /* Start of message */
  CEC_StartOfMessage();

  /* Wait till the header message is sent */
  while(count < HDMI_CEC_TIMEOUT_VALUE)
  {
    if (CEC_GetFlagStatus(CEC_FLAG_TXACKE) != RESET)
    {
      errorstatus = HDMI_CEC_GetErrorStatus();
      CEC_ClearFlag(CEC_FLAG_TXACKE);
      break;
    }
    else if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
    {
      errorstatus = HDMI_CEC_OK;
      break;
    }
    count++;
  }

  if (count >= HDMI_CEC_TIMEOUT_VALUE)
  {
    errorstatus = HDMI_CEC_TIMEOUT;
    return(errorstatus);
  }

  if (errorstatus == HDMI_CEC_OK)
  {
    HDMI_CEC_Follower_String[9][1] = (uint8_t *)"1";
    i++;
    errorstatus = HDMI_CEC_OK;
  }
  /* Clear CEC ISR register */
  CEC_ClearFlag(CEC_FLAG_TXBR|CEC_FLAG_TXEND);

  /*-------------------------------- Tuner 4 ---------------------------------*/
  CEC_EndOfMessage();

  CEC_SendData((MyLogicalAddress << 4) | 0xA);

  /* Start of message */
  CEC_StartOfMessage();

  /* Wait till the header message is sent */
  while (count < HDMI_CEC_TIMEOUT_VALUE)
  {
    if (CEC_GetFlagStatus(CEC_FLAG_TXACKE) != RESET)
    { 
      errorstatus = HDMI_CEC_GetErrorStatus();
      CEC_ClearFlag(CEC_FLAG_TXACKE);
      break;
    }
    else if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
    {
      errorstatus = HDMI_CEC_OK;
      break;
    }
    count++;
  }

  if (count >= HDMI_CEC_TIMEOUT_VALUE)
  {
    errorstatus = HDMI_CEC_TIMEOUT;
    return(errorstatus);
  }

  if (errorstatus == HDMI_CEC_OK)
  {
    HDMI_CEC_Follower_String[10][1] = (uint8_t *)"1";
    i++;
    errorstatus = HDMI_CEC_OK;
  }
  /* Clear CEC ISR register */
  CEC_ClearFlag(CEC_FLAG_TXBR|CEC_FLAG_TXEND);

  /*---------------------------- Playback device 3 ---------------------------*/
  CEC_EndOfMessage();

  CEC_SendData((MyLogicalAddress << 4)|0xB);

  /* Start of message */
  CEC_StartOfMessage();

  /* Wait till the header message is sent */
  while(count < HDMI_CEC_TIMEOUT_VALUE)
  {
    if (CEC_GetFlagStatus(CEC_FLAG_TXACKE) != RESET)
    {
      errorstatus = HDMI_CEC_GetErrorStatus();
      CEC_ClearFlag(CEC_FLAG_TXACKE);
      break;
    }
    else if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
    {
      errorstatus = HDMI_CEC_OK;
      break;
    }
    count++;
  }

  if (count >= HDMI_CEC_TIMEOUT_VALUE)
  {
    errorstatus = HDMI_CEC_TIMEOUT;
    return(errorstatus);
  }

  if (errorstatus == HDMI_CEC_OK)
  {
    HDMI_CEC_Follower_String[11][1] = (uint8_t *)"1";
    i++;
    errorstatus = HDMI_CEC_OK;
  }
  /* Clear CEC ISR register */
  CEC_ClearFlag(CEC_FLAG_TXBR|CEC_FLAG_TXEND);

  CECDevicesNumber = i - 1;

  return errorstatus;
}

/**
  * @brief  Sink Physical address discovery.
  * @param  None
  * @retval HDMI_CEC_Error: CEC Error code.
  */
static HDMI_CEC_Error SinkPhysicalAddressDiscovery(void)
{
  uint32_t DataNum=0;
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;

  I2C_InitTypeDef  I2C_InitStructure;
  GPIO_InitTypeDef GPIO_InitStructure;

  /*------------------------------ Physical address discovery ----------------*/
  /* if HDMI_CEC_ROOT == 0x1*/
  MyPhysicalAddress = 0x0000;
  /* The HDMI-CEC here is configured as sink or as a repeater. The configuration
  of the +5V power signal and the HPD should be well configured.
  Implement here the EDID Structure to be sent to the HDMI source.
  For more details please refer to the HDMI specification. 
  The EDID structure should be sent to the device source using the DDC Channel
  and using the HPD signal. */

  /* Enable CEC_I2C clocks */
  RCC_APB1PeriphClockCmd(HDMI_CEC_SINK_I2C_CLK, ENABLE);

  /* Enable CEC_I2C_GPIO and CEC_HPD_GPIO clocks */
  RCC_AHBPeriphClockCmd(HDMI_CEC_SINK_I2C_GPIO_CLK | HDMI_CEC_HPD_SINK_GPIO_CLK, ENABLE);

  I2C_DeInit(HDMI_CEC_SINK_I2C);

  GPIO_PinAFConfig(HDMI_CEC_SINK_I2C_GPIO_PORT, HDMI_CEC_SINK_I2C_SCL_PIN_SOURCE, GPIO_AF_1);
  GPIO_PinAFConfig(HDMI_CEC_SINK_I2C_GPIO_PORT, HDMI_CEC_SINK_I2C_SDA_PIN_SOURCE, GPIO_AF_1);

  /* Set PF6/PF7 as OD AF - I2C2_SCL/I2C2_SDA of Source*/
  GPIO_InitStructure.GPIO_Pin = HDMI_CEC_SINK_I2C_SDA_PIN | HDMI_CEC_SINK_I2C_SCL_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_Init(HDMI_CEC_SINK_I2C_GPIO_PORT, &GPIO_InitStructure);

  /* Set PA11 as PP - HPD */
  GPIO_InitStructure.GPIO_Pin = HDMI_CEC_HPD_SINK_PIN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_Init(HDMI_CEC_HPD_SINK_GPIO_PORT, &GPIO_InitStructure);

  RCC_I2CCLKConfig(RCC_I2C1CLK_HSI);

  HDMI_CEC_SINK_I2C->CR1 &= (uint16_t)(~I2C_CR1_NOSTRETCH);

  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_Timing = HDMI_CEC_I2C_TIMING;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  /* Initialize the I2C_DigitalFilter member */
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_OwnAddress1 = HDMI_CEC_I2C_SLAVE_ADDRESS7;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_Init(HDMI_CEC_SINK_I2C, &I2C_InitStructure);

  /* Enable HDMI_CEC_SINK_I2C */
  I2C_Cmd(HDMI_CEC_SINK_I2C, ENABLE);

  /* Wait until ADDR is set */
  while (!(HDMI_CEC_SINK_I2C->ISR & I2C_ISR_ADDR));

  /* Clear ADDR flag */
  HDMI_CEC_SINK_I2C->ICR = I2C_ICR_ADDRCF;

  while (!(HDMI_CEC_SINK_I2C->ISR & I2C_ISR_RXNE));

  HDMI_CEC_SINK_I2C->RXDR;

  /* Wait until ADDR is set */
  while (!(HDMI_CEC_SINK_I2C->ISR & I2C_ISR_ADDR));

  /* Clear ADDR flag */
  HDMI_CEC_SINK_I2C->ICR = I2C_ICR_ADDRCF;

  while (!(HDMI_CEC_SINK_I2C->ISR & I2C_ISR_TXIS));

  while (DataNum != 256)
  {
    if (HDMI_CEC_SINK_I2C->ISR & I2C_ISR_TXIS)
    {
      /* Write data to TXDR */
      HDMI_CEC_SINK_I2C->TXDR = (uint8_t)(EDID_Structure[DataNum++]);
    }
  }

  while (!(HDMI_CEC_SINK_I2C->ISR & I2C_ISR_STOPF));
  while (!(HDMI_CEC_SINK_I2C->ISR & I2C_ISR_NACKF));

  /* Clear STOPF and NACKF flag */
  HDMI_CEC_SINK_I2C->ICR = ( I2C_ICR_STOPCF | I2C_ICR_NACKCF);

  return errorstatus;
}

/**
  * @brief  Source Physical address discovery.
  * @param  None
  * @retval HDMI_CEC_Error: CEC Error code.
  */
static HDMI_CEC_Error SourcePhysicalAddressDiscovery(void)
{
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;

#ifdef HDMI_CEC_USE_DDC
  uint32_t  i = 0;
  uint32_t DataNum=0;
  GPIO_InitTypeDef GPIO_InitStructure;
  I2C_InitTypeDef  I2C_InitStructure;

  /* Enable CEC_I2C clocks */
  RCC_APB1PeriphClockCmd(HDMI_CEC_I2C_CLK, ENABLE);

  /* Enable CEC_I2C_GPIO and CEC_HPD_GPIO clocks */
  RCC_AHBPeriphClockCmd(HDMI_CEC_I2C_GPIO_CLK|HDMI_CEC_HPD_SOURCE_GPIO_CLK, ENABLE);

  GPIO_PinAFConfig(HDMI_CEC_I2C_GPIO_PORT,HDMI_CEC_I2C_SCL_PIN_SOURCE,GPIO_AF_1);
  GPIO_PinAFConfig(HDMI_CEC_I2C_GPIO_PORT,HDMI_CEC_I2C_SDA_PIN_SOURCE,GPIO_AF_1);

  /* Configure CEC_I2C_SCL_PIN and CEC_I2C_SDA_PIN  as Output open drain */
  GPIO_InitStructure.GPIO_Pin = HDMI_CEC_I2C_SCL_PIN|HDMI_CEC_I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_Init(HDMI_CEC_I2C_GPIO_PORT, &GPIO_InitStructure);

/* This configuration is only when the HDMI CEC is configured as source.
   The HDMI source has to provide the +5V Power signal to the sink. 
   Then, the source will wait for HPD signal to be asserted from the sink.
   Once the HPD signal is detected the source shall read the EDID structure 
   through the DDC channel. */
  /* Configure CEC_HPD_GPIO as Input pull down */
  GPIO_InitStructure.GPIO_Pin = HDMI_CEC_HPD_SOURCE_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
  GPIO_Init(HDMI_CEC_HPD_SOURCE_GPIO_PORT, &GPIO_InitStructure);

  /* I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack =I2C_Ack_Enable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_Timing = HDMI_CEC_I2C_TIMING;

  /* I2C configuration */
  I2C_Init(HDMI_CEC_I2C, &I2C_InitStructure);

  /* I2C Peripheral Enable */
  I2C_Cmd(HDMI_CEC_I2C, ENABLE);

  /* The HDMI-CEC here is configured as source or as a repeater. The configuration
  of the +5V power signal and the HPD should be well configured.
  The source should wait for HPD and then read the EDID structure. */
  while(GPIO_ReadInputDataBit(HDMI_CEC_HPD_SOURCE_GPIO_PORT, HDMI_CEC_HPD_SOURCE_PIN) == RESET)
  {
  }

  /* Wait for 100 ms after HPD was received */
  for(i = 0; i <0x5FFFFF; i++)
  {
  }
  
  /* Configure slave address, nbytes, reload and generate start */
  HDMI_CEC_I2C->CR2 = (uint32_t)(0xA0)|(uint32_t)(1 << 16)|I2C_CR2_START;

  /* Wait until TXIS is set */
  while (!(HDMI_CEC_I2C->ISR & I2C_ISR_TXIS));

    /* Send memory address */
    HDMI_CEC_I2C->TXDR = (uint8_t)(0x00);

    /* Wait until TC is set */
    while (!(HDMI_CEC_I2C->ISR & I2C_ISR_TC));

    /* Update CR2 : set Nbytes and reload */
    HDMI_CEC_I2C->CR2 = (uint32_t)(0xA0)|I2C_CR2_RD_WRN|I2C_CR2_START |(uint32_t)(255 << 16)|I2C_CR2_RELOAD;

    /* Wait until all data are received */
    while (DataNum != 255)
    {
      /* Wait until RXNE flag is set */
      while (!(HDMI_CEC_I2C->ISR & I2C_ISR_RXNE));

      /* Read data from RXDR */
      pBuffer[DataNum]= (uint8_t)HDMI_CEC_I2C->RXDR;

      /* Update number of received data */
      DataNum++;
    }

    /* Wait until TCR flag is set */
    while (!(HDMI_CEC_I2C->ISR & I2C_ISR_TCR));

    /* Update CR2 : set Nbytes and end mode */
    HDMI_CEC_I2C->CR2 = (uint32_t)(1 << 16)|I2C_CR2_AUTOEND;

    while (!(HDMI_CEC_I2C->ISR & I2C_ISR_RXNE));

    /* Read data from RXDR */
    pBuffer[DataNum]= (uint8_t)HDMI_CEC_I2C->RXDR;

    /* Wait until STOPF flag is set */      
    while (!(HDMI_CEC_I2C->ISR & I2C_ISR_STOPF));

    /* Clear STOPF flag */
    HDMI_CEC_I2C->ICR = I2C_ICR_STOPCF;

    MyPhysicalAddress = ((pBuffer[158] << 8)|pBuffer[159]);
#else
    MyPhysicalAddress = 0x1000;
#endif

  return errorstatus;
}

/**
  * @brief  Allocate the logical address.
  * @param  None
  * @retval HDMI_CEC_Error: CEC Error code.
  */
static HDMI_CEC_Error LogicalAddressAllocation(void)
{
  HDMI_CEC_Error errorstatus = HDMI_CEC_OK;
  uint32_t count = 0;

  /*------------------ Logical address allocation ----------------------------*/
  /* Get the device type */
  /* Device type = CEC_TV */
  if (DeviceType == HDMI_CEC_TV)
  {
    if (HDMI_CEC_ROOT)
    {
      MyLogicalAddress = 0x00;
    }
    else
    {
      CEC_OwnAddressConfig(0xE); /* Own address = 0xE */

      CEC_EndOfMessage();

      CEC_SendData(0xEE);

      /* Start of message */
      CEC_StartOfMessage();

      /* Wait till the header message is sent */
      while ((CEC_GetFlagStatus(CEC_FLAG_TXACKE) == RESET) && (count < HDMI_CEC_TIMEOUT_VALUE))
      {
        if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
        {
          break;
        }
        count++;
      }

      if (count >= HDMI_CEC_TIMEOUT_VALUE)
      {
        errorstatus = HDMI_CEC_TIMEOUT;
        return(errorstatus);
      }

      errorstatus = HDMI_CEC_GetErrorStatus();

      if (errorstatus == HDMI_CEC_TX_ACKNOWLEDGE)
      {
        MyLogicalAddress = 0x0E;
        errorstatus = HDMI_CEC_OK;
      }
      else if (errorstatus == HDMI_CEC_OK)
      {
        MyLogicalAddress = 0x0F;
        errorstatus = HDMI_CEC_DEVICE_UNREGISTRED;
      }
    }
    /* Clear CEC ISR register */
    CEC_ClearFlag(CEC_FLAG_TXEND);
  }

  /* Device type = CEC_RECORDING */
  if (DeviceType == HDMI_CEC_RECORDING)
  {
    CEC_OwnAddressConfig(0x1); /* Own address = 0x1 */

    CEC_EndOfMessage();

    CEC_SendData(0x11);

    /* Start of message */
    CEC_StartOfMessage();

    /* Wait till the header message is sent */
    while ((CEC_GetFlagStatus(CEC_FLAG_TXACKE) == RESET) && (count < HDMI_CEC_TIMEOUT_VALUE))
    {
      if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
      {
        break;
      }
      count++;
    }

    if (count >= HDMI_CEC_TIMEOUT_VALUE)
    {
      errorstatus = HDMI_CEC_TIMEOUT;
      return(errorstatus);
    }

    errorstatus = HDMI_CEC_GetErrorStatus();

    if (errorstatus == HDMI_CEC_TX_ACKNOWLEDGE)
    {
      MyLogicalAddress = 0x01;
      errorstatus = HDMI_CEC_OK;
    }
    else if (errorstatus == HDMI_CEC_OK)
    {
      /* Clear CEC ISR register */
      CEC_ClearFlag(CEC_FLAG_TXEND|CEC_FLAG_TXACKE|CEC_FLAG_TXBR);

      /* Clear the OAR */
      CEC_OwnAddressClear();

      CEC_OwnAddressConfig(0x2); /* Own address = 0x2 */

      CEC_EndOfMessage();

      CEC_SendData(0x22);

      /* Start of message */
      CEC_StartOfMessage();

      count = 0;

      /* Wait till the header message is sent */
      while ((CEC_GetFlagStatus(CEC_FLAG_TXACKE) == RESET) && (count < HDMI_CEC_TIMEOUT_VALUE))
      {
        if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
        {
          break;
        }
        count++;
      }

      if (count >= HDMI_CEC_TIMEOUT_VALUE)
      {
        errorstatus = HDMI_CEC_TIMEOUT;
        return(errorstatus);
      }
      errorstatus = HDMI_CEC_GetErrorStatus();

      if (errorstatus == HDMI_CEC_TX_ACKNOWLEDGE)
      {
        MyLogicalAddress = 0x02;
        errorstatus = HDMI_CEC_OK;
      }
      else if (errorstatus == HDMI_CEC_OK)
      {
        /* Clear CEC ISR register */
        CEC_ClearFlag( CEC_FLAG_TXEND);

        CEC_OwnAddressClear();

        CEC_OwnAddressConfig(0x9); /* Own address = 0x9 */

        CEC_EndOfMessage();

        CEC_SendData(0x99);

        /* Start of message */
        CEC_StartOfMessage();

        count = 0;

        /* Wait till the header message is sent */
        while ((CEC_GetFlagStatus(CEC_FLAG_TXACKE) == RESET) && (count < HDMI_CEC_TIMEOUT_VALUE))
        {
          if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
          {
            break;
          }
          count++;
        }

        if (count >= HDMI_CEC_TIMEOUT_VALUE)
        {
          errorstatus = HDMI_CEC_TIMEOUT;
          return(errorstatus);
        }
        errorstatus = HDMI_CEC_GetErrorStatus();

        if (errorstatus == HDMI_CEC_TX_ACKNOWLEDGE)
        {
          MyLogicalAddress = 0x09;
          errorstatus = HDMI_CEC_OK;
        }
        else if (errorstatus == HDMI_CEC_OK)
        {
          MyLogicalAddress = 0x0F;
          errorstatus = HDMI_CEC_DEVICE_UNREGISTRED;
        }
      }
    }
    /* Clear CEC ISR register */
    CEC_ClearFlag(CEC_FLAG_TXEND|CEC_FLAG_TXACKE|CEC_FLAG_TXBR);
  }

  /* Device type = CEC_TUNER */
  if (DeviceType == HDMI_CEC_TUNER)
  {
    CEC_OwnAddressConfig(0x3); /* Own address = 0x3 */

    CEC_EndOfMessage();

    CEC_SendData(0x33);

    /* Start of message */
    CEC_StartOfMessage();

    count = 0;

    /* Wait till the header message is sent */
    while ((CEC_GetFlagStatus(CEC_FLAG_TXACKE) == RESET) && (count < HDMI_CEC_TIMEOUT_VALUE))
    {
      if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
      {
        break;
      }
      count++;
    }

    if (count >= HDMI_CEC_TIMEOUT_VALUE)
    {
      errorstatus = HDMI_CEC_TIMEOUT;
      return(errorstatus);
    }
    errorstatus = HDMI_CEC_GetErrorStatus();

    if (errorstatus == HDMI_CEC_TX_ACKNOWLEDGE)
    {
      MyLogicalAddress = 0x03;
      errorstatus = HDMI_CEC_OK;
    }
    else if (errorstatus == HDMI_CEC_OK)
    {
      /* Clear CEC ISR register */
      CEC_ClearFlag(CEC_FLAG_TXEND);

      /* Clear the OAR */
      CEC_OwnAddressClear();

      CEC_OwnAddressConfig(0x6); /* Own address = 0x6 */

      CEC_EndOfMessage();

      CEC_SendData(0x66);

      /* Start of message */
      CEC_StartOfMessage();

      count = 0;

      /* Wait till the header message is sent */
      while ((CEC_GetFlagStatus(CEC_FLAG_TXACKE) == RESET) && (count < HDMI_CEC_TIMEOUT_VALUE))
      {
        if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
        {
          break;
        }
        count++;
      }

      if (count >= HDMI_CEC_TIMEOUT_VALUE)
      {
        errorstatus = HDMI_CEC_TIMEOUT;
        return(errorstatus);
      }
      errorstatus = HDMI_CEC_GetErrorStatus();

      if (errorstatus == HDMI_CEC_TX_ACKNOWLEDGE)
      {
        MyLogicalAddress = 0x06;
        errorstatus = HDMI_CEC_OK;
      }
      else if (errorstatus == HDMI_CEC_OK)
      {
        /* Clear CEC ISR register */
        CEC_ClearFlag(CEC_FLAG_TXEND);

        /* Clear the OAR */
        CEC_OwnAddressClear();

        CEC_OwnAddressConfig(0x7); /* Own address = 0x7 */

        CEC_EndOfMessage();

        CEC_SendData(0x77);

        /* Start of message */
        CEC_StartOfMessage();

        count = 0;

        /* Wait till the header message is sent */
        while ((CEC_GetFlagStatus(CEC_FLAG_TXACKE) == RESET) && (count < HDMI_CEC_TIMEOUT_VALUE))
        {
          if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
          {
            break;
          }
          count++;
        }

        if (count >= HDMI_CEC_TIMEOUT_VALUE)
        {
          errorstatus = HDMI_CEC_TIMEOUT;
          return(errorstatus);
        }
        errorstatus = HDMI_CEC_GetErrorStatus();

        if (errorstatus == HDMI_CEC_TX_ACKNOWLEDGE)
        {
          MyLogicalAddress = 0x07;
          errorstatus = HDMI_CEC_OK;
        }
        else if (errorstatus == HDMI_CEC_OK)
        {
          /* Clear CEC ISR register */
          CEC_ClearFlag(CEC_FLAG_TXEND);

          /* Clear the OAR */
          CEC_OwnAddressClear();

          CEC_OwnAddressConfig(0xA); /* Own address = 0xA */

          CEC_EndOfMessage();

          CEC_SendData(0xAA);

          /* Start of message */
          CEC_StartOfMessage();

          count = 0;

          /* Wait till the header message is sent */
          while ((CEC_GetFlagStatus(CEC_FLAG_TXACKE) == RESET) && (count < HDMI_CEC_TIMEOUT_VALUE))
          {
            if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
            {
              break;
            }
            count++;
          }
          if (count >= HDMI_CEC_TIMEOUT_VALUE)
          {
            errorstatus = HDMI_CEC_TIMEOUT;
            return(errorstatus);
          }
          errorstatus = HDMI_CEC_GetErrorStatus();
          
          if (errorstatus == HDMI_CEC_TX_ACKNOWLEDGE)
          {
            MyLogicalAddress = 0x0A;
            errorstatus = HDMI_CEC_OK;
          }
          else if (errorstatus == HDMI_CEC_OK)
          {
            MyLogicalAddress = 0x0F;
            errorstatus = HDMI_CEC_DEVICE_UNREGISTRED;
          }
        }
      }
    }
    /* Clear CEC ISR register */
    CEC_ClearFlag(CEC_FLAG_TXEND);
  }

  /* Device type = CEC_PLAYBACK */
  if (DeviceType == HDMI_CEC_PLAYBACK)
  {
    CEC_OwnAddressConfig(0x4); /* Own address = 0x4 */

    CEC_EndOfMessage();

     CEC_SendData(0x44);

    /* Start of message */
    CEC_StartOfMessage();

    count = 0;

    /* Wait till the header message is sent */
    while ((CEC_GetFlagStatus(CEC_FLAG_TXACKE) == RESET) && (count < HDMI_CEC_TIMEOUT_VALUE))
    {
      if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
      {
        break;
      }
      count++;
    }
    if (count >= HDMI_CEC_TIMEOUT_VALUE)
    {
      errorstatus = HDMI_CEC_TIMEOUT;
      return(errorstatus);
    }

    errorstatus = HDMI_CEC_GetErrorStatus();

    if (errorstatus == HDMI_CEC_TX_ACKNOWLEDGE)
    {
      MyLogicalAddress = 0x04;
      errorstatus = HDMI_CEC_OK;
    }
    else if (errorstatus == HDMI_CEC_OK)
    {
      /* Clear CEC ISR register */
      CEC_ClearFlag(CEC_FLAG_TXEND);

      /* Clear the OAR */
      CEC_OwnAddressClear();

      CEC_OwnAddressConfig(0x8); /* Own address = 0x8 */

      CEC_EndOfMessage();

      CEC_SendData(0x88);

      /* Start of message */
      CEC_StartOfMessage();

      count = 0;

      /* Wait till the header message is sent */
      while ((CEC_GetFlagStatus(CEC_FLAG_TXACKE) == RESET) && (count < HDMI_CEC_TIMEOUT_VALUE))
      {
        if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
        {
          break;
        }
        count++;
      }

      if (count >= HDMI_CEC_TIMEOUT_VALUE)
      {
        errorstatus = HDMI_CEC_TIMEOUT;
        return(errorstatus);
      }
      errorstatus = HDMI_CEC_GetErrorStatus();

      if (errorstatus == HDMI_CEC_TX_ACKNOWLEDGE)
      {
        MyLogicalAddress = 0x08;
        errorstatus = HDMI_CEC_OK;
      }
      else if (errorstatus == HDMI_CEC_OK)
      {
        /* Clear CEC ISR register */
        CEC_ClearFlag(CEC_FLAG_TXEND);

        /* Clear the OAR */
        CEC_OwnAddressClear();

        CEC_OwnAddressConfig(0xB); /* Own address = 0xBB */

        CEC_EndOfMessage();

        CEC_SendData(0xBB);

        /* Start of message */
        CEC_StartOfMessage();

        count = 0;

        /* Wait till the header message is sent */
        while ((CEC_GetFlagStatus(CEC_FLAG_TXACKE) == RESET) && (count < HDMI_CEC_TIMEOUT_VALUE))
        {
          if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
          {
            break;
          }
          count++;
        }
        /* Clear the OAR */
        CEC_OwnAddressClear();

        if (count >= HDMI_CEC_TIMEOUT_VALUE)
        {
          errorstatus = HDMI_CEC_TIMEOUT;
          return(errorstatus);
        }

        errorstatus = HDMI_CEC_GetErrorStatus();

        if (errorstatus == HDMI_CEC_TX_ACKNOWLEDGE)
        {
          MyLogicalAddress = 0x0B;
          errorstatus = HDMI_CEC_OK;
        }
        else if (errorstatus == HDMI_CEC_OK)
        {
          MyLogicalAddress = 0x0F;
          errorstatus = HDMI_CEC_DEVICE_UNREGISTRED;
        }
      }
    }
    /* Clear CEC ISR register */
    CEC_ClearFlag(CEC_FLAG_TXEND);
  }

  /* Device type = CEC Audio System */
  if (DeviceType == HDMI_CEC_AUDIOSYSTEM)
  {
    CEC_OwnAddressConfig(0x5); /* Own address = 0x5 */

    CEC_EndOfMessage();

    CEC_SendData(0x55);

    /* Start of message */
    CEC_StartOfMessage();

    count = 0;

    /* Wait till the header message is sent */
    while ((CEC_GetFlagStatus(CEC_FLAG_TXACKE) == RESET) && (count < HDMI_CEC_TIMEOUT_VALUE))
    {
      if(CEC_GetFlagStatus(CEC_FLAG_TXEND) != RESET)
      {
        break;
      }
      count++;
    }

    if (count >= HDMI_CEC_TIMEOUT_VALUE)
    {
      errorstatus = HDMI_CEC_TIMEOUT;
      return(errorstatus);
    }
    
    errorstatus = HDMI_CEC_GetErrorStatus();

    if (errorstatus == HDMI_CEC_TX_ACKNOWLEDGE)
    {
      MyLogicalAddress = 0x05;
      errorstatus = HDMI_CEC_OK;
    }
    else if (errorstatus == HDMI_CEC_OK)
    {
      MyLogicalAddress = 0x0F;
      errorstatus = HDMI_CEC_DEVICE_UNREGISTRED;
    }

    /* Clear CEC ISR register */
    CEC_ClearFlag(CEC_FLAG_TXEND);
  }

  return errorstatus;
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
