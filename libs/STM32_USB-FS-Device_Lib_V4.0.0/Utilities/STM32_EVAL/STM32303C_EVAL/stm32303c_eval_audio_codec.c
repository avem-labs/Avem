/**
  ******************************************************************************
  * @file    stm32303c_eval_audio_codec.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    23-October-2012
  * @brief   This file includes the low layer driver for CS42L52 Audio Codec
  *          available on STM32303C_EVAL evaluation board(MB1019).  
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

/*==============================================================================================================================
                                             User NOTES
1. How To use this driver:
--------------------------
   - This driver supports STM32F30x devices on STM32303C_EVAL (MB1019) Evaluation boards.

   - Configure the options in file stm32303c_eval_audio_codec.h in the section CONFIGURATION.
      Refer to the sections 2 and 3 to have more details on the possible configurations.

   - Call the function EVAL_AUDIO_Init(
                                    OutputDevice: physical output mode (OUTPUT_DEVICE_SPEAKER, 
                                                 OUTPUT_DEVICE_HEADPHONE, OUTPUT_DEVICE_AUTO or 
                                                 OUTPUT_DEVICE_BOTH)
                                    Volume: initial volume to be set (0 is min (mute), 100 is max (100%)
                                    AudioFreq: Audio frequency in Hz (8000, 16000, 22500, 32000 ...)
                                     this parameter is relative to the audio file/stream type.
                                   )
      This function configures all the hardware required for the audio application (codec, I2C, I2S, 
      GPIOs, DMA and interrupt if needed). This function returns 0 if configuration is OK.
      if the returned value is different from 0 or the function is stuck then the communication with
      the codec has failed (try to un-plug the power or reset device in this case).
      + OUTPUT_DEVICE_SPEAKER: only speaker will be set as output for the audio stream.
      + OUTPUT_DEVICE_HEADPHONE: only headphones will be set as output for the audio stream.
      + OUTPUT_DEVICE_AUTO: Selection of output device is made through external switch (implemented 
         into the audio jack on the evaluation board). When the Headphone is connected it is used
         as output. When the headphone is disconnected from the audio jack, the output is
         automatically switched to Speaker.
      + OUTPUT_DEVICE_BOTH: both Speaker and Headphone are used as outputs for the audio stream
         at the same time.

   - Call the function EVAL_AUDIO_Play(
                                  pBuffer: pointer to the audio data file address
                                  Size: size of the buffer to be sent in Bytes
                                 )
      to start playing (for the first time) from the audio file/stream.

   - Call the function EVAL_AUDIO_PauseResume(
                                         Cmd: AUDIO_PAUSE (or 0) to pause playing or AUDIO_RESUME (or 
                                               any value different from 0) to resume playing.
                                         )
       Note. After calling EVAL_AUDIO_PauseResume() function for pause, only EVAL_AUDIO_PauseResume() should be called
          for resume (it is not allowed to call EVAL_AUDIO_Play() in this case).
       Note. This function should be called only when the audio file is played or paused (not stopped).

   - For each mode, you may need to implement the relative callback functions into your code.
      The Callback functions are named EVAL_AUDIO_XXX_CallBack() and only their prototypes are declared in 
      the stm32303c_eval_audio_codec.h file. (refer to the example for more details on the callbacks implementations)

   - To Stop playing, to modify the volume level or to mute, use the functions
       EVAL_AUDIO_Stop(), EVAL_AUDIO_VolumeCtl() and EVAL_AUDIO_Mute().

   - The driver API and the callback functions are at the end of the stm32303c_eval_audio_codec.h file.
 

 Driver architecture:
 --------------------
 This driver is composed of three main layers:
   o High Audio Layer: consists of the function API exported in the stm32303c_eval_audio_codec.h file
     (EVAL_AUDIO_Init(), EVAL_AUDIO_Play() ...)
   o Codec Control layer: consists of the functions API controlling the audio codec (CS42L52) and 
     included as local functions in file stm32303c_eval_audio_codec.c (Codec_Init(), Codec_Play() ...)
   o Media Access Layer (MAL): which consists of functions allowing to access the media containing/
     providing the audio file/stream. These functions are also included as local functions into
     the stm32303c_eval_audio_codec.c file (Audio_MAL_Init(), Audio_MAL_Play() ...)
  Each set of functions (layer) may be implemented independently of the others and customized when 
  needed.    

2. Modes description:
---------------------
     + AUDIO_MAL_MODE_NORMAL : is suitable when the audio file is in a memory location.
     + AUDIO_MAL_MODE_CIRCULAR: is suitable when the audio data are read either from a 
        memory location or from a device at real time (double buffer could be used).

3. DMA interrupts description:
------------------------------
     + EVAL_AUDIO_IT_TC_ENABLE: Enable this define to use the DMA end of transfer interrupt.
        then, a callback should be implemented by user to perform specific actions
        when the DMA has finished the transfer.
     + EVAL_AUDIO_IT_HT_ENABLE: Enable this define to use the DMA end of half transfer interrupt.
        then, a callback should be implemented by user to perform specific actions
        when the DMA has reached the half of the buffer transfer (generally, it is useful 
        to load the first half of buffer while DMA is loading from the second half).
     + EVAL_AUDIO_IT_ER_ENABLE: Enable this define to manage the cases of error on DMA transfer.

4. Known Limitations:
---------------------
   1- When using the Speaker, if the audio file quality is not high enough, the speaker output
      may produce high and uncomfortable noise level. To avoid this issue, to use speaker
      output properly, try to increase audio file sampling rate (typically higher than 48KHz).
      This operation will lead to larger file size.
   2- Communication with the audio codec (through I2C) may be corrupted if it is interrupted by some
      user interrupt routines (in this case, interrupts could be disabled just before the start of 
      communication then re-enabled when it is over). Note that this communication is only done at
      the configuration phase (EVAL_AUDIO_Init() or EVAL_AUDIO_Stop()) and when Volume control modification is 
      performed (EVAL_AUDIO_VolumeCtl() or EVAL_AUDIO_Mute()). When the audio data is played, no communication is 
      required with the audio codec.
  3- Parsing of audio file is not implemented (in order to determine audio file properties: Mono/Stereo, Data size, 
     File size, Audio Frequency, Audio Data header size ...). The configuration is fixed for the given audio file.
  4- Mono audio streaming is not supported (in order to play mono audio streams, each data should be sent twice 
     on the I2S or should be duplicated on the source buffer. Or convert the stream in stereo before playing).
  5- Supports only 16-bit audio data size.
===============================================================================================================================*/


