/**
  ******************************************************************************
  * @file    usb_scsi.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   All processing related to the SCSI commands
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
#ifndef __USB_SCSI_H
#define __USB_SCSI_H

/* Includes ------------------------------------------------------------------*/
#include "hw_config.h"
#include "usb_type.h"

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/

/* SCSI Commands */
#define SCSI_FORMAT_UNIT                            0x04
#define SCSI_INQUIRY                                0x12
#define SCSI_MODE_SELECT6                           0x15
#define SCSI_MODE_SELECT10                          0x55
#define SCSI_MODE_SENSE6                            0x1A
#define SCSI_MODE_SENSE10                           0x5A
#define SCSI_ALLOW_MEDIUM_REMOVAL                   0x1E
#define SCSI_READ6                                  0x08
#define SCSI_READ10                                 0x28
#define SCSI_READ12                                 0xA8
#define SCSI_READ16                                 0x88

#define SCSI_READ_CAPACITY10                        0x25
#define SCSI_READ_CAPACITY16                        0x9E

#define SCSI_REQUEST_SENSE                          0x03
#define SCSI_START_STOP_UNIT                        0x1B
#define SCSI_TEST_UNIT_READY                        0x00
#define SCSI_WRITE6                                 0x0A
#define SCSI_WRITE10                                0x2A
#define SCSI_WRITE12                                0xAA
#define SCSI_WRITE16                                0x8A

#define SCSI_VERIFY10                               0x2F
#define SCSI_VERIFY12                               0xAF
#define SCSI_VERIFY16                               0x8F

#define SCSI_SEND_DIAGNOSTIC                        0x1D
#define SCSI_READ_FORMAT_CAPACITIES                 0x23

#define NO_SENSE		                    0
#define RECOVERED_ERROR		                    1
#define NOT_READY		                    2
#define MEDIUM_ERROR		                    3
#define HARDWARE_ERROR		                    4
#define ILLEGAL_REQUEST		                    5
#define UNIT_ATTENTION		                    6
#define DATA_PROTECT		                    7
#define BLANK_CHECK		                    8
#define VENDOR_SPECIFIC		                    9
#define COPY_ABORTED		                    10
#define ABORTED_COMMAND		                    11
#define VOLUME_OVERFLOW		                    13
#define MISCOMPARE		                    14


#define INVALID_COMMAND                             0x20
#define INVALID_FIELED_IN_COMMAND                   0x24
#define PARAMETER_LIST_LENGTH_ERROR                 0x1A
#define INVALID_FIELD_IN_PARAMETER_LIST             0x26
#define ADDRESS_OUT_OF_RANGE                        0x21
#define MEDIUM_NOT_PRESENT 			    0x3A
#define MEDIUM_HAVE_CHANGED			    0x28

#define READ_FORMAT_CAPACITY_DATA_LEN               0x0C
#define READ_CAPACITY10_DATA_LEN                    0x08
#define MODE_SENSE10_DATA_LEN                       0x08
#define MODE_SENSE6_DATA_LEN                        0x04
#define REQUEST_SENSE_DATA_LEN                      0x12
#define STANDARD_INQUIRY_DATA_LEN                   0x24
#define BLKVFY                                      0x04

extern  uint8_t Page00_Inquiry_Data[];
extern  uint8_t Standard_Inquiry_Data[];
extern  uint8_t Standard_Inquiry_Data2[];
extern  uint8_t Mode_Sense6_data[];
extern  uint8_t Mode_Sense10_data[];
extern  uint8_t Scsi_Sense_Data[];
extern  uint8_t ReadCapacity10_Data[];
extern  uint8_t ReadFormatCapacity_Data [];

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
void SCSI_Inquiry_Cmd(uint8_t lun);
void SCSI_ReadFormatCapacity_Cmd(uint8_t lun);
void SCSI_ReadCapacity10_Cmd(uint8_t lun);
void SCSI_RequestSense_Cmd (uint8_t lun);
void SCSI_Start_Stop_Unit_Cmd(uint8_t lun);
void SCSI_ModeSense6_Cmd (uint8_t lun);
void SCSI_ModeSense10_Cmd (uint8_t lun);
void SCSI_Write10_Cmd(uint8_t lun , uint32_t LBA , uint32_t BlockNbr);
void SCSI_Read10_Cmd(uint8_t lun , uint32_t LBA , uint32_t BlockNbr);
void SCSI_Verify10_Cmd(uint8_t lun);

void SCSI_Invalid_Cmd(uint8_t lun);
void SCSI_Valid_Cmd(uint8_t lun);
bool SCSI_Address_Management(uint8_t lun , uint8_t Cmd , uint32_t LBA , uint32_t BlockNbr);

void Set_Scsi_Sense_Data(uint8_t lun , uint8_t Sens_Key, uint8_t Asc);
void SCSI_TestUnitReady_Cmd (uint8_t lun);
void SCSI_Format_Cmd (uint8_t lun);

//#define SCSI_TestUnitReady_Cmd           SCSI_Valid_Cmd
#define SCSI_Prevent_Removal_Cmd         SCSI_Valid_Cmd

/* Invalid (Unsupported) commands */
#define SCSI_READ_CAPACITY16_Cmd         SCSI_Invalid_Cmd
//#define SCSI_FormatUnit_Cmd              SCSI_Invalid_Cmd
#define SCSI_Write6_Cmd                  SCSI_Invalid_Cmd
#define SCSI_Write16_Cmd                 SCSI_Invalid_Cmd
#define SCSI_Write12_Cmd                 SCSI_Invalid_Cmd
#define SCSI_Read6_Cmd                   SCSI_Invalid_Cmd
#define SCSI_Read12_Cmd                  SCSI_Invalid_Cmd
#define SCSI_Read16_Cmd                  SCSI_Invalid_Cmd
#define SCSI_Send_Diagnostic_Cmd         SCSI_Invalid_Cmd
#define SCSI_Mode_Select6_Cmd            SCSI_Invalid_Cmd
#define SCSI_Mode_Select10_Cmd           SCSI_Invalid_Cmd
#define SCSI_Verify12_Cmd                SCSI_Invalid_Cmd
#define SCSI_Verify16_Cmd                SCSI_Invalid_Cmd

#endif /* __USB_SCSI_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

