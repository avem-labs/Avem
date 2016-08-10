/**
  ******************************************************************************
  * @file    nor_if.c
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   specific media access Layer for NOR flash
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

#include "platform_config.h"

#ifdef USE_STM3210E_EVAL

/* Includes ------------------------------------------------------------------*/
#include "fsmc_nor.h"
#include "nor_if.h"
#include "dfu_mal.h"
#include "stm32f10x_fsmc.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern NOR_IDTypeDef NOR_ID;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : NOR_If_Init
* Description    : Initializes the Media on the STM32
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t NOR_If_Init(void)
{
  /* Configure FSMC Bank1 NOR/SRAM2 */
  FSMC_NOR_Init();

  /* Enable FSMC Bank1 NOR/SRAM2 */
  FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM2, ENABLE);

  return MAL_OK;
}

/*******************************************************************************
* Function Name  : NOR_If_Erase
* Description    : Erase sector
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t NOR_If_Erase(uint32_t Address)
{
  /* Erase the destination memory */
  FSMC_NOR_EraseBlock(Address & 0x00FFFFFF);    

  return MAL_OK;
}

/*******************************************************************************
* Function Name  : NOR_If_Write
* Description    : Write sectors
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
uint16_t NOR_If_Write(uint32_t Address, uint32_t DataLength)
{
  if ((DataLength & 1) == 1) /* Not an aligned data */
  {
    DataLength += 1;
    MAL_Buffer[DataLength-1] = 0xFF;
  }
  
  FSMC_NOR_WriteBuffer((uint16_t *)MAL_Buffer, (Address&0x00FFFFFF), DataLength >> 1);  
  
  return MAL_OK;
}

/*******************************************************************************
* Function Name  : NOR_If_Read
* Description    : Read sectors
* Input          : None
* Output         : None
* Return         : buffer address pointer
*******************************************************************************/
uint8_t *NOR_If_Read(uint32_t Address, uint32_t DataLength)
{
  return  (uint8_t*)(Address);
}

#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