/* Includes ------------------------------------------------------------------*/
#include "stm32303c_eval_audio_codec.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM32303C_EVAL
  * @{
  */

/** @addtogroup STM32303C_EVAL_AUDIO_CODEC
  * @brief       This file includes the low layer driver for CS42L52 Audio Codec
  *              available on STM32303C_EVAL evaluation board(MB1019).
  * @{
  */ 

/** @defgroup STM32303C_EVAL_AUDIO_CODEC_Private_Types
  * @{
  */ 
/**
  * @}
  */ 
  
/** @defgroup STM32303C_EVAL_AUDIO_CODEC_Private_Defines
  * @{
  */ 

/* Mask for the bit EN of the I2S CFGR register */
#define I2S_ENABLE_MASK                 0x0400

/* Delay for the Codec to be correctly reset */
#define CODEC_RESET_DELAY               0x4FFF

/* Codec audio Standards */
#ifdef I2S_STANDARD_PHILLIPS
 #define  CODEC_STANDARD                0x04
 #define I2S_STANDARD                   I2S_Standard_Phillips         
#elif defined(I2S_STANDARD_MSB)
 #define  CODEC_STANDARD                0x00
 #define I2S_STANDARD                   I2S_Standard_MSB    
#elif defined(I2S_STANDARD_LSB)
 #define  CODEC_STANDARD                0x08
 #define I2S_STANDARD                   I2S_Standard_LSB    
#else 
 #error "Error: No audio communication standard selected !"
#endif /* I2S_STANDARD */

/* The 7 bits Codec address (sent through I2C interface) */
#define CODEC_ADDRESS                   0x94  /* b1001010. */
/**
  * @}
  */ 

/** @defgroup STM32303C_EVAL_AUDIO_CODEC_Private_Macros
  * @{
  */
/**
  * @}
  */ 
  
/** @defgroup STM32303C_EVAL_AUDIO_CODEC_Private_Variables
  * @{
  */
/* This structure is declared global because it is handled by two different functions */
DMA_InitTypeDef DMA_InitStructure; 
DMA_InitTypeDef AUDIO_MAL_DMA_InitStructure;

uint32_t AudioTotalSize = 0xFFFF; /* This variable holds the total size of the audio file */
uint32_t AudioRemSize   = 0xFFFF; /* This variable holds the remaining data in audio file */
uint16_t *CurrentPos;             /* This variable holds the current position of audio pointer */

__IO uint32_t  CODECTimeout = CODEC_LONG_TIMEOUT;   
__IO uint8_t OutputDev = 0;

/**
  * @}
  */ 

/** @defgroup STM32303C_EVAL_AUDIO_CODEC_Private_Function_Prototypes
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32303C_EVAL_AUDIO_CODEC_Private_Functions
  * @{
  */ 

/*----------------------------------------------------------------------------
                           Audio Codec functions 
  ----------------------------------------------------------------------------*/
/* High Layer codec functions */
static uint32_t Codec_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq);
static uint32_t Codec_DeInit(void);
static uint32_t Codec_Play(void);
static uint32_t Codec_PauseResume(uint32_t Cmd);
static uint32_t Codec_Stop(uint32_t Cmd);
static uint32_t Codec_VolumeCtrl(uint8_t Volume);
static uint32_t Codec_Mute(uint32_t Cmd);
/* Low layer codec functions */
static void     Codec_CtrlInterface_Init(void);
static void     Codec_CtrlInterface_DeInit(void);
static void     Codec_AudioInterface_Init(uint32_t AudioFreq);
static void     Codec_AudioInterface_DeInit(void);
uint32_t Codec_ReadRegister(uint8_t RegisterAddr);
static void     Codec_GPIO_Init(void);
static void     Codec_GPIO_Recorder_Init(void);
static void     Codec_GPIO_DeInit(void);
static void     Delay(__IO uint32_t nCount);
/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------
                   MAL (Media Access Layer) functions 
  ----------------------------------------------------------------------------*/
/* Peripherals configuration functions */
static void     Audio_MAL_Init(void);
static void     Audio_MAL_DeInit(void);
static void     Audio_MAL_PauseResume(uint32_t Cmd, uint32_t Addr);
static void     Audio_MAL_Stop(void);
/*----------------------------------------------------------------------------*/

/**
  * @brief  Configure the audio peripherals.
  * @param  OutputDevice: OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO .
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{    
  /* Perform low layer Codec initialization */
  if (Codec_Init(OutputDevice, VOLUME_CONVERT(Volume), AudioFreq) != 0)
  {
    return 1;                
  }
  else
  {    
    /* I2S data transfer preparation:
    Prepare the Media to be used for the audio transfer from memory to I2S peripheral */
    Audio_MAL_Init();
    
    /* Return 0 when all operations are OK */
    return 0;
  }
}

/**
  * @brief  Deinitializes all the resources used by the codec (those initialized
  *         by EVAL_AUDIO_Init() function). 
  * @param  None
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_DeInit(void)
{ 
  /* DeInitialize Codec */  
  Codec_DeInit();  
  
  /* DeInitialize the Media layer */
  Audio_MAL_DeInit();
  
  return 0;
}

/**
  * @brief  Starts playing audio stream from a data buffer for a determined size. 
  * @param  pBuffer: Pointer to the buffer 
  * @param  Size: Number of audio data BYTES.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_Play(uint16_t* pBuffer, uint32_t Size)
{
  /* Set the total number of data to be played (count in half-word) */
  AudioTotalSize = Size/2;

  /* Call the audio Codec Play function */
  Codec_Play();
  
  /* Update the Media layer and enable it for play */  
  Audio_MAL_Play((uint32_t)pBuffer, (uint32_t)(DMA_MAX(AudioTotalSize / 2)));
  
  /* Update the remaining number of data to be played */
  AudioRemSize = (Size/2) - DMA_MAX(AudioTotalSize);
  
  /* Update the current audio pointer position */
  CurrentPos = pBuffer + DMA_MAX(AudioTotalSize);
  
  return 0;
}

/**
  * @brief  This function Pauses or Resumes the audio file stream. In case
  *         of using DMA, the DMA Pause feature is used. In all cases the I2S 
  *         peripheral is disabled. 
  * 
  * @WARNING When calling EVAL_AUDIO_PauseResume() function for pause, only
  *          this function should be called for resume (use of EVAL_AUDIO_Play() 
  *          function for resume could lead to unexpected behavior).
  * 
  * @param  Cmd: AUDIO_PAUSE (or 0) to pause, AUDIO_RESUME (or any value different
  *         from 0) to resume. 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_PauseResume(uint32_t Cmd)
{    
  /* Call the Audio Codec Pause/Resume function */
  if (Codec_PauseResume(Cmd) != 0)
  {
    return 1;
  }
  else
  {
    /* Call the Media layer pause/resume function */
    Audio_MAL_PauseResume(Cmd, 0);
    
    /* Return 0 if all operations are OK */
    return 0;
  }
}

/**
  * @brief  Stops audio playing and Power down the Audio Codec. 
  * @param  Option: could be one of the following parameters 
  *           - CODEC_PDWN_SW: for software power off (by writing registers). 
  *                            Then no need to reconfigure the Codec after power on.
  *           - CODEC_PDWN_HW: completely shut down the codec (physically). 
  *                            Then need to reconfigure the Codec after power on.  
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_Stop(uint32_t Option)
{
  /* Call Audio Codec Stop function */
  if (Codec_Stop(Option) != 0)
  {
    return 1;
  }
  else
  {
    /* Call Media layer Stop function */
    Audio_MAL_Stop();
    
    /* Update the remaining data number */
    AudioRemSize = AudioTotalSize;    
    
    /* Return 0 when all operations are correctly done */
    return 0;
  }
}

/**
  * @brief  Controls the current audio volume level. 
  * @param  Volume: Volume level to be set in percentage from 0% to 100% (0 for 
  *         Mute and 100 for Max volume level).
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_VolumeCtl(uint8_t Volume)
{
  /* Call the codec volume control function with converted volume value */
  return (Codec_VolumeCtrl(VOLUME_CONVERT(Volume)));
}

/**
  * @brief  Enables or disables the MUTE mode by software 
  * @param  Command: could be AUDIO_MUTE_ON to mute sound or AUDIO_MUTE_OFF to 
  *         unmute the codec and restore previous volume level.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t EVAL_AUDIO_Mute(uint32_t Cmd)
{ 
  /* Call the Codec Mute function */
  return (Codec_Mute(Cmd));
}

/**
  * @brief  This function handles main Media layer interrupt. 
  * @param  None
  * @retval 0 if correct communication, else wrong communication
  */
