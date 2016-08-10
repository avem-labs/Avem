/**
  ******************************************************************************
  * @file    usb_prop.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   All processing related to Mass Storage Demo (Endpoint 0)
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


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usb_prop_H
#define __usb_prop_H

/* Includes ------------------------------------------------------------------*/
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void Speaker_init(void);
void Speaker_Reset(void);
void Speaker_SetConfiguration(void);
void Speaker_SetDeviceAddress (void);
void Speaker_Status_In (void);
void Speaker_Status_Out (void);
RESULT Speaker_Data_Setup(uint8_t);
RESULT Speaker_NoData_Setup(uint8_t);
RESULT Speaker_Get_Interface_Setting(uint8_t Interface, uint8_t AlternateSetting);
uint8_t *Speaker_GetDeviceDescriptor(uint16_t );
uint8_t *Speaker_GetConfigDescriptor(uint16_t);
uint8_t *Speaker_GetStringDescriptor(uint16_t);
uint8_t *Mute_Command(uint16_t Length);

/* Exported define -----------------------------------------------------------*/
#define Speaker_GetConfiguration          NOP_Process
//#define Speaker_SetConfiguration          NOP_Process
#define Speaker_GetInterface              NOP_Process
#define Speaker_SetInterface              NOP_Process
#define Speaker_GetStatus                 NOP_Process
#define Speaker_ClearFeature              NOP_Process
#define Speaker_SetEndPointFeature        NOP_Process
#define Speaker_SetDeviceFeature          NOP_Process
//#define Speaker_SetDeviceAddress          NOP_Process
#define GET_CUR                           0x81
#define SET_CUR                           0x01

#endif /* __usb_prop_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

