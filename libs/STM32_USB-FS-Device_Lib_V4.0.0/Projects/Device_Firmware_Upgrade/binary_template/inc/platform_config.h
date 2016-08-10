/**
  ******************************************************************************
  * @file    platform_config.h
  * @author  MCD Application Team
  * @version V4.0.0
  * @date    21-January-2013
  * @brief   Evaluation board specific configuration file.
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
#ifndef __PLATFORM_CONFIG_H
#define __PLATFORM_CONFIG_H

/* Includes ------------------------------------------------------------------*/

#if defined(STM32L1XX_MD) || defined(STM32L1XX_HD)|| defined(STM32L1XX_MD_PLUS)
 #include "stm32l1xx.h"
 #if defined (USE_STM32L152_EVAL)
  #include "stm32l152_eval.h"
 #elif defined (USE_STM32L152D_EVAL)
  #include "stm32l152d_eval.h"
 #else
  #error "Missing define: USE_STM32L152_EVAL or USE_STM32L152D_EVAL"
 #endif /* USE_STM32L152_EVAL */
#elif defined (STM32F10X_MD) || defined (STM32F10X_HD) || defined (STM32F10X_XL)
 #include "stm32f10x.h"
 #if defined (USE_STM3210B_EVAL)
  #include "stm3210b_eval.h"
 #elif defined (USE_STM3210E_EVAL)
  #include "stm3210e_eval.h"
 #else
  #error "Missing define: USE_STM3210B_EVAL or USE_STM3210E_EVAL"
#endif /* USE_STM3210B_EVAL */
#elif defined (USE_STM32373C_EVAL)
 #include "stm32f37x.h"
 #include "stm32373c_eval.h"
#elif defined (USE_STM32303C_EVAL)
 #include "stm32f30x.h"
 #include "stm32303c_eval.h"
#endif

/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
/* Uncomment the line corresponding to the STMicroelectronics evaluation board
   used to run the example */
#if !defined (USE_STM3210B_EVAL) &&  !defined (USE_STM3210E_EVAL) &&  !defined (USE_STM32L152_EVAL) &&  !defined (USE_STM32L152D_EVAL) && !defined (USE_STM32373C_EVAL) && !defined (USE_STM32303C_EVAL)
 //#define USE_STM3210B_EVAL
 //#define USE_STM3210E_EVAL
 //#define USE_STM32L152_EVAL
//#define USE_STM32L152D_EVAL
//#define USE_STM32373C_EVAL
#define USE_STM32303C_EVAL
#endif


/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

#endif /* __PLATFORM_CONFIG_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