void Audio_MAL_IRQHandler(void)
{    
#ifndef AUDIO_MAL_MODE_NORMAL
  uint16_t *pAddr = (uint16_t *)CurrentPos;
  uint32_t Size = AudioRemSize;
#endif /* AUDIO_MAL_MODE_NORMAL */
  
#ifdef AUDIO_MAL_DMA_IT_TC_EN
  /* Transfer complete interrupt */
  if (DMA_GetFlagStatus(AUDIO_MAL_DMA_FLAG_TC) != RESET)
  {     
 #ifdef AUDIO_MAL_MODE_NORMAL
    /* Check if the end of file has been reached */
    if (AudioRemSize > 0)
    {      
      /* Wait the DMA Channel to be effectively disabled */
      while (DMA_GetFlagStatus(AUDIO_MAL_DMA_FLAG_TC) != SET)
      {}
      
      /* Clear the Interrupt flag */
      DMA_ClearFlag(AUDIO_MAL_DMA_FLAG_TC);  
      
      /* Re-Configure the buffer address and size */
      DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t) CurrentPos;
      DMA_InitStructure.DMA_BufferSize = (uint32_t) (DMA_MAX(AudioRemSize));
      
      /* Configure the DMA Channel with the new parameters */
      DMA_Init(AUDIO_MAL_DMA_CHANNEL , &DMA_InitStructure);
      
      /* Enable the I2S DMA Channel */
      DMA_Cmd(AUDIO_MAL_DMA_CHANNEL , ENABLE);    
      
      /* Update the current pointer position */
      CurrentPos += DMA_MAX(AudioRemSize);        
      
      /* Update the remaining number of data to be played */
      AudioRemSize -= DMA_MAX(AudioRemSize);    
    }
    else
    {
      /* Disable the I2S DMA Channel */
      DMA_Cmd(AUDIO_MAL_DMA_CHANNEL , DISABLE);   
      
      /* Clear the Interrupt flag */
      DMA_ClearFlag(AUDIO_MAL_DMA_FLAG_TC);       
      
      /* Manage the remaining file size and new address offset: This function 
      should be coded by user (its prototype is already declared in stm32_eval_audio_codec.h) */  
      EVAL_AUDIO_TransferComplete_CallBack((uint32_t)CurrentPos, 0);       
    }
    
 #elif defined(AUDIO_MAL_MODE_CIRCULAR)
    /* Manage the remaining file size and new address offset: This function 
       should be coded by user (its prototype is already declared in stm32_eval_audio_codec.h) */  
    EVAL_AUDIO_TransferComplete_CallBack(pAddr, Size);    
    
    /* Clear the Interrupt flag */
    DMA_ClearFlag(AUDIO_MAL_DMA_STREAM, AUDIO_MAL_DMA_FLAG_TC);
 #endif /* AUDIO_MAL_MODE_NORMAL */  
  }
#endif /* AUDIO_MAL_DMA_IT_TC_EN */

#ifdef AUDIO_MAL_DMA_IT_HT_EN  
  /* Half Transfer complete interrupt */
  if (DMA_GetFlagStatus(AUDIO_MAL_DMA_CHANNEL, AUDIO_MAL_DMA_FLAG_HT) != RESET)
  {
    /* Manage the remaining file size and new address offset: This function 
       should be coded by user (its prototype is already declared in stm32_eval_audio_codec.h) */  
    EVAL_AUDIO_HalfTransfer_CallBack((uint32_t)pAddr, Size);    
   
    /* Clear the Interrupt flag */
    DMA_ClearFlag(AUDIO_MAL_DMA_CHANNEL, AUDIO_MAL_DMA_FLAG_HT);    
  }
#endif /* AUDIO_MAL_DMA_IT_HT_EN */
  
#ifdef AUDIO_MAL_DMA_IT_TE_EN  
  /* FIFO Error interrupt */
  if ((DMA_GetFlagStatus(AUDIO_MAL_DMA_CHANNEL, AUDIO_MAL_DMA_FLAG_TE) != RESET) || \
     (DMA_GetFlagStatus(AUDIO_MAL_DMA_CHANNEL, AUDIO_MAL_DMA_FLAG_FE) != RESET) || \
     (DMA_GetFlagStatus(AUDIO_MAL_DMA_CHANNEL, AUDIO_MAL_DMA_FLAG_DME) != RESET))
    
  {
    /* Manage the error generated on DMA FIFO: This function 
       should be coded by user (its prototype is already declared in stm32_eval_audio_codec.h) */  
    EVAL_AUDIO_Error_CallBack((uint32_t*)&pAddr);    
    
    /* Clear the Interrupt flag */
    DMA_ClearFlag(AUDIO_MAL_DMA_CHANNEL, AUDIO_MAL_DMA_FLAG_TE | AUDIO_MAL_DMA_FLAG_FE | \
                                        AUDIO_MAL_DMA_FLAG_DME);
  }  
#endif /* AUDIO_MAL_DMA_IT_TE_EN */
}

/*============================================================================
                CS42L52 Audio Codec Control Functions
  ============================================================================*/
/**
  * @brief  Initializes the audio codec and all related interfaces (control 
  *         interface: I2C and audio interface: I2S)
  * @param  OutputDevice: can be OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO .
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_Init(uint16_t OutputDevice, uint8_t Volume, uint32_t AudioFreq)
{
  uint32_t counter = 0; 

  /* Configure the Codec related IOs */
  Codec_GPIO_Init();   

  /* Initialize the Control interface of the Audio Codec */
  Codec_CtrlInterface_Init();     
  
  /* Keep Codec powered OFF */
  counter += Codec_WriteRegister(0x02, 0x9E/*0x01*/);  

  switch (OutputDevice)
  {
    case OUTPUT_DEVICE_SPEAKER:
      counter += Codec_WriteRegister(0x04, 0xFA); /* SPK always ON & HP always OFF */
      OutputDev = 0xFA;
      break;
      
    case OUTPUT_DEVICE_HEADPHONE:
      counter += Codec_WriteRegister(0x04, 0xAF); /* SPK always OFF & HP always ON */
      OutputDev = 0xAF;
      break;
      
    case OUTPUT_DEVICE_BOTH:
      counter += Codec_WriteRegister(0x04, 0xAA); /* SPK always ON & HP always ON */
      OutputDev = 0xAA;
      break;
      
    case OUTPUT_DEVICE_AUTO:
      counter += Codec_WriteRegister(0x04, 0x05); /* Detect the HP or the SPK automatically */
      OutputDev = 0x05;
      break;    
      
    default:
      counter += Codec_WriteRegister(0x04, 0x05); /* Detect the HP or the SPK automatically */
      OutputDev = 0x05;
      break;    
  }
  
  /* Clock configuration: Auto detection */  
  counter += Codec_WriteRegister(0x05, 0x80);
  /* Set the Slave Mode and the audio Standard */  
  counter += Codec_WriteRegister(0x06, 0x03/*CODEC_STANDARD*/);  
  /* Interface Control 2: SCLK is Re-timed signal from MCLK*/
  counter +=Codec_WriteRegister(0x07, 0x00); 
  /* ADCA and PGAA Select: no input selected*/
  counter +=Codec_WriteRegister(0x08, 0x00);
  /* ADCB and PGAB Select: no input selected*/
  counter +=Codec_WriteRegister(0x09, 0x00); 
  /*Play Back Control 1: headphone gain is 0.4, PCM not inverted, Master not mute*/
  counter +=Codec_WriteRegister(0x0D, 0x10);/* CS42L52 has different config than CS42L52*/ 
  /* Miscellaneous Controls: Passthrough Analog & Passthrough Mute off, Soft Ramp on @0x0E*/
  counter +=Codec_WriteRegister(0x0E, 0x02);  
  /* Play Back Control 2: Headphone Mute off, speaker mute off, mono enabled */
  counter +=Codec_WriteRegister(0x0F, 0x32); 
  /* PCM A Volume: PCM Mute disabled, Volume is 0db(default) */
  counter +=Codec_WriteRegister(0x1A, 0x00);
  /* PCM B Volume: PCM Mute disabled, Volume is 0db(default) */
  counter +=Codec_WriteRegister(0x1B, 0x00); 
  /* Headphone A Volume: Headphone Volume is -6db */
  counter +=Codec_WriteRegister(0x22, (u8)(0-12));
  /* Headphone B Volume: Headphone Volume is -6db */
  counter +=Codec_WriteRegister(0x23, (u8)(0-12));    
  /* Speaker A Volume: Speaker Volume is 0db (default) */
  counter +=Codec_WriteRegister(0x24, 0x00);
  /* Speaker B Volume: Speaker Volume is 0db (default) */
  counter +=Codec_WriteRegister(0x25, 0x00);
  /* Charge Pump Frequency: 5 (default) */
  counter +=Codec_WriteRegister(0x34, 5<<4);
  /* Power Control 1: power up */
  counter += Codec_WriteRegister(0x02, 0x00); 
  counter += Codec_WriteRegister(0x20, 0xff); 
  counter += Codec_WriteRegister(0x21, 0xff); 

  /* Configure the I2S peripheral */
  Codec_AudioInterface_Init(AudioFreq);  
  
  /* Return communication control value */
  return counter;  
}

