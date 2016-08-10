/**
  ******************************************************************************
  * @file    stm32l152d_eval_glass_lcd.h
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    09-March-2012
  * @brief   Header file for stm32l152d_eval_glass_lcd.c module.
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
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM32L152D_EVAL_GLASS_LCD_H
#define __STM32L152D_EVAL_GLASS_LCD_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l152d_eval.h"

/** @addtogroup Utilities
  * @{
  */
  
/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM32L152D_EVAL
  * @{
  */

/** @addtogroup STM32L152D_EVAL_GLASS_LCD
  * @{
  */ 

/** @defgroup STM32L152D_EVAL_GLASS_LCD_Exported_Types
  * @{
  */

/**
  * @brief LCD Glass point
  * Warning: element values correspond to LCD Glass point.
  */

typedef enum
{
  POINT_OFF = 0,
  POINT_ON = 1
}Point_Typedef;

/**
  * @brief LCD Glass Double point
  * Warning: element values correspond to LCD Glass Double point.
  */
typedef enum
{
  DOUBLEPOINT_OFF = 0,
  DOUBLEPOINT_ON = 1
}DoublePoint_Typedef;

/**
  * @brief LCD Glass Battery Level
  * Warning: element values correspond to LCD Glass Battery Level.
  */

typedef enum
{
  BATTERYLEVEL_OFF  = 0,
  BATTERYLEVEL_1_4  = 1,
  BATTERYLEVEL_1_2  = 2,
  BATTERYLEVEL_3_4  = 3,
  BATTERYLEVEL_FULL = 4
}BatteryLevel_TypeDef;

/**
  * @brief LCD Glass Temperature Level
  * Warning: element values correspond to LCD Glass Temperature Level.
  */

typedef enum
{
  TEMPERATURELEVEL_OFF = 0,
  TEMPERATURELEVEL_1   = 1,
  TEMPERATURELEVEL_2   = 2,
  TEMPERATURELEVEL_3   = 3,
  TEMPERATURELEVEL_4   = 4,
  TEMPERATURELEVEL_5   = 5,
  TEMPERATURELEVEL_6   = 6
}TemperatureLevel_TypeDef;

/**
  * @brief LCD Glass Arrow Direction
  * Warning: element values correspond to LCD Glass Arrow Direction.
  */

typedef enum
{
  ARROWDIRECTION_OFF   = 0,
  ARROWDIRECTION_UP    = 1,
  ARROWDIRECTION_DOWN  = 2,
  ARROWDIRECTION_LEFT  = 3,
  ARROWDIRECTION_RIGHT = 4
}ArrowDirection_TypeDef;

/**
  * @brief LCD Glass Value Unit
  * Warning: element values correspond to LCD Glass Value Unit.
  */
typedef enum
{
  VALUEUNIT_OFF         = 0,
  VALUEUNIT_MILLIAMPERE = 1,
  VALUEUNIT_MICROAMPERE = 2,
  VALUEUNIT_NANOAMPERE  = 3
}ValueUnit_TypeDef;

/**
  * @brief LCD Glass Sign
  * Warning: element values correspond to LCD Glass Sign.
  */
typedef enum
{
  SIGN_POSITIVE = 0,
  SIGN_NEGATIVE = 1
}Sign_TypeDef;

/**
  * @brief LCD Glass Pixel Row
  * Warning: element values correspond to LCD Glass Pixel Row.
  */
typedef enum
{
  PIXELROW_1 = 1,
  PIXELROW_2 = 2,
  PIXELROW_3 = 3,
  PIXELROW_4 = 4,
  PIXELROW_5 = 5,
  PIXELROW_6 = 6,
  PIXELROW_7 = 7,
  PIXELROW_8 = 8,
  PIXELROW_9 = 9,
  PIXELROW_10 = 10
}PixelRow_TypeDef;

/**
  * @brief LCD Glass Pixel Column
  * Warning: element values correspond to LCD Glass Pixel Column.
  */
typedef enum
{
  PIXELCOLUMN_1 = 1,
  PIXELCOLUMN_2 = 2,
  PIXELCOLUMN_3 = 3,
  PIXELCOLUMN_4 = 4,
  PIXELCOLUMN_5 = 5,
  PIXELCOLUMN_6 = 6,
  PIXELCOLUMN_7 = 7,
  PIXELCOLUMN_8 = 8,
  PIXELCOLUMN_9 = 9,
  PIXELCOLUMN_10 = 10,
  PIXELCOLUMN_11 = 11,
  PIXELCOLUMN_12 = 12,
  PIXELCOLUMN_13 = 13,
  PIXELCOLUMN_14 = 14,
  PIXELCOLUMN_15 = 15,
  PIXELCOLUMN_16 = 16,
  PIXELCOLUMN_17 = 17,
  PIXELCOLUMN_18 = 18,
  PIXELCOLUMN_19 = 19
}PixelColumn_TypeDef;

/**
  * @}
  */ 

/** @defgroup STM32L152D_EVAL_GLASS_LCD_Exported_Constants
  * @{
  */ 

/**
  * @}
  */

/** @defgroup STM32L152D_EVAL_GLASS_LCD_Exported_Macros
  * @{
  */

/**
  * @}
  */ 

/** @defgroup STM32L152D_EVAL_GLASS_LCD_Exported_Functions
  * @{
  */
void LCD_GLASS_Init(void);
void LCD_GLASS_DisplayChar(uint8_t* ch, Point_Typedef point, DoublePoint_Typedef DoublePoint,uint8_t position);
void LCD_GLASS_DisplayString(uint8_t* ptr);
void LCD_GLASS_WriteChar(uint8_t* ch, Point_Typedef point, DoublePoint_Typedef DoublePoint,uint8_t position);
void LCD_GLASS_ClearChar(uint8_t position);
void LCD_GLASS_Clear(void);
void LCD_GLASS_ScrollString(uint8_t* ptr, uint16_t nScroll, uint16_t ScrollSpeed);

void LCD_GLASS_ClearTextZone(void);
void LCD_GLASS_DisplayLogo(FunctionalState NewState);
void LCD_GLASS_BatteryLevelConfig(BatteryLevel_TypeDef BatteryLevel);
void LCD_GLASS_ArrowConfig(ArrowDirection_TypeDef ArrowDirection);
void LCD_GLASS_TemperatureConfig(TemperatureLevel_TypeDef TemperatureLevel);
void LCD_GLASS_ValueUnitConfig(ValueUnit_TypeDef ValueUnit);
void LCD_GLASS_SignCmd(Sign_TypeDef Sign, FunctionalState NewState);

void LCD_GLASS_WriteMatrixPixel(PixelRow_TypeDef PixelRow, PixelColumn_TypeDef PixelColumn);
void LCD_GLASS_ClearMatrixPixel(PixelRow_TypeDef PixelRow, PixelColumn_TypeDef PixelColumn);
/**
  * @}
  */

#ifdef __cplusplus
}
#endif

#endif /* __STM32L152D_EVAL_GLASS_LCD_H */

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


