/**
  ******************************************************************************
  * @file    i2s_codec.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   This file contains all the functions prototypes for the I2S codec
  *          firmware driver.
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
#ifndef __I2S_CODEC_H
#define __I2S_CODEC_H

/* .WAV file format :

 Endian      Offset      Length      Contents
  big         0           4 bytes     'RIFF'             // 0x52494646
  little      4           4 bytes     <file length - 8>
  big         8           4 bytes     'WAVE'             // 0x57415645

Next, the fmt chunk describes the sample format:

  big         12          4 bytes     'fmt '          // 0x666D7420
  little      16          4 bytes     0x00000010      // Length of the fmt data (16 bytes)
  little      20          2 bytes     0x0001          // Format tag: 1 = PCM
  little      22          2 bytes     <channels>      // Channels: 1 = mono, 2 = stereo
  little      24          4 bytes     <sample rate>   // Samples per second: e.g., 22050
  little      28          4 bytes     <bytes/second>  // sample rate * block align
  little      32          2 bytes     <block align>   // channels * bits/sample / 8
  little      34          2 bytes     <bits/sample>   // 8 or 16

Finally, the data chunk contains the sample data:

  big         36          4 bytes     'data'        // 0x64617461
  little      40          4 bytes     <length of the data block>
  little      44          *           <sample data>

*/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x.h"

/* Exported types ------------------------------------------------------------*/

/* Audio file information structure */
typedef struct
{
  uint32_t  RIFFchunksize;
  uint16_t  FormatTag;
  uint16_t  NumChannels;
  uint32_t  SampleRate;
  uint32_t  ByteRate;
  uint16_t  BlockAlign;
  uint16_t  BitsPerSample;
  uint32_t  DataSize;
} WAVE_FormatTypeDef;

/* Error Identification structure */
typedef enum
{
  Valid_WAVE_File = 0,
  Unvalid_RIFF_ID,
  Unvalid_WAVE_Format,
  Unvalid_FormatChunk_ID,
  Unsupporetd_FormatTag,
  Unsupporetd_Number_Of_Channel,
  Unsupporetd_Sample_Rate,
  Unsupporetd_Bits_Per_Sample,
  Unvalid_DataChunk_ID,
  Unsupporetd_ExtraFormatBytes,
  Unvalid_FactChunk_ID
} ErrorCode;
 
/* Exported constants --------------------------------------------------------*/

/* Codec output DEVICE */
#define OutputDevice_SPEAKER          1
#define OutputDevice_HEADPHONE        2  
#define OutputDevice_BOTH             3

/* VOLUME control constants */
#define DEFAULT_VOL                   0x52
#define VolumeDirection_HIGH          0xF
#define VolumeDirection_LOW           0xA
#define VolumeDirection_LEVEL         0x0
#define VOLStep                       4 

/* Forward and Rewind constants */
#define STEP_FORWARD                  2 /* 2% of wave file data length*/
#define STEP_BACK                     6 /* 6% of wave file data length*/

/* Codec POWER DOWN modes */
#define CodecPowerDown_HW             1
#define CodecPowerDown_SW 	          2

/* Audio Play STATUS */
#define AudioPlayStatus_STOPPED       0
#define AudioPlayStatus_PLAYING	      1
#define AudioPlayStatus_PAUSED		    2	 

/* MUTE commands */ 
#define MUTE_ON                       1
#define MUTE_OFF							        0

/* I2S configuration parameters */
#define I2S_STANDARD                  I2S_Standard_Phillips
#define I2S_MCLKOUTPUT                I2S_MCLKOutput_Enable

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */
 
  /* CONFIGURATION */
uint32_t I2S_CODEC_Init(uint32_t OutputDevice, uint32_t Address);
void I2S_CODEC_ReplayConfig(uint32_t Repetions);

	/* AUDIO CONTROL */
uint32_t I2S_CODEC_Play(uint32_t AudioStartPosition);
uint32_t I2S_CODEC_Pause(void);
uint32_t I2S_CODEC_Stop(void);	
uint32_t I2S_CODEC_ControlVolume(uint32_t direction, uint8_t volume);
void I2S_CODEC_Mute(uint32_t Command);
void I2S_CODEC_ForwardPlay(uint32_t Step);
void I2S_CODEC_RewindPlay(uint32_t Step);

   /* EXTRA CONTROLS */								 
void I2S_CODEC_PowerDown(uint32_t CodecPowerDown_Mode);
void I2S_CODEC_Reset(void);
uint32_t I2S_CODEC_SpeakerHeadphoneSwap(uint32_t CODEC_AudioOutput, uint32_t Address);
uint8_t GetVar_CurrentVolume(void);

/* Medium Layer Codec Functions ----------------------------------------------*/						
void I2S_CODEC_DataTransfer(void);
void I2S_CODEC_UpdateStatus(void);
uint32_t GetVar_DataStartAddr(void);	
ErrorCode I2S_CODEC_WaveParsing(uint8_t* HeaderTab);
uint32_t GetVar_CurrentOutputDevice(void);

uint32_t GetVar_AudioDataIndex(void);
void SetVar_AudioDataIndex(uint32_t value);
void ResetVar_AudioDataIndex(void);
void IncrementVar_AudioDataIndex(uint32_t IncrementNumber);
void DecrementVar_AudioDataIndex(uint32_t DecrementNumber);

uint32_t GetVar_AudioReplay(void);
void Decrement_AudioReplay(void);

uint32_t GetVar_AudioPlayStatus(void);
uint32_t SetVar_AudioPlayStatus(uint32_t Status);

uint16_t GetVar_i2saudiofreq(void);
uint32_t GetVar_AudioDataLength(void);

/* Low Layer Codec Function --------------------------------------------------*/
uint32_t SetVar_SendDummyData(void);
uint32_t ResetVar_SendDummyData(void);
uint32_t GetVar_SendDummyData(void);
uint32_t AudioFile_Init(void);

void I2S_GPIO_Config(void); 
void I2S_Config(uint16_t Standard, uint16_t MCLKOutput, uint16_t AudioFreq);
uint32_t CODEC_Config(uint16_t AudioOutput, uint16_t I2S_Standard, uint16_t I2S_MCLKOutput, uint8_t Volume);

uint32_t Media_Init(void);
void Media_BufferRead(uint8_t* pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead);
void Media_StartReadSequence(uint32_t ReadAddr);
uint16_t Media_ReadHalfWord(uint32_t Offset);
uint8_t Media_ReadByte(uint32_t Offset);

void delay(__IO uint32_t nCount); 

#endif /* __I2S_CODEC_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