/**
  * @brief  Initializes the audio codec and all related interfaces (control 
  *         interface: I2C and audio interface: I2S)
  * @param  OutputDevice: can be OUTPUT_DEVICE_SPEAKER, OUTPUT_DEVICE_HEADPHONE,
  *                       OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO .
  * @param  Volume: Initial volume level (from 0 (Mute) to 100 (Max))
  * @param  AudioFreq: Audio frequency used to play the audio stream.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t Codec_Record_Init(void)
{
  uint32_t counter = 0; 

  /* Configure the Codec related IOs */
  Codec_GPIO_Recorder_Init();   
  
  /* Initialize the Control interface of the Audio Codec */
  Codec_CtrlInterface_Init();     
  
  /* Mono Record @ MIC1+ with Bias */
  /* Power Control 2[@03h]: MICB Off, MIC A & Bias On */
  counter += Codec_WriteRegister(0x03, 0x04);
  /* Power Control 3[@04h]: headphone A/B Off, speaker A & B Off */
  counter += Codec_WriteRegister(0x04, 0xFF);
  /* Clocking Control: auto sample rate(Fs) */
  counter += Codec_WriteRegister(0x05, 0x80);
  /* Interface Control 1: slave, LeftJustified, 16-bit data */
  counter += Codec_WriteRegister(0x06, 0x03);
  /* Interface Control 2: SCLK is Re-timed signal from MCLK, BIAS: 0.5*VA */
  counter += Codec_WriteRegister(0x07, 0x00);
  /* ADCA and PGAA Select: MICx selected */
  counter += Codec_WriteRegister(0x08, 0x90);
  /* ADCB and PGAB Select: MICx selected */
  counter += Codec_WriteRegister(0x09, 0x90);
  /* Misc ADC Control: SDOUT Signal Source is ADC_or_DSP */
  counter += Codec_WriteRegister(0x0C, 0x00);
  /* Play Back Control 1: headphone gain is 0.4, PCM not inverted, Master not mute */
  counter += Codec_WriteRegister(0x0D, 0x00);
  /* Miscellaneous Controls: Passthrough Analog & Passthrough Mute off, Soft Ramp on @0x0E */
  counter += Codec_WriteRegister(0x0E, 0x02);
  /* Play Back Control 2: Headphone Mute on, speaker mute off, mono enabled */
  counter += Codec_WriteRegister(0x0F, 0xC2);
  /* MICA Amp Control: select MIC 2x, Single-Ended, Gain=16db */
  counter += Codec_WriteRegister(0x10, 0x2E);
  /* MICB Amp Control: select MIC 2x, Single-Ended, Gain=16db */
  counter += Codec_WriteRegister(0x11, 0x40);
  /* MICB Amp Control: try with Differential, Gain=16db */
  counter += Codec_WriteRegister(0x11, 0x20);
  /* PGA Volume ALC Control: Gain=0db */
  counter += Codec_WriteRegister(0x12, 0x00);
  /* PGA Volume ALC Control: Gain=0db */
  counter += Codec_WriteRegister(0x13, 0x00); 
  counter += Codec_WriteRegister(0x34, 5<<4);
  
  counter += Codec_WriteRegister(0x02, 0x14);
  
  /* Return communication control value */
  return counter;  
    
}

/**
  * @brief  Restore the audio codec state to default state and free all used 
  *         resources.
  * @param  None
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_DeInit(void)
{
  uint32_t counter = 0; 
  uint16_t i = 0;
  uint8_t tmp = 0; 
  
  /* Enable Master Playback Mute @0x0D */
  counter += Codec_WriteRegister(0x02, 0x03);
  /* Power down CS42L52 @0x03 */
  counter += Codec_WriteRegister(0x03, 0x07);
  /* Power down CS42L52 @0x04 */
  counter += Codec_WriteRegister(0x04, 0xFF);
  /* config interface @0x06 */
  counter += Codec_WriteRegister(0x06, 0x03);  
  /* Disable Analog (Soft Ramp & Zero Cross) and HighPassFilter @0x0A */
  counter += Codec_WriteRegister(0x0A, 0x00);
  /* Digital Soft Ramp on & Zero Cross off @0x0E */
  counter += Codec_WriteRegister(0x0E, 0x02);
  /* Headphone Mute, Speaker Mute @0x0F */
  counter += Codec_WriteRegister(0x0F, 0xF2);  
  /* PCM A Volume: Mute @0x1A */
  counter += Codec_WriteRegister(0x1A, 0x80);
  /* PCM B Volume: Mute @0x1B */
  counter += Codec_WriteRegister(0x1B, 0x80);
  /* Limiter Attack Rate: 0x00 @0x29 */
  counter += Codec_WriteRegister(0x29, 0x00);
  
  i = 0;
  do{
  // Power down CS42L52 @0x02
  counter += Codec_WriteRegister(0x02, 0x9F);
  tmp = Codec_ReadRegister(0x02);
  i++;
  }
  while (((tmp & 0x01) == 0) && (i < 10));
   
  Delay(CODEC_RESET_DELAY);
  
  /* Deinitialize all use GPIOs */
  Codec_GPIO_DeInit();
  
  /* Deinitialize the Codec audio interface (I2S) */
  Codec_AudioInterface_DeInit(); 
  /* Disable the Codec control interface */
  Codec_CtrlInterface_DeInit();
  
  /* Return communication control value */
  return counter;  
}

/**
  * @brief  Start the audio Codec play feature.
  * @note   For this codec no Play options are required.
  * @param  None
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_Play(void)
{
  /* 
     No actions required on Codec level for play command
     */  

  /* Return communication control value */
  return 0;  
}

/**
  * @brief  Pauses and resumes playing on the audio codec.
  * @param  Cmd: AUDIO_PAUSE (or 0) to pause, AUDIO_RESUME (or any value different
  *         from 0) to resume. 
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_PauseResume(uint32_t Cmd)
{
  uint32_t counter = 0;   
  
  /* Pause the audio file playing */
  if (Cmd == AUDIO_PAUSE)
  { 
    /* Mute the output first */
    counter += Codec_Mute(AUDIO_MUTE_ON);

    /* Put the Codec in Power save mode */    
    counter += Codec_WriteRegister(0x02, 0x01); 
  }
  else /* AUDIO_RESUME */
  {
    /* Unmute the output first */
    counter += Codec_Mute(AUDIO_MUTE_OFF);
    
    counter += Codec_WriteRegister(0x04, OutputDev);
    
    /* Exit the Power save mode */
    counter += Codec_WriteRegister(0x02, 0x9E); 

  }

  return counter;
}

/**
  * @brief  Stops audio Codec playing. It powers down the codec.
  * @param  CodecPdwnMode: selects the  power down mode.
  *          - CODEC_PDWN_SW: only mutes the audio codec. When resuming from this 
  *                           mode the codec keeps the previous initialization
  *                           (no need to re-Initialize the codec registers).
  *          - CODEC_PDWN_HW: Physically power down the codec. When resuming from this
  *                           mode, the codec is set to default configuration 
  *                           (user should re-Initialize the codec in order to 
  *                            play again the audio stream).
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_Stop(uint32_t CodecPdwnMode)
{
  uint32_t counter = 0;   

  /* Mute the output first */
  Codec_Mute(AUDIO_MUTE_ON);
  
  if (CodecPdwnMode == CODEC_PDWN_SW)
  {    
    /* Power down the DAC and the speaker (PMDAC and PMSPK bits)*/
    counter += Codec_WriteRegister(0x02, 0x9F);
  }
  else /* CODEC_PDWN_HW */
  { 
    /* Power down the components */
    counter += Codec_WriteRegister(0x02, 0x9F);

    /* Wait at least 100us */
    Delay(0xFFF);
  }  
  return counter;    
}

/**
  * @brief  Sets higher or lower the codec volume level.
  * @param  Volume: a byte value from 0 to 255 (refer to codec registers 
  *         description for more details).
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_VolumeCtrl(uint8_t Volume)
{
  uint32_t counter = 0;
  
  if (Volume > 0xE6)
  {
    /* Set the Master volume */
    counter += Codec_WriteRegister(0x20, Volume - 0xE7); 
    counter += Codec_WriteRegister(0x21, Volume - 0xE7);     
  }
  else
  {
    /* Set the Master volume */
    counter += Codec_WriteRegister(0x20, Volume + 0x19); 
    counter += Codec_WriteRegister(0x21, Volume + 0x19); 
  }

  return counter;  
}

/**
  * @brief  Enables or disables the mute feature on the audio codec.
  * @param  Cmd: AUDIO_MUTE_ON to enable the mute or AUDIO_MUTE_OFF to disable the
  *             mute mode.
  * @retval 0 if correct communication, else wrong communication
  */
static uint32_t Codec_Mute(uint32_t Cmd)
{
  uint32_t counter = 0;  
  
  /* Set the Mute mode */
  if (Cmd == AUDIO_MUTE_ON)
  {
    counter += Codec_WriteRegister(0x04, 0xFF);
    counter += Codec_WriteRegister(0x0F, 0xF0);
  }
  else /* AUDIO_MUTE_OFF Disable the Mute */
  {
    counter += Codec_WriteRegister(0x04, 0x05);
    counter += Codec_WriteRegister(0x0F, 0x02);
  }
  
  return counter; 
}

/**
  * @brief  Switch dynamically (while audio file is played) the output target 
  *         (speaker or headphone).
  * @note   This function modifies a global variable of the audio codec driver: OutputDev.
  * @param  Output: specifies the audio output target: OUTPUT_DEVICE_SPEAKER,
  *         OUTPUT_DEVICE_HEADPHONE, OUTPUT_DEVICE_BOTH or OUTPUT_DEVICE_AUTO 
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t Codec_SwitchOutput(uint8_t Output)
{
  uint8_t counter = 0;
  
  switch (Output) 
  {
    case OUTPUT_DEVICE_SPEAKER:
      counter += Codec_WriteRegister(0x04, 0xFA); /* SPK always ON & HP always OFF */
      OutputDev = 0xFA;
      break;
      
    case OUTPUT_DEVICE_HEADPHONE:
      counter += Codec_WriteRegister(0x04, 0xAF); /* SPK always OFF & HP always ON */
      OutputDev = 0xAF;
      break;
      
    case OUTPUT_DEVICE_BOTH:
      counter += Codec_WriteRegister(0x04, 0xAA); /* SPK always ON & HP always ON */
      OutputDev = 0xAA;
      break;
      
    case OUTPUT_DEVICE_AUTO:
      counter += Codec_WriteRegister(0x04, 0x05); /* Detect the HP or the SPK automatically */
      OutputDev = 0x05;
      break;    
      
    default:
      counter += Codec_WriteRegister(0x04, 0x05); /* Detect the HP or the SPK automatically */
      OutputDev = 0x05;
      break;
  }
  
  return counter;
}

/**
  * @brief  Writes a Byte to a given register into the audio codec through the 
            control interface (I2C)
  * @param  RegisterAddr: The address (location) of the register to be written.
  * @param  RegisterValue: the Byte value to be written into destination register.
  * @retval 0 if correct communication, else wrong communication
  */
uint32_t Codec_WriteRegister(uint8_t RegisterAddr, uint8_t RegisterValue)
{
  uint32_t result = 0;
  
  /* Test on BUSY Flag */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_ISR_BUSY) != RESET) 
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(CODEC_I2C, CODEC_ADDRESS, 2, I2C_AutoEnd_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  CODECTimeout = CODEC_FLAG_TIMEOUT;  
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Send Register address */
  I2C_SendData(CODEC_I2C, (uint8_t)RegisterAddr );
    
  /* Wait until TXIS flag is set */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }  
  
  /* Write data to TXDR */
  I2C_SendData(CODEC_I2C, (uint8_t)RegisterValue);
  
  /* Wait until STOPF flag is set */
  CODECTimeout = CODEC_FLAG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_ISR_STOPF) == RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }   
  
  /* Clear STOPF flag */
  I2C_ClearFlag(CODEC_I2C, I2C_ICR_STOPCF);

#ifdef VERIFY_WRITTENDATA
  /* Verify that the data has been correctly written */  
  result = (Codec_ReadRegister(RegisterAddr) == RegisterValue)? 0:1;
#endif /* VERIFY_WRITTENDATA */

  /* Return the verifying value: 0 (Passed) or 1 (Failed) */
  return result;
}

/**
  * @brief  Reads and returns the value of an audio codec register through the
  *         control interface (I2C).
  * @param  RegisterAddr: Address of the register to be read.
  * @retval Value of the register to be read or dummy value if the communication
  *         fails.
  */
uint32_t Codec_ReadRegister(uint8_t RegisterAddr)
{
   uint32_t result = 0;
  
  /* Test on BUSY Flag */
  CODECTimeout = CODEC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_ISR_BUSY) != RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }

  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(CODEC_I2C, CODEC_ADDRESS, 1, I2C_SoftEnd_Mode, I2C_Generate_Start_Write);
  
  /* Wait until TXIS flag is set */
  CODECTimeout = CODEC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_ISR_TXIS) == RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Send Register address */
  I2C_SendData(CODEC_I2C, (uint8_t)RegisterAddr);
  
  /* Wait until TC flag is set */
  CODECTimeout = CODEC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_ISR_TC) == RESET)
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }  
  
  /* Configure slave address, nbytes, reload, end mode and start or stop generation */
  I2C_TransferHandling(CODEC_I2C, CODEC_ADDRESS, 1, I2C_AutoEnd_Mode, I2C_Generate_Start_Read);
  
  /* Wait until RXNE flag is set */
  CODECTimeout = CODEC_LONG_TIMEOUT;  
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_ISR_RXNE) == RESET)  
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Read data from RXDR */
  result = I2C_ReceiveData(CODEC_I2C);  
  
  /* Wait until STOPF flag is set */
  CODECTimeout = CODEC_LONG_TIMEOUT;
  while(I2C_GetFlagStatus(CODEC_I2C, I2C_ISR_STOPF) == RESET) 
  {
    if((CODECTimeout--) == 0) return Codec_TIMEOUT_UserCallback();
  }
  
  /* Clear STOPF flag */
  I2C_ClearFlag(CODEC_I2C, I2C_ICR_STOPCF);

  /* Return the byte read from Codec */
  return result;
}

/**
  * @brief  Initializes the Audio Codec control interface (I2C).
  * @param  None
  * @retval None
  */
static void Codec_CtrlInterface_Init(void)
{
  I2C_InitTypeDef I2C_InitStructure;
  
  /* Configure the I2C clock source. The clock is derived from the SYSCLK */
  RCC_I2CCLKConfig(RCC_I2C2CLK_SYSCLK);
  
  /* Enable the CODEC_I2C peripheral clock */
  RCC_APB1PeriphClockCmd(CODEC_I2C_CLK, ENABLE);
  
  /* CODEC_I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_Timing = CODEC_I2C_TIMING;
  
  /* Enable the I2C peripheral */  
  I2C_Init(CODEC_I2C, &I2C_InitStructure);
  I2C_Cmd(CODEC_I2C, ENABLE);
   
}

/**
  * @brief  Restore the Audio Codec control interface to its default state.
  *         This function doesn't de-initialize the I2C because the I2C peripheral
  *         may be used by other modules.
  * @param  None
  * @retval None
  */
static void Codec_CtrlInterface_DeInit(void)
{
  /* Disable the I2C peripheral */ /* This step is not done here because 
     the I2C interface can be used by other modules */
   /*I2C_DeInit(CODEC_I2C); */
}

/**
  * @brief  Initializes the Audio Codec audio interface (I2S)
  * @note   This function assumes that the I2S input clock (through PLL_R in 
  *         Devices RevA/Z and through dedicated PLLI2S_R in Devices RevB/Y)
  *         is already configured and ready to be used.    
  * @param  AudioFreq: Audio frequency to be configured for the I2S peripheral. 
  * @retval None
  */
static void Codec_AudioInterface_Init(uint32_t AudioFreq)
{
  I2S_InitTypeDef I2S_InitStructure;

  /* Enable the CODEC_I2S peripheral clock */
  RCC_APB1PeriphClockCmd(CODEC_I2S_CLK, ENABLE);
  
  /* Deinitialize SPI3_I2S3 peripheral */
  SPI_I2S_DeInit(CODEC_I2S);
   
  /* CODEC_I2S peripheral configuration */
  SPI_I2S_DeInit(CODEC_I2S);
  I2S_InitStructure.I2S_AudioFreq = AudioFreq;
  I2S_InitStructure.I2S_Standard = I2S_Standard_MSB;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
#ifdef CODEC_MCLK_ENABLED
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
#elif defined(CODEC_MCLK_DISABLED)
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Disable;
#else
 #error "No selection for the MCLK output has been defined !"
#endif /* CODEC_MCLK_ENABLED */

  /* Initialize the I2S peripheral with the structure above */
  I2S_Init(CODEC_I2S, &I2S_InitStructure);

  /* The I2S peripheral will be enabled only in the EVAL_AUDIO_Play() function 
       or by user functions if DMA mode not enabled */ 
}

/**
  * @brief  Restores the Audio Codec audio interface to its default state.
  * @param  None
  * @retval None
  */
static void Codec_AudioInterface_DeInit(void)
{
  /* Disable the CODEC_I2S peripheral (in case it hasn't already been disabled) */
  I2S_Cmd(CODEC_I2S, DISABLE);
  
  /* Deinitialize the CODEC_I2S peripheral */
  SPI_I2S_DeInit(CODEC_I2S);
  
  /* Disable the CODEC_I2S peripheral clock */
  RCC_APB1PeriphClockCmd(CODEC_I2S_CLK, DISABLE); 
}

/**
  * @brief Initializes IOs used by the Audio Codec (on the control and audio 
  *        interfaces).
  * @param  None
  * @retval None
  */
static void Codec_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef  NVIC_InitStructure;
  I2S_InitTypeDef I2S_InitStructure;
  
  /* Enable I2S and I2C GPIO clocks */
  RCC_AHBPeriphClockCmd(CODEC_I2C_GPIO_CLOCK | CODEC_I2S_GPIO_CLOCK, ENABLE);
  
  /* Connect pins to I2C peripheral */
  GPIO_PinAFConfig(CODEC_I2C_SCL_GPIO, CODEC_I2S_SCL_PINSRC, CODEC_I2C_GPIO_AF);
  GPIO_PinAFConfig(CODEC_I2C_SDA_GPIO, CODEC_I2S_SDA_PINSRC, CODEC_I2C_GPIO_AF);
  
  /* CODEC_I2C SCL and SDA pins configuration ----------------------------------*/
  GPIO_InitStructure.GPIO_Pin = CODEC_I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CODEC_I2C_SDA_GPIO, &GPIO_InitStructure);     
  
  GPIO_InitStructure.GPIO_Pin = CODEC_I2C_SCL_PIN ;
  GPIO_Init(CODEC_I2C_SCL_GPIO, &GPIO_InitStructure); 
     
  /* Connect pins to I2S peripheral  */
  GPIO_PinAFConfig(CODEC_I2S_MCK_WS_GPIO, CODEC_I2S_WS_PINSRC, CODEC_I2S_GPIO_AF);
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_SCK_PINSRC, CODEC_I2S_GPIO_AF);
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_DIN_PINSRC, CODEC_I2S_GPIO_AF);
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_DOUT_PINSRC, CODEC_I2S_GPIO_AF);
  
  /* CODEC_I2S pins configuration: WS, SCK, DIN and DOUT pins -------------------------*/
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_SCK_PIN | CODEC_I2S_DIN_PIN | CODEC_I2S_DOUT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(CODEC_I2S_GPIO, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_WS_PIN;
  GPIO_Init(CODEC_I2S_MCK_WS_GPIO, &GPIO_InitStructure);

 #ifdef CODEC_MCLK_ENABLED
  /* CODEC_I2S pins configuration: MCK pin */
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_MCK_PIN; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(CODEC_I2S_MCK_WS_GPIO, &GPIO_InitStructure);   
  /* Connect pins to I2S peripheral  */
  GPIO_PinAFConfig(CODEC_I2S_MCK_WS_GPIO, CODEC_I2S_MCK_PINSRC, CODEC_I2S_MCK_GPIO_AF); 
 #endif /* CODEC_MCLK_ENABLED */  
  
  /* SPI3/I2S3 IRQ Channel configuration */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);
  
  NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    
  /* Deinitialize SPI3_I2S3 peripheral */
  SPI_I2S_DeInit(CODEC_I2S);
  /* I2S3 configuration ---------------------------------------//
   SPI3_I2S3 - configured as follow:
        - Work as Master & (transmiter) Rx
        - 16bit data
        - (Disable) Output MCLK
        - Audio sample rate: 11kHz(have to use 8k/24k for CS42L52 ?)
        - Default clock polarity: low level */
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_InitStructure.I2S_Standard = I2S_Standard_MSB;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_32k;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_Init(CODEC_I2S, &I2S_InitStructure);
  /* Disable the I2S3 TXE Interrupt */
  SPI_I2S_ITConfig(CODEC_I2S, SPI_I2S_IT_TXE, DISABLE); 
  /* Enable the SPI3/I2S3 peripheral */
  I2S_Cmd(CODEC_I2S, ENABLE);

}

/**
  * @brief Initializes IOs used by the Audio Codec (on the control and audio 
  *        interfaces).
  * @param  None
  * @retval None
  */
static void Codec_GPIO_Recorder_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
  I2S_InitTypeDef I2S_InitStructure;
  
  /* Enable I2S and I2C GPIO clocks */
  RCC_AHBPeriphClockCmd(CODEC_I2C_GPIO_CLOCK | CODEC_I2S_GPIO_CLOCK, ENABLE);
  
  /* Connect pins to I2C peripheral */
  GPIO_PinAFConfig(CODEC_I2C_SCL_GPIO, CODEC_I2S_SCL_PINSRC, CODEC_I2C_GPIO_AF);
  GPIO_PinAFConfig(CODEC_I2C_SDA_GPIO, CODEC_I2S_SDA_PINSRC, CODEC_I2C_GPIO_AF);
  
  /* CODEC_I2C SCL and SDA pins configuration ----------------------------------*/
  GPIO_InitStructure.GPIO_Pin = CODEC_I2C_SDA_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;
  GPIO_Init(CODEC_I2C_SDA_GPIO, &GPIO_InitStructure);     
  
  GPIO_InitStructure.GPIO_Pin = CODEC_I2C_SCL_PIN ;
  GPIO_Init(CODEC_I2C_SCL_GPIO, &GPIO_InitStructure); 
  
  /* Connect pins to I2S peripheral  */
  GPIO_PinAFConfig(CODEC_I2S_MCK_WS_GPIO, CODEC_I2S_WS_PINSRC, CODEC_I2S_GPIO_AF);
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_SCK_PINSRC, CODEC_I2S_GPIO_AF);
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_DIN_PINSRC, CODEC_I2S_GPIO_AF);
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_DOUT_PINSRC, CODEC_I2S_GPIO_AF);
  
  /* CODEC_I2S pins configuration: WS, SCK, DIN and DOUT pins -------------------------*/
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_SCK_PIN | CODEC_I2S_DIN_PIN | CODEC_I2S_DOUT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(CODEC_I2S_GPIO, &GPIO_InitStructure);
  
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_WS_PIN;
  GPIO_Init(CODEC_I2S_MCK_WS_GPIO, &GPIO_InitStructure);
  
 #ifdef CODEC_MCLK_ENABLED
  /* CODEC_I2S pins configuration: MCK pin */
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_MCK_PIN; 
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(CODEC_I2S_MCK_WS_GPIO, &GPIO_InitStructure);   
  /* Connect pins to I2S peripheral  */
  GPIO_PinAFConfig(CODEC_I2S_MCK_WS_GPIO, CODEC_I2S_MCK_PINSRC, CODEC_I2S_MCK_GPIO_AF); 
 #endif /* CODEC_MCLK_ENABLED */   
  
  
  /* SPI3/I2S3 IRQ Channel configuration */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_0);

  NVIC_InitStructure.NVIC_IRQChannel = SPI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI3, ENABLE);
    
  /* Deinitialize SPI3_I2S3 peripheral */
  SPI_I2S_DeInit(CODEC_I2S);
  /* I2S3 configuration ---------------------------------------//
   SPI3_I2S3 - configured as follow:
        - Work as Master & (transmiter) Rx
        - 16bit data
        - (Disable) Output MCLK
        - Audio sample rate: 11kHz(have to use 8k/24k for CS42L52)
        - Default clock polarity: low level */
  I2S_InitStructure.I2S_Mode = I2S_Mode_MasterTx;
  I2S_InitStructure.I2S_Standard = I2S_Standard_MSB;
  I2S_InitStructure.I2S_DataFormat = I2S_DataFormat_16b;
  I2S_InitStructure.I2S_MCLKOutput = I2S_MCLKOutput_Enable;
  I2S_InitStructure.I2S_AudioFreq = I2S_AudioFreq_8k;
  I2S_InitStructure.I2S_CPOL = I2S_CPOL_Low;
  I2S_Init(CODEC_I2S, &I2S_InitStructure);
  /* configure Full Duplex */
  I2S_FullDuplexConfig(I2S3ext, &I2S_InitStructure);
  /* Disable the I2S3 TXE Interrupt */
  SPI_I2S_ITConfig(CODEC_I2S, SPI_I2S_IT_TXE, DISABLE); 
  /* Enable the SPI3/I2S3 peripheral */
  I2S_Cmd(CODEC_I2S, ENABLE);
  I2S_Cmd(I2S3ext, ENABLE);
}

/**
  * @brief  Restores the IOs used by the Audio Codec interface to their default state.
  * @param  None
  * @retval None
  */
static void Codec_GPIO_DeInit(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  
  /* Deinitialize all the GPIOs used by the driver (EXCEPT the I2C IOs since 
     they are used by the IOExpander as well) */
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_SCK_PIN | CODEC_I2S_DIN_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_2MHz;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
  GPIO_Init(CODEC_I2S_GPIO, &GPIO_InitStructure);     
  
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_WS_PIN;
  GPIO_Init(CODEC_I2S_MCK_WS_GPIO, &GPIO_InitStructure);

#ifdef CODEC_MCLK_ENABLED
  /* CODEC_I2S pins deinitialization: MCK pin */
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_MCK_PIN; 
  GPIO_Init(CODEC_I2S_MCK_WS_GPIO, &GPIO_InitStructure);   
  /* Disconnect pins from I2S peripheral  */
  GPIO_PinAFConfig(CODEC_I2S_MCK_WS_GPIO, CODEC_I2S_MCK_PINSRC, 0x00); 
#endif /* CODEC_MCLK_ENABLED */ 
  
  GPIO_InitStructure.GPIO_Pin = CODEC_I2S_DOUT_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_Init(CODEC_I2S_GPIO, &GPIO_InitStructure);
  
  /* Disconnect pins from I2S peripheral  */
  GPIO_PinAFConfig(CODEC_I2S_MCK_WS_GPIO, CODEC_I2S_WS_PINSRC, 0x00);
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_SCK_PINSRC, 0x00);
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_DIN_PINSRC, 0x00);
  GPIO_PinAFConfig(CODEC_I2S_GPIO, CODEC_I2S_DOUT_PINSRC, 0x00);
}

/**
  * @brief  Inserts a delay time (not accurate timing).
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
void Delay( __IO uint32_t nCount)
{
  for (; nCount != 0; nCount--);
}

#ifdef USE_DEFAULT_TIMEOUT_CALLBACK
/**
  * @brief  Basic management of the timeout situation.
  * @param  None
  * @retval None
  */
uint32_t Codec_TIMEOUT_UserCallback(void)
{
  /* The following code allows I2C error recovery and return to normal communication
     if the error source doesn’t still exist (ie. hardware issue..) */
  I2C_InitTypeDef I2C_InitStructure;

  
  /* LCD_ErrLog("> I2C Timeout error occurred\n"); */  
  I2C_GenerateSTOP(CODEC_I2C, ENABLE);
  /* Generated a Software reset */
  I2C_SoftwareResetCmd(CODEC_I2C);
  
  /* CODEC_I2C configuration */
  I2C_InitStructure.I2C_Mode = I2C_Mode_I2C;
  I2C_InitStructure.I2C_AnalogFilter = I2C_AnalogFilter_Enable;
  I2C_InitStructure.I2C_DigitalFilter = 0x00;
  I2C_InitStructure.I2C_OwnAddress1 = 0x00;
  I2C_InitStructure.I2C_Ack = I2C_Ack_Disable;
  I2C_InitStructure.I2C_AcknowledgedAddress = I2C_AcknowledgedAddress_7bit;
  I2C_InitStructure.I2C_Timing = CODEC_I2C_TIMING;
  
  /* Enable the I2C peripheral */  
  I2C_Init(CODEC_I2C, &I2C_InitStructure);
  I2C_Cmd(CODEC_I2C, ENABLE);

  /* At this stage the I2C error should be recovered and device can communicate
     again (except if the error source still exist).
     User can implement mechanism (ex. test on max trial number) to manage situation
     when the I2C can't recover from current error. */

  /* LCD_UsrLog("> I2C error recovered.\n"); */

  return 0;
}
#endif /* USE_DEFAULT_TIMEOUT_CALLBACK */

/*============================================================================
                Audio MAL Interface Control Functions
  ============================================================================*/

/**
  * @brief  Initializes and prepares the Media to perform audio data transfer 
  *         from Media to the I2S peripheral.
  * @param  None
  * @retval None
  */
static void Audio_MAL_Init(void)  
{   
#if defined(AUDIO_MAL_DMA_IT_TC_EN) || defined(AUDIO_MAL_DMA_IT_HT_EN) || defined(AUDIO_MAL_DMA_IT_TE_EN)
  NVIC_InitTypeDef NVIC_InitStructure;
#endif
  
  /* Enable the DMA clock */
  RCC_AHBPeriphClockCmd(AUDIO_MAL_DMA_CLOCK, ENABLE); 
    
  /* Configure the DMA Channel */
  DMA_Cmd(AUDIO_MAL_DMA_CHANNEL, DISABLE);
  DMA_DeInit(AUDIO_MAL_DMA_CHANNEL);
  /* Set the parameters to be configured */
 
  DMA_InitStructure.DMA_PeripheralBaseAddr = CODEC_I2S_ADDRESS;
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)0;      /* This field will be configured in play function */
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
  DMA_InitStructure.DMA_BufferSize = (uint32_t)0xFFFE;      /* This field will be configured in play function */
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
  DMA_InitStructure.DMA_PeripheralDataSize = AUDIO_MAL_DMA_PERIPH_DATA_SIZE;
  DMA_InitStructure.DMA_MemoryDataSize = AUDIO_MAL_DMA_MEM_DATA_SIZE; 
#ifdef AUDIO_MAL_MODE_NORMAL
  DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
#elif defined(AUDIO_MAL_MODE_CIRCULAR)
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
#else
  #error "AUDIO_MAL_MODE_NORMAL or AUDIO_MAL_MODE_CIRCULAR should be selected !!"
#endif /* AUDIO_MAL_MODE_NORMAL */  
  DMA_Init(AUDIO_MAL_DMA_CHANNEL, &DMA_InitStructure);  

  /* Enable the selected DMA interrupts (selected in "stm32_eval_audio_codec.h" defines) */
#ifdef AUDIO_MAL_DMA_IT_TC_EN
  DMA_ITConfig(AUDIO_MAL_DMA_CHANNEL, DMA_IT_TC, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_TC_EN */
#ifdef AUDIO_MAL_DMA_IT_HT_EN
  DMA_ITConfig(AUDIO_MAL_DMA_CHANNEL, DMA_IT_HT, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_HT_EN */
#ifdef AUDIO_MAL_DMA_IT_TE_EN
  DMA_ITConfig(AUDIO_MAL_DMA_CHANNEL, DMA_IT_TE | DMA_IT_FE | DMA_IT_DME, ENABLE);
#endif /* AUDIO_MAL_DMA_IT_TE_EN */
  
  /* Enable the I2S DMA request */
  SPI_I2S_DMACmd(CODEC_I2S, SPI_I2S_DMAReq_Tx, ENABLE);
  
#if defined(AUDIO_MAL_DMA_IT_TC_EN) || defined(AUDIO_MAL_DMA_IT_HT_EN) || defined(AUDIO_MAL_DMA_IT_TE_EN)
  /* I2S DMA IRQ Channel configuration */
  NVIC_InitStructure.NVIC_IRQChannel = AUDIO_MAL_DMA_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EVAL_AUDIO_IRQ_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = EVAL_AUDIO_IRQ_SUBRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
#endif 
}

/**
  * @brief  Restore default state of the used Media.
  * @param  None
  * @retval None
  */
static void Audio_MAL_DeInit(void)  
{   
#if defined(AUDIO_MAL_DMA_IT_TC_EN) || defined(AUDIO_MAL_DMA_IT_HT_EN) || defined(AUDIO_MAL_DMA_IT_TE_EN)
  NVIC_InitTypeDef NVIC_InitStructure;  
  
  /* Deinitialize the NVIC interrupt for the I2S DMA Channel */
  NVIC_InitStructure.NVIC_IRQChannel = AUDIO_MAL_DMA_IRQ;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EVAL_AUDIO_IRQ_PREPRIO;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = EVAL_AUDIO_IRQ_SUBRIO;
  NVIC_InitStructure.NVIC_IRQChannelCmd = DISABLE;
  NVIC_Init(&NVIC_InitStructure);  
#endif 
  
  /* Disable the DMA before the deinit */
  DMA_Cmd(AUDIO_MAL_DMA_CHANNEL, DISABLE);
  
  /* Dinitialize the DMA Channel */
  DMA_DeInit(AUDIO_MAL_DMA_CHANNEL);
  
  /* The DMA clock is not disabled */
}

/**
  * @brief  Starts playing audio stream from the audio Media.
  * @param  None
  * @retval None
  */
void Audio_MAL_Play(uint32_t Addr, uint32_t Size)
{   
  /* Configure the buffer address and size */
  DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)Addr;
  DMA_InitStructure.DMA_BufferSize = (uint32_t)Size/2;
  
  /* Configure the DMA Channel with the new parameters */
  DMA_Init(AUDIO_MAL_DMA_CHANNEL, &DMA_InitStructure);
  
  /* Enable the I2S DMA Channel */
  DMA_Cmd(AUDIO_MAL_DMA_CHANNEL, ENABLE);
}

/**
  * @brief  Pauses or Resumes the audio stream playing from the Media.
  * @param  Cmd: AUDIO_PAUSE (or 0) to pause, AUDIO_RESUME (or any value different
  *              from 0) to resume. 
  * @param  Addr: Address from/at which the audio stream should resume/pause.
  * @retval None
  */
static void Audio_MAL_PauseResume(uint32_t Cmd, uint32_t Addr)
{
  /* Pause the audio file playing */
  if (Cmd == AUDIO_PAUSE)
  {   
    /* Disable the I2S DMA request */
    SPI_I2S_DMACmd(CODEC_I2S, SPI_I2S_DMAReq_Tx, DISABLE);

    /* Pause the I2S DMA Channel 
        Note. For the STM32F30x devices, the DMA implements a pause feature, 
              by disabling the channel, all configuration is preserved and data 
              transfer is paused till the next enable of the channel.*/
    DMA_Cmd(AUDIO_MAL_DMA_CHANNEL, DISABLE);  
  }
  else /* AUDIO_RESUME */
  {
    /* Enable the I2S DMA request */
    SPI_I2S_DMACmd(CODEC_I2S, SPI_I2S_DMAReq_Tx, ENABLE);
  
    /* Resume the I2S DMA Channel 
        Note. For the STM32F30x devices, the DMA implements a pause feature, 
              by disabling the channel, all configuration is preserved and data 
              transfer is paused till the next enable of the channel.*/
    DMA_Cmd(AUDIO_MAL_DMA_CHANNEL, ENABLE);
    
    /* If the I2S peripheral is still not enabled, enable it */
    if ((CODEC_I2S->I2SCFGR & I2S_ENABLE_MASK) == 0)
    {
      I2S_Cmd(CODEC_I2S, ENABLE);
    }    
  }  
}

/**
  * @brief  Stops audio stream playing on the used Media.
  * @param  None
  * @retval None
  */
static void Audio_MAL_Stop(void)
{   
  /* Stop the Transfer on the I2S side: Stop and disable the DMA channel */
  DMA_Cmd(AUDIO_MAL_DMA_CHANNEL, DISABLE);

  /* Clear all the DMA flags for the next transfer */
  DMA_ClearFlag(AUDIO_MAL_DMA_FLAG_TC|AUDIO_MAL_DMA_FLAG_HT|AUDIO_MAL_DMA_FLAG_TE);
  
  /* The I2S DMA requests are not disabled here. */
  
  /* In all modes, disable the I2S peripheral */
  I2S_Cmd(CODEC_I2S, DISABLE);
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
