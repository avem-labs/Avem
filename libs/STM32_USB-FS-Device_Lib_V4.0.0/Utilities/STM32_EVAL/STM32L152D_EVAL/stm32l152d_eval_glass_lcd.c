/**
  ******************************************************************************
  * @file    stm32l152d_eval_glass_lcd.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    09-March-2012
  * @brief   This file includes the LCD Glass driver for LCD XHO5002B Module of 
  *          STM32L152D-EVAL board.
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
#include "stm32l152d_eval_glass_lcd.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM32L152D_EVAL
  * @{
  */
  
/** @defgroup STM32L152D_EVAL_GLASS_LCD 
  * @brief This file includes the LCD Glass driver for LCD_XHO5002B Module of 
  *        STM32L152D-EVAL board.
  * @{
  */ 

/** @defgroup STM32L152D_EVAL_GLASS_LCD_Private_Types
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32L152D_EVAL_GLASS_LCD_Private_Defines
  * @{
  */ 
/**
  * @}
  */   

/** @defgroup STM32L152D_EVAL_GLASS_LCD_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 

/** @defgroup STM32L152D_EVAL_GLASS_LCD_Private_Variables
  * @{
  */ 

/**
  @verbatim
================================================================================
                              GLASS LCD MAPPING
================================================================================
               A
       _  ----------
     P|_| |\   |H  /|
         F| G  |  I |B
          |  \ | /  |
          --M-- --N--
          |   /| \  |
         E|  L |  J |C
       _  | /  |K  \|
     Q|_| -----------
              D

A LCD character coding is based on the following matrix:
             COM0    COM1    COM4    COM5     COM6    COM7
  SEG(n)    { 0 ,     0 ,     I ,     B ,      C ,     J }
  SEG(n+1)  { D ,     K ,     A ,     H ,      M ,     N }
  SEG(n+2)  { Q ,     L ,     G ,     F ,      P ,     E }

The character A for example is:
-----------------------------------------------------------
             COM0    COM1     COM4    COM5    COM6     COM7
  SEG(n)    { 0 ,     0 ,      0 ,     1 ,     1 ,      0 }
  SEG(n+1)  { 0 ,     0 ,      1 ,     0 ,     1 ,      1 }
  SEG(n+2)  { 0 ,     0 ,      0 ,     1 ,     0 ,      1 }
   --------------------------------------------------------
           =  0       0        2       5       3        6 hex

   => 'A' = 0x002536

  @endverbatim
  */

/**
  * @brief LETTERS AND NUMBERS MAPPING DEFINITION
  */
uint16_t i;
__I uint32_t Mask[] =
  {
    0x00F00000, 0x000F0000, 0x0000F000, 0x00000F00, 0x000000F0, 0x0000000F
  };
const uint8_t Shift[6] =
  {
    20, 16, 12, 8, 4, 0
  };

uint32_t Digit[6];     /* Digit frame buffer */

/* Letters and number map of the custom LCD 8x40(STM8L152D-EVAL evaluation board) */
__I uint32_t LetterMap[26] =
  {
    /*  A            B          C           D           E            F   */
    0x00002536, 0x00202536, 0x00202404, 0x00222310, 0x00202426, 0x00002426, 
    /*   G            H          I            J           K         L    */ 
    0x00202416, 0x00000536, 0x00222200, 0x00200114, 0x00001425, 0x00200404,
    /*   M          N           O            P          Q         R      */ 
    0x00005514, 0x00004515, 0x00202514, 0x00002526, 0x00002532, 0x00002527,
    /*   S           T           U           V           W           X   */
    0x00202432, 0x00022200, 0x00200514, 0x00041404, 0x00050515, 0x00045001, 
    /*  Y          Z     */
    0x00025000, 0x00243000
  };

__I uint32_t NumberMap[10] =
  {
    /*   0           1           2          3           4   */
    0x00202514, 0x00000110, 0x00202126, 0x00202132, 0x00000532,
    /*    5         6            7          8           9   */
    0x00202432, 0x00202436, 0x00002110, 0x00202536, 0x00202532
  };

/**
  * @}
  */

/** @defgroup STM32L152D_EVAL_LCD_Private_Function_Prototypes
  * @{
  */
static void Convert(uint8_t* c, Point_Typedef Point, DoublePoint_Typedef DoublePoint);
static void delay(__IO uint32_t nCount);
static void LCD_GPIOConfig(void);

/**
  * @}
  */ 

/** @defgroup STM32L152D_EVAL_LCD_Private_Functions
  * @{
  */ 

/**
  * @brief  Configures the LCD GLASS relative GPIO port IOs and LCD peripheral.
  * @param  None 
  * @retval None
  */
void LCD_GLASS_Init(void)
{
  LCD_InitTypeDef LCD_InitStructure;

  LCD_GPIOConfig(); /*!< Configure the LCD Glass GPIO pins */

  /*!< Configure the LCD interface -------------------------------------------*/
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_LCD, ENABLE); /*!< Enable LCD APB1 Clock */

  LCD_InitStructure.LCD_Prescaler = LCD_Prescaler_4;
  LCD_InitStructure.LCD_Divider = LCD_Divider_16;
  LCD_InitStructure.LCD_Duty = LCD_Duty_1_8;
  LCD_InitStructure.LCD_Bias = LCD_Bias_1_4;
  LCD_InitStructure.LCD_VoltageSource = LCD_VoltageSource_Internal;
  LCD_Init(&LCD_InitStructure);

  /*!< Configure the Pulse On Duration */
  LCD_PulseOnDurationConfig(LCD_PulseOnDuration_2);

  /*!< Configure the LCD Contrast (3.51V) */
  LCD_ContrastConfig(LCD_Contrast_Level_7);

  /*!< Wait Until the LCD FCR register is synchronized */
  LCD_WaitForSynchro();

  /*!< Enable LCD peripheral */
  LCD_Cmd(ENABLE);

  /*!< Wait Until the LCD is enabled */
  while(LCD_GetFlagStatus(LCD_FLAG_ENS) == RESET)
  {
  }
  /*!< Wait Until the LCD Booster is ready */  
  while(LCD_GetFlagStatus(LCD_FLAG_RDY) == RESET)
  {
  }
}

/**
  * @brief  This function writes a char in the LCD RAM.
  * @param  ch: The character to display.
  * @param  point: A point to add in front of char.
  *          This parameter  can be one of the following values:
  *              @arg POINT_OFF : No point to add in front of char.
  *              @arg POINT_ON : Add a point in front of char.
  * @param  apostrophe: Flag indicating if a apostrophe has to be add in front 
  *                     of displayed character.
  *          This parameter  can be one of the following values:
  *              @arg APOSTROPHE_OFF : No apostrophe to add in back of char.
  *              @arg APOSTROPHE_ON : Add an apostrophe in back of char.
  * @param  position: Position in the LCD of the caracter to write.
  *          This parameter can be any value in range [0:7].
  * @retval None
  */
void LCD_GLASS_WriteChar(uint8_t* ch, Point_Typedef point, DoublePoint_Typedef DoublePoint, uint8_t position)
{
  Convert(ch, point, DoublePoint);

  switch (position)
  {
    /* Position 0 on LCD */
    case 0: 
      /* Write Digit 0 on COM0 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFBFF;
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)(Digit[0] << (uint32_t)0x09);
      /* Write Digit 1 on COM1 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFF3FF;
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)(Digit[1] << (uint32_t)0x09);
      /* Write Digit 2 on COM4 */
      LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFFFF1FF;
      LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)(Digit[2] << (uint32_t)0x09);
      /* Write Digit 3 on COM5 */
      LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFFFF1FF;
      LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)(Digit[3] << (uint32_t)0x09);
      /* Write Digit 4 on COM6 */
      LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFFFF9FF;
      LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)(Digit[4] << (uint32_t)0x09);
      /* Write Digit 5 on COM7 */
      LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFFFF1FF;
      LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)(Digit[5] << (uint32_t)0x09);
      break;

    /* Position 1 on LCD */
    case 1:
      /* Write Digit 0 on COM0 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFF9FFF;
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)(Digit[0] << (uint32_t)0x0C);
      /* Write Digit 1 on COM1 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFF9FFF;
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)(Digit[1] << (uint32_t)0x0C);
      /* Write Digit 2 on COM4 */
      LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFFF8FFF;
      LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)(Digit[2] << (uint32_t)0x0C);
      /* Write Digit 3 on COM5 */
      LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFFF8FFF;
      LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)(Digit[3] << (uint32_t)0x0C);
      /* Write Digit 4 on COM6 */
      LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFFF8FFF;
      LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)(Digit[4] << (uint32_t)0x0C);
      /* Write Digit 5 on COM7 */
      LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFFF8FFF;
      LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)(Digit[5] << (uint32_t)0x0C);
      break;

    /* Position 2 on LCD */
    case 2:
      /* Write Digit 0 on COM0 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFCFFFF;
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)(Digit[0] << (uint32_t)0x0F);
      /* Write Digit 1 on COM1 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFCFFFF;
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)(Digit[1] << (uint32_t)0x0F);
      /* Write Digit 2 on COM4 */
      LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFFC7FFF;
      LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)(Digit[2] << (uint32_t)0x0F);
      /* Write Digit 3 on COM5 */
      LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFFC7FFF;
      LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)(Digit[3] << (uint32_t)0x0F);
      /* Write Digit 4 on COM6 */
      LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFFC7FFF;
      LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)(Digit[4] << (uint32_t)0x0F);
      /* Write Digit 5 on COM7 */
      LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFFC7FFF;
      LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)(Digit[5] << (uint32_t)0x0F);
      break;

    /* Position 3 on LCD */
    case 3:
      /* Write Digit 0 on COM0 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFE7FFFF;
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)(Digit[0] << (uint32_t)0x12);
      /* Write Digit 1 on COM1 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFE7FFFF;
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)(Digit[1] << (uint32_t)0x12);
      /* Write Digit 2 on COM4 */
      LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFE3FFFF;
      LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)(Digit[2] << (uint32_t)0x12);
      /* Write Digit 3 on COM5 */
      LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFE3FFFF;
      LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)(Digit[3] << (uint32_t)0x12);
      /* Write Digit 4 on COM6 */
      LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFE3FFFF;
      LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)(Digit[4] << (uint32_t)0x12);
      /* Write Digit 5 on COM7 */
      LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFE3FFFF;
      LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)(Digit[5] << (uint32_t)0x12);
      break;

    /* Position 4 on LCD */
    case 4:
      /* Write Digit 0 on COM0 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFFF9;
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)(Digit[0] );
      /* Write Digit 1 on COM1 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFFF9;
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)(Digit[1]);
      /* Write Digit 2 on COM4 */
      LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFFFFFF8;
      LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)(Digit[2]);
      /* Write Digit 3 on COM5 */
      LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFFFFFF8;
      LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)(Digit[3]);
      /* Write Digit 4 on COM6 */
      LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFFFFFF8;
      LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)(Digit[4]);
      /* Write Digit 5 on COM7 */
      LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFFFFFF8;
      LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)(Digit[5]);
      break;

      /* Position 5 on LCD */
    case 5:
      /* Write Digit 0 on COM0 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFFCF;
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)(Digit[0] << (uint32_t)0x03);
      /* Write Digit 1 on COM1 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFFCF;
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)(Digit[1] << (uint32_t)0x03);
      /* Write Digit 2 on COM4 */
      LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFFFFFC7;
      LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)(Digit[2] << (uint32_t)0x03);
      /* Write Digit 3 on COM5 */
      LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFFFFFC7;
      LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)(Digit[3] << (uint32_t)0x03);
      /* Write Digit 4 on COM6 */
      LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFFFFFC7;
      LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)(Digit[4] << (uint32_t)0x03);
      /* Write Digit 5 on COM7 */
      LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFFFFFC7;
      LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)(Digit[5] << (uint32_t)0x03);
      break;

    /* Position 6 on LCD */
    case 6:
      /* Write Digit 0 on COM0 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFE7F;
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)(Digit[0] << (uint32_t)0x06);
      /* Write Digit 1 on COM1 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFE7F;
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)(Digit[1] << (uint32_t)0x06);
      /* Write Digit 2 on COM4 */
      LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFFFFE3F;
      LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)(Digit[2] << (uint32_t)0x06);
      /* Write Digit 3 on COM5 */
      LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFFFFE3F;
      LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)(Digit[3] << (uint32_t)0x06);
      /* Write Digit 4 on COM6 */
      LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFFFFE3F;
      LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)(Digit[4] << (uint32_t)0x06);
      /* Write Digit 5 on COM7 */
      LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFFFFE3F;
      LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)(Digit[5] << (uint32_t)0x06);
      break;

    default:
      break;
  }
}

/**
  * @brief This function Clear a char in the LCD RAM.
  * @param position: Position in the LCD of the caracter to write.
  *                  This parameter can be any value in range [0:7].
  * @retval None
  */
void LCD_GLASS_ClearChar(uint8_t position)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }
  switch (position)
  {
    /* Clear position 0 on LCD */
    case 0:
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFBFF;
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFF3FF;
      LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFFFF1FF;
      LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFFFF1FF;
      LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFFFF9FF;
      LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFFFF1FF;
      break;

    /* Clear position 1 on LCD */
    case 1:
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFF9FFF;
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFF9FFF;
      LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFFF8FFF;
      LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFFF8FFF;
      LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFFF8FFF;
      LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFFF8FFF;
      break;

    /* Clear position 2 on LCD */
    case 2:
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFCFFFF;
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFCFFFF;
      LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFFC7FFF;
      LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFFC7FFF;
      LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFFC7FFF;
      LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFFC7FFF;
      break;

    /* Clear position 3 on LCD */
    case 3:
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFE7FFFF;
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFE7FFFF;
      LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFE3FFFF;
      LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFE3FFFF;
      LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFE3FFFF;
      LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFE3FFFF;
      break;

    /* Clear position 4 on LCD */
    case 4:
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFFF9;
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFFF9;
      LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFFFFFF8;
      LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFFFFFF8;
      LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFFFFFF8;
      LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFFFFFF8;
      break;

    /* Clear position 5 on LCD */
    case 5:
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFFCF;
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFFCF;
      LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFFFFFC7;
      LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFFFFFC7;
      LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFFFFFC7;
      LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFFFFFC7;
      break;

    /* Clear position 6 on LCD */
    case 6:
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFE7F;
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFE7F;
      LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFFFFE3F;
      LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFFFFE3F;
      LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFFFFE3F;
      LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFFFFE3F;
      break;

    default:
      break;
  }
  /*!< Request LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  This function writes a char in the LCD RAM.
  * @param  ch: The character to display.
  * @param  point: A point to add in front of char.
  *          This parameter  can be one of the following values:  
  *              @arg POINT_OFF: No point to add in front of char.
  *              @arg POINT_ON: Add a point in front of char.
  * @param  apostrophe: Flag indicating if a apostrophe has to be add in front 
  *                     of displayed character.
  *          This parameter  can be one of the following values:
  *              @arg APOSTROPHE_OFF: No apostrophe to add in back of char.
  *              @arg APOSTROPHE_ON: Add an apostrophe in back of char.
  * @param  position: Position in the LCD of the caracter to write.
  *                   This parameter can be any value in range [0:7].
  * @retval None
  */
void LCD_GLASS_DisplayChar(uint8_t* ch, Point_Typedef point, DoublePoint_Typedef DoublePoint, uint8_t position)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }
  /*!< LCD Write Char */
  LCD_GLASS_WriteChar(ch, point, DoublePoint, position);

  /*!< Request LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  This function writes a char in the LCD RAM.
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @retval None
  */
void LCD_GLASS_DisplayString(uint8_t* ptr)
{
  uint8_t i = 0x00;

  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

  /*!< Send the string character by character on lCD */
  while ((*ptr != 0) & (i < 8))
  {
    /*!< Display one character on LCD */
    LCD_GLASS_WriteChar(ptr, POINT_OFF, DOUBLEPOINT_OFF, i);
    /*!< Point on the next character */
    ptr++;
    /*!< Increment the character counter */
    i++;
  }
  /*!< Request LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  Display a string in scrolling mode
  * @note   The LCD should be cleared before to start the write operation.  
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @param  nScroll: Specifies how many time the message will be scrolled
  * @param  ScrollSpeed: Specifies the speed of the scroll.
  *                     Low value gives higher speed. 
  * @retval None
  */
void LCD_GLASS_ScrollString(uint8_t* ptr, uint16_t nScroll, uint16_t ScrollSpeed)
{
  uint8_t Repetition = 0;
  uint8_t* ptr1;
  uint8_t str[8] = "";
  ptr1 = ptr;

  LCD_GLASS_DisplayString(ptr1);

  delay(ScrollSpeed);

  for (Repetition = 0; Repetition < nScroll; Repetition++)
  {
    *(str + 1) = *ptr1;
    *(str + 2) = *(ptr1 + 1);
    *(str + 3) = *(ptr1 + 2);
    *(str + 4) = *(ptr1 + 3);
    *(str + 5) = *(ptr1 + 4);
    *(str + 6) = *(ptr1 + 5);
    *(str + 7) =*(ptr1 + 6);
    *(str) = *(ptr1 + 7);    
    LCD_GLASS_ClearTextZone();
    LCD_GLASS_DisplayString(str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 7);
    *(str + 2) = *ptr1;
    *(str + 3) = *(ptr1 + 1);
    *(str + 4) = *(ptr1 + 2);
    *(str + 5) = *(ptr1 + 3);
    *(str + 6) = *(ptr1 + 4);
    *(str + 7) = *(ptr1 + 5);
    *(str) = *(ptr1 + 6);    
    LCD_GLASS_ClearTextZone();
    LCD_GLASS_DisplayString(str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 6);
    *(str + 2) = *(ptr1 + 7);
    *(str + 3) = *ptr1;
    *(str + 4) = *(ptr1 + 1);
    *(str + 5) = *(ptr1 + 2);
    *(str + 6) = *(ptr1 + 3);
    *(str + 7) = *(ptr1 + 4);
    *(str) = *(ptr1 + 5);
    LCD_GLASS_ClearTextZone();
    LCD_GLASS_DisplayString(str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 5);
    *(str + 2) = *(ptr1 + 6);
    *(str + 3) = *(ptr1 + 7);
    *(str + 4) = *ptr1;
    *(str + 5) = *(ptr1 + 1);
    *(str + 6) = *(ptr1 + 2);
    *(str + 7) = *(ptr1 + 3);
    *(str) = *(ptr1 + 4);
    LCD_GLASS_ClearTextZone();
    LCD_GLASS_DisplayString(str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 4);
    *(str + 2) = *(ptr1 + 5);
    *(str + 3) = *(ptr1 + 6);
    *(str + 4) = *(ptr1 + 7);
    *(str + 5) = *ptr1;
    *(str + 6) = *(ptr1 + 1);
    *(str + 7) = *(ptr1 + 2);
    *(str) = *(ptr1 + 3);
    LCD_GLASS_ClearTextZone();
    LCD_GLASS_DisplayString(str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 3);
    *(str + 2) = *(ptr1 + 4);
    *(str + 3) = *(ptr1 + 5);
    *(str + 4) = *(ptr1 + 6);
    *(str + 5) = *(ptr1 + 7);
    *(str + 6) = *ptr1;
    *(str + 7) = *(ptr1 + 1);
    *(str) = *(ptr1 + 2);
    LCD_GLASS_ClearTextZone();
    LCD_GLASS_DisplayString(str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 2);
    *(str + 2) = *(ptr1 + 3);
    *(str + 3) = *(ptr1 + 4);
    *(str + 4) = *(ptr1 + 5);
    *(str + 5) = *(ptr1 + 6);
    *(str + 6) = *(ptr1 + 7);
    *(str + 7) = *ptr1;
    *(str) = *(ptr1 + 1);
    LCD_GLASS_ClearTextZone();
    LCD_GLASS_DisplayString(str);
    delay(ScrollSpeed);

    *(str + 1) = *(ptr1 + 1);
    *(str + 2) = *(ptr1 + 2);
    *(str + 3) = *(ptr1 + 3);
    *(str + 4) = *(ptr1 + 4);
    *(str + 5) = *(ptr1 + 5);
    *(str + 6) = *(ptr1 + 6);
    *(str + 7) = *(ptr1 + 7);
    *(str) = *(ptr1);
    LCD_GLASS_ClearTextZone();
    LCD_GLASS_DisplayString(str);
    delay(ScrollSpeed);
  }
}

/**
  * @brief  This function Clears the LCD Glass Text Zone.
  * @param  None
  * @retval None
  */
void LCD_GLASS_ClearTextZone(void)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

  LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFE49A49;
  LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFE49249;
  LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFE00000;
  LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFE00000;
  LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFE00800;
  LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFE00000;

  /*!< Request LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  This function Clear the whole LCD RAM.
  * @param  None 
  * @retval None
  */
void LCD_GLASS_Clear(void)
{
  uint32_t counter = 0;

  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

  for (counter = 0; counter <= 0x0F; counter++)
  {
    LCD->RAM[counter] = (uint32_t)0x00;
  }

  /*!< Request LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  Configure ST Logo display.
  * @param  NewState: enable or disable the logo display .
  * @retval None
  */
void LCD_GLASS_DisplayLogo(FunctionalState NewState)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

  if (NewState != DISABLE)
  {
    /* Set logo on  */
    LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00000040;
  }
  else
  {
    /* Set logo of  */
    LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFFBF;
  }

  /*!< Request LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  Configure the LCD Battery Level.
  * @param  BatteryLevel: Specify the Battery Level to set.
  * @retval None
  */
void LCD_GLASS_BatteryLevelConfig(BatteryLevel_TypeDef BatteryLevel)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

  switch (BatteryLevel)
  {
    /* BATTERYLEVEL 1/4 */
    case BATTERYLEVEL_1_4:
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFFF6;
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFFF6;
      /* Set BATTERYLEVEL_1_4  on  */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00000001;
      break;

    /* BATTERYLEVEL 1/2 */
    case BATTERYLEVEL_1_2:
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFFF6;
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFFF6;
      /* Set BatteryLevel_1_4 on  */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00000001;
      /* Set BatteryLevel_1_2 on  */
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00000001;
      break;

    /* Battery Level 3/4 */
    case BATTERYLEVEL_3_4:
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFFF6;
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFFF6;
      /* Set BATTERYLEVEL_3_4 on  */
      /* Set BatteryLevel_1_4 on  */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00000001;
      /* Set BatteryLevel_1_2 on  */
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00000009;
      break;

    /* BATTERYLEVEL_FULL */
    case BATTERYLEVEL_FULL:
      /* Set BATTERYLEVEL_3_4 on  */
      /* Set BatteryLevel_1_4 on  */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00000009;
      /* Set BATTERYLEVEL_FULL on  */
      /* Set BatteryLevel_1_2 on  */
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00000009;
      break;

    case BATTERYLEVEL_OFF:
      /* Set BATTERYLEVEL_3_4 off  */
      /* Set BATTERYLEVEL_1_4 off  */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFFF6;
      /* Set BATTERYLEVEL_1_2 off  */
      /* Set BATTERYLEVEL_FULL off  */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFFF6;
      break;

    default:
      break;
  }
  /*!< Request LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  Configure the LCD ArrowDirection.
  * @param  Arrow: Specify the ArrowDirection to set.
  * @retval None
  */
void LCD_GLASS_ArrowConfig(ArrowDirection_TypeDef ArrowDirection)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

  switch (ArrowDirection)
  {
    /* ARROWDIRECTION_UP */
    case ARROWDIRECTION_UP:
      /* Set ARROWDIRECTION_UP on  */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00001000;
      break;

    /* ARROWDIRECTION_LEFT */
    case ARROWDIRECTION_LEFT :
      /* Set ARROWDIRECTION_LEFT on  */
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00001000;
      break;

    /* ARROWDIRECTION_DOWN */
    case ARROWDIRECTION_DOWN:
      /* Set ARROWDIRECTION_DOWN on  */
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00008000;
      break;

    /* ARROWDIRECTION_RIGHT */
    case ARROWDIRECTION_RIGHT:
      /* Set ARROWDIRECTION_RIGHT on  */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00008000;
      break;

    case ARROWDIRECTION_OFF:
      /* Set ARROWDIRECTION_UP off  */
      /* Set ARROWDIRECTION_RIGHT off  */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFF6FFF;
      /* Set ARROWDIRECTION_LEFT off  */
      /* Set ARROWDIRECTION_DOWN off  */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFF6FFF;
      break;

    default:
      break;
  }
  /*!< Request LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  Configure the LCD Temperature level display .
  * @param  Temperature: indicate the Temperature level to set.
  * @retval None
  */
void LCD_GLASS_TemperatureConfig(TemperatureLevel_TypeDef Temperature)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

  switch (Temperature)
  {
    /* Temperature level 1*/
    case TEMPERATURELEVEL_1 :
      /* Clear Temperature level 5 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFDFF;
      /* Clear Temperature level 6 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFDFF;
      /* Clear Temperature level 4 and level 2 */
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFF3FF;
      /* Clear Temperature level 1 and level 3 */
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFF3FF;
      /* Set Temperature level 1*/
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00000200;
      break;

    /* Temperature level 2 */
    case TEMPERATURELEVEL_2:
      /* Clear Temperature level 5 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFDFF;
      /* Clear Temperature level 6 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFDFF;
      /* Clear Temperature level 4 and level 2 */
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFF3FF;
      /* Clear Temperature level 1 and level 3 */
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFF3FF;
      /* Set Temperature level 1 */
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00000200;
      /* Set Temperature level 2 */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00000200;
      break;

      /* Temperature level 3 */
    case TEMPERATURELEVEL_3:
      /* Clear Temperature level 5 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFDFF;
      /* Clear Temperature level 6 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFDFF;
      /* Clear Temperature level 4 and level 2 */
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFF3FF;
      /* Clear Temperature level 1 and level 3 */
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFF3FF;
      /* Set Temperature level 1 */
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00000200;
      /* Set Temperature level 2 */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00000200;
      /* Set Temperature level 3 */
      LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00000400;
      break;

      /* Temperature level 4*/
    case TEMPERATURELEVEL_4:
      /* Clear Temperature level 5 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFDFF;
      /* Clear Temperature level 6 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFDFF;
      /* Clear Temperature level 4 and level 2 */
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFF3FF;
      /* Clear Temperature level 1 and level 3 */
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFF3FF;
      /* Set Temperature level 1  */
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00000200;
      /* Set Temperature level 2  */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00000200;
      /* Set Temperature level 3 */
      LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00000400;
      /* Set Temperature level 4 */
      LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00000400;
      break;

      /* Temperature_2*/
    case TEMPERATURELEVEL_5:
      /* Clear Temperature level 5 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFDFF;
      /* Clear Temperature level 6 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFDFF;
      /* Clear Temperature level 4 and level 2 */
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFF3FF;
      /* Clear Temperature level 1 and level 3 */
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFF3FF;
      /* Set Temperature level 1 */
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00000200;
      /* Set Temperature level 2 */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00000200;
      /* Set Temperature level 3 and level 5 */
      LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00000C00;
      /* Set Temperature level 4 */
      LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00000400;
      break;

      /* Temperature_6*/
    case  TEMPERATURELEVEL_6:
      /* Clear Temperature level 5 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFDFF;
      /* Clear Temperature level 6 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFDFF;
      /* Clear Temperature level 4 and level 2 */
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFF3FF;
      /* Clear Temperature level 1 and level 3 */
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFF3FF;
      /* Set Temperature level 1 */
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00000200;
      /* Set Temperature level 2 */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00000200;
      /* Set Temperature level 3 and level 5 */
      LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00000C00;
      /* Set Temperature level 4 and level 6 */
      LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00000C00;
      break;

    case TEMPERATURELEVEL_OFF:
      /* Clear Temperature level 5 */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFFDFF;
      /* Clear Temperature level 6 */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFFFDFF;
      /* Clear Temperature level 4 and level 2 */
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFF3FF;
      /* Clear Temperature level 1 and level 3 */
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFF3FF;
      break;

    default:
      break;
  }

  /*!< Request LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  Configure the LCD Value Unit.
  * @param  ValueUnit: indicate the Value Unit to set.
  * @param  NewState: enable or disable the Digit.
  * @retval None
  */
void LCD_GLASS_ValueUnitConfig(ValueUnit_TypeDef ValueUnit)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

  switch (ValueUnit)
  {
    /* VALUEUNIT MILLIAMPERE*/
    case VALUEUNIT_MILLIAMPERE:
      /* Set VALUEUNIT_MILLIAMPERE on  */
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00040000;
      break;

    /* VALUEUNIT MICROAMPERE)*/
    case VALUEUNIT_MICROAMPERE:
      /* Set VALUEUNIT_MICROAMPERE  */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00040000;
      break;

    /* VALUEUNIT NANOAMPERE*/
    case  VALUEUNIT_NANOAMPERE:
      /* Set VALUEUNIT_NANOAMPERE on  */
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00000040;
      break;

    case  VALUEUNIT_OFF:
      /* Set VALUEUNIT_MILLIAMPERE and VALUEUNIT_NANOAMPERE OFF */
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFFBFFBF;
      /* Set VALUEUNIT_MICROAMPERE off  */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFBFFFF;
      break;

    default:
      break;
  }
  /*!< Request LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  Configure the LCD sign.
  * @param  Sign: indicate the sign to set.
  * @param  NewState: enable or disable the Digit.
  * @retval None
  */
void LCD_GLASS_SignCmd(Sign_TypeDef Sign, FunctionalState NewState)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

  if (NewState != DISABLE)
  {
    switch (Sign)
    {
      /* Sign positive */
      case SIGN_POSITIVE:
        /* Set SIGN_POSITIVE on */
        LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)0x00000800;
        break;

      /* Sign negative*/
      case SIGN_NEGATIVE:
        /* Set SIGN_NEGATIVE on */
        LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00000800;
        break;

      default:
        break;
    }
  }
  else
  {
    switch (Sign)
    {
      /* Sign positive */
      case SIGN_POSITIVE:
        /* Set SIGN_POSITIVE off */
        LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFFFF7FF;
        break;

      /* Sign negative */
      case SIGN_NEGATIVE:
        /* Set SIGN_NEGATIVE off */
        LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFFFF7FF;
        break;

      default:
        break;
    }
  }

  /*!< Request LCD RAM update */
  LCD_UpdateDisplayRequest();
}
/**
  * @brief  Set Matrix Pixel on.
  * @param  PixelRow: Specify Matrix Row.
  * @param  PixelColumn: Specify Matrix Column.
  * @retval None
  */
void LCD_GLASS_WriteMatrixPixel(PixelRow_TypeDef PixelRow, PixelColumn_TypeDef PixelColumn)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

  switch (PixelRow)
  {
    case PIXELROW_1:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00000200;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 1 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00004000;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 1 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00002000;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 1 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00001000;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 1 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00020000;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 1 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00010000;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 1 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00008000;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 1 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00100000;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 1 , Column = 9 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00080000;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 1 , Column = 10 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00040000;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 1 , Column = 11 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00000004;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 1 , Column = 12 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00000002;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 1 , Column = 13 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00000001;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 1 , Column = 14 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00000020;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 1 , Column = 15 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00000010;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 1 , Column = 16 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00000008;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 1 , Column = 17 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00000100;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 1 , Column = 18 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00000080;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 1 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00000040;
          break;

        default:
          break;

      }
      break;

    case PIXELROW_2:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 2 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00000200;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 2 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00004000;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 2 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00002000;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 2 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00001000;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 2 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00020000;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 2 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00010000;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 2 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00008000;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 2 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00100000;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 2 , Column = 9 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00080000;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 2 , Column = 10 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00040000;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 2 , Column = 11 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00000004;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 2 , Column = 12 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00000002;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 2 , Column = 13 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00000001;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 2 , Column = 14 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00000020;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 2 , Column = 15 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00000010;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 2 , Column = 16 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00000008;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 2 , Column = 17 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00000100;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 2 , Column = 18 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00000080;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 2 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00000040;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_3:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 3 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00200000;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 3 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00400000;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 3 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x00800000;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 3 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x01000000;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 3 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x02000000;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 3 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x04000000;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 3 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x08000000;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 3 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x10000000;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 3 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x20000000;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 3 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x40000000;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 3 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)0x80000000;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 3 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_5] |= (uint32_t)0x00000001;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 3 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_5] |= (uint32_t)0x00000002;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 3 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_5] |= (uint32_t)0x00000004;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 3 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_5] |= (uint32_t)0x00000008;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 3 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_5] |= (uint32_t)0x00000010;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 3 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_5] |= (uint32_t)0x00000020;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 3 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_5] |= (uint32_t)0x00000040;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 3 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_5] |= (uint32_t)0x00000080;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_4:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 4 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00200000;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 4 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00400000;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 4 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x00800000;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 4 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x01000000;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 4 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x02000000;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 4 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x04000000;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 4 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x08000000;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 4 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x10000000;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 4 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x20000000;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 4 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x40000000;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 4 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)0x80000000;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 4 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_7] |= (uint32_t)0x00000001;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 4 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_7] |= (uint32_t)0x00000002;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 4 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_7] |= (uint32_t)0x00000004;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 4 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_7] |= (uint32_t)0x00000008;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 4 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_7] |= (uint32_t)0x00000010;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 4 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_7] |= (uint32_t)0x00000020;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 4 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_7] |= (uint32_t)0x00000040;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 4 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_7] |= (uint32_t)0x00000080;
          break;

        default:
          break;

      }
      break;

    case PIXELROW_5:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 5 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00200000;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 5 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00400000;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 5 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x00800000;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 5 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x01000000;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 5 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x02000000;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 5 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x04000000;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 5 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x08000000;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 5 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x10000000;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 5 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x20000000;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 5 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x40000000;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 5 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)0x80000000;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 5 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_1] |= (uint32_t)0x00000001;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 5 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_1] |= (uint32_t)0x00000002;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 5 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_1] |= (uint32_t)0x00000004;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 5 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_1] |= (uint32_t)0x00000008;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 5 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_1] |= (uint32_t)0x00000010;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 5 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_1] |= (uint32_t)0x00000020;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 5 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_1] |= (uint32_t)0x00000040;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 5 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_1] |= (uint32_t)0x00000080;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_6:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 6 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00200000;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 6 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00400000;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 6 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x00800000;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 6 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x01000000;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 6 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x02000000;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 6 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x04000000;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 6 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x08000000;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 6 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x10000000;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 6 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x20000000;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 6 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x40000000;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 6 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)0x80000000;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 6 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_3] |= (uint32_t)0x00000001;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 6 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_3] |= (uint32_t)0x00000002;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 6 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_3] |= (uint32_t)0x00000004;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 6 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_3] |= (uint32_t)0x00000008;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 6 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_3] |= (uint32_t)0x00000010;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 6 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_3] |= (uint32_t)0x00000020;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 6 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_3] |= (uint32_t)0x00000040;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 6 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_3] |= (uint32_t)0x00000080;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_7:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 7 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)0x00200000;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 7 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)0x00400000;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 7 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)0x00800000;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 7 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)0x01000000;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 7 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)0x02000000;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 7 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)0x04000000;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 7 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)0x08000000;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 7 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)0x10000000;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 7 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)0x20000000;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 7 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)0x40000000;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 7 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_14] |= (uint32_t)0x80000000;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 7 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_15] |= (uint32_t)0x00000001;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 7 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_15] |= (uint32_t)0x00000002;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 7 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_15] |= (uint32_t)0x00000004;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 7 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_15] |= (uint32_t)0x00000008;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 7 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_15] |= (uint32_t)0x00000010;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 7 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_15] |= (uint32_t)0x00000020;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 7 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_15] |= (uint32_t)0x00000040;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 7 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_15] |= (uint32_t)0x00000080;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_8:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 8 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)0x00200000;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 8 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)0x00400000;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 8 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)0x00800000;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 8 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)0x01000000;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 8 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)0x02000000;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 8 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)0x04000000;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 8 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)0x08000000;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 8 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)0x10000000;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 8 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)0x20000000;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 8 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)0x40000000;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 8 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_12] |= (uint32_t)0x80000000;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 8 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_13] |= (uint32_t)0x00000001;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 8 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_13] |= (uint32_t)0x00000002;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 8 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_13] |= (uint32_t)0x00000004;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 8 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_13] |= (uint32_t)0x00000008;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 8 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_13] |= (uint32_t)0x00000010;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 8 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_13] |= (uint32_t)0x00000020;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 8 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_13] |= (uint32_t)0x00000040;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 8 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_13] |= (uint32_t)0x00000080;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_9:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 9 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)0x00200000;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 9 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)0x00400000;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 9 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)0x00800000;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 9 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)0x01000000;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 9 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)0x02000000;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 9 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)0x04000000;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 9 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)0x08000000;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 9 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)0x10000000;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 9 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)0x20000000;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 9 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)0x40000000;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 9 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_10] |= (uint32_t)0x80000000;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 9 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_11] |= (uint32_t)0x00000001;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 9 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_11] |= (uint32_t)0x00000002;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 9 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_11] |= (uint32_t)0x00000004;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 9 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_11] |= (uint32_t)0x00000008;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 9 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_11] |= (uint32_t)0x00000010;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 9 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_11] |= (uint32_t)0x00000020;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 9 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_11] |= (uint32_t)0x00000040;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 9 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_11] |= (uint32_t)0x00000080;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_10:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 10 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)0x00200000;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 10 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)0x00400000;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 10 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)0x00800000;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 10 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)0x01000000;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 10 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)0x02000000;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 10 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)0x04000000;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 10 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)0x08000000;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 10 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)0x10000000;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 10 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)0x20000000;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 10 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)0x40000000;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 10 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_8] |= (uint32_t)0x80000000;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 10 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_9] |= (uint32_t)0x00000001;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 10 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_9] |= (uint32_t)0x00000002;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 10 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_9] |= (uint32_t)0x00000004;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 10 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_9] |= (uint32_t)0x00000008;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 10 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_9] |= (uint32_t)0x00000010;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 10 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_9] |= (uint32_t)0x00000020;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 10 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_9] |= (uint32_t)0x00000040;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 10 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_9] |= (uint32_t)0x00000080;
          break;

        default:
          break;
      }
    break;
  }

  /*!< Request LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  Set Matrix Pixel off.
  * @param  PixelRow: Specify Matrix Row.
  * @param  PixelColumn: Specify Matrix Column.
  * @retval None
  */
void LCD_GLASS_ClearMatrixPixel(PixelRow_TypeDef PixelRow, PixelColumn_TypeDef PixelColumn)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }

  switch (PixelRow)
  {
    case PIXELROW_1:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 1 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFFDFF;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 1 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFBFFF;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 1 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFDFFF;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 1 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFEFFF;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 1 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFDFFFF;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 1 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFEFFFF;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 1 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFF7FFF;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 1 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFEFFFFF;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 1 , Column = 9 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFF7FFFF;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 1 , Column = 10 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFBFFFF;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 1 , Column = 11 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFFFFB;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 1 , Column = 12 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFFFFD;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 1 , Column = 13 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFFFFE;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 1 , Column = 14 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFFFDF;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 1 , Column = 15 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFFFEF;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 1 , Column = 16 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFFFF7;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 1 , Column = 17 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFFEFF;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 1 , Column = 18 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFFF7F;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 1 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFFFFFBF;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_2:

      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 2 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFFDFF;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 2 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFBFFF;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 2 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFDFFF;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 2 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFEFFF;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 2 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFDFFFF;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 2 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFEFFFF;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 2 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFF7FFF;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 2 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFEFFFFF;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 2 , Column = 9 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFF7FFFF;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 2 , Column = 10 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFBFFFF;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 2 , Column = 11 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFFFFB;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 2 , Column = 12 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFFFFD;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 2 , Column = 13 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFFFFE;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 2 , Column = 14 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFFFDF;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 2 , Column = 15 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFFFEF;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 2 , Column = 16 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFFFF7;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 2 , Column = 17 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFFEFF;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 2 , Column = 18 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFFF7F;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 2 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFFFFFBF;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_3:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 3 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFDFFFFF;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 3 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFFBFFFFF;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 3 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFF7FFFFF;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 3 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFEFFFFFF;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 3 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFDFFFFFF;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 3 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xFBFFFFFF;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 3 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xF7FFFFFF;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 3 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xEFFFFFFF;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 3 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xDFFFFFFF;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 3 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0xBFFFFFFF;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 3 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)0x7FFFFFFF;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 3 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)0xFFFFFFFE;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 3 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)0xFFFFFFFD;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 3 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)0xFFFFFFFB;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 3 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)0xFFFFFFF7;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 3 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)0xFFFFFFEF;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 3 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)0xFFFFFFDF;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 3 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)0xFFFFFFBF;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 3 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)0xFFFFFF7F;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_4:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 4 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFDFFFFF;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 4 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFFBFFFFF;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 4 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFF7FFFFF;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 4 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFEFFFFFF;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 4 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFDFFFFFF;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 4 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xFBFFFFFF;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 4 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xF7FFFFFF;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 4 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xEFFFFFFF;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 4 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xDFFFFFFF;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 4 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0xBFFFFFFF;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 4 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)0x7FFFFFFF;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 4 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)0xFFFFFFFE;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 4 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)0xFFFFFFFD;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 4 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)0xFFFFFFFB;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 4 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)0xFFFFFFF7;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 4 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)0xFFFFFFEF;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 4 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)0xFFFFFFDF;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 4 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)0xFFFFFFBF;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 4 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)0xFFFFFF7F;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_5:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 5 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFDFFFFF;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 5 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFFBFFFFF;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 5 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFF7FFFFF;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 5 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFEFFFFFF;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 5 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFDFFFFFF;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 5 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xFBFFFFFF;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 5 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xF7FFFFFF;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 5 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xEFFFFFFF;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 5 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xDFFFFFFF;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 5 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0xBFFFFFFF;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 5 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)0x7FFFFFFF;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 5 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)0xFFFFFFFE;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 5 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)0xFFFFFFFD;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 5 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)0xFFFFFFFB;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 5 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)0xFFFFFFF7;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 5 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)0xFFFFFFEF;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 5 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)0xFFFFFFDF;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 5 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)0xFFFFFFBF;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 5 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)0xFFFFFF7F;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_6:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 6 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFDFFFFF;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 6 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFFBFFFFF;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 6 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFF7FFFFF;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 6 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFEFFFFFF;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 6 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFDFFFFFF;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 6 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xFBFFFFFF;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 6 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xF7FFFFFF;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 6 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xEFFFFFFF;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 6 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xDFFFFFFF;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 6 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0xBFFFFFFF;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 6 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)0x7FFFFFFF;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 6 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)0xFFFFFFFE;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 6 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)0xFFFFFFFD;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 6 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)0xFFFFFFFB;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 6 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)0xFFFFFFF7;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 6 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)0xFFFFFFEF;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 6 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)0xFFFFFFDF;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 6 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)0xFFFFFFBF;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 6 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)0xFFFFFF7F;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_7:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 7 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFDFFFFF;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 7 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFFBFFFFF;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 7 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFF7FFFFF;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 7 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFEFFFFFF;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 7 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFDFFFFFF;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 7 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xFBFFFFFF;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 7 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xF7FFFFFF;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 7 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xEFFFFFFF;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 7 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xDFFFFFFF;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 7 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0xBFFFFFFF;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 7 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_14] &= (uint32_t)0x7FFFFFFF;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 7 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_15] &= (uint32_t)0xFFFFFFFE;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 7 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_15] &= (uint32_t)0xFFFFFFFD;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 7 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_15] &= (uint32_t)0xFFFFFFFB;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 7 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_15] &= (uint32_t)0xFFFFFFF7;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 7 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_15] &= (uint32_t)0xFFFFFFEF;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 7 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_15] &= (uint32_t)0xFFFFFFDF;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 7 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_15] &= (uint32_t)0xFFFFFFBF;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 7 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_15] &= (uint32_t)0xFFFFFF7F;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_8:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 8 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFDFFFFF;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 8 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFFBFFFFF;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 8 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFF7FFFFF;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 8 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFEFFFFF0;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 8 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFDFFFFFF;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 8 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xFBFFFFF0;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 8 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xF7FFFFFF;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 8 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xEFFFFFFF;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 8 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xDFFFFFFF;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 8 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0xBFFFFFFF;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 8 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_12] &= (uint32_t)0x7FFFFFFF;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 8 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_13] &= (uint32_t)0xFFFFFFFE;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 8 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_13] &= (uint32_t)0xFFFFFFFD;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 8 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_13] &= (uint32_t)0xFFFFFFFB;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 8 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_13] &= (uint32_t)0xFFFFFFF7;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 8 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_13] &= (uint32_t)0xFFFFFFEF;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 8 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_13] &= (uint32_t)0xFFFFFFDF;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 8 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_13] &= (uint32_t)0xFFFFFFBF;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 8 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_13] &= (uint32_t)0xFFFFFF7F;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_9:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 9 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFDFFFFF;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 9 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFFBFFFFF;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 9 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFF7FFFFF;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 9 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFEFFFFFF;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 9 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFDFFFFFF;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 9 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xFBFFFFFF;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 9 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xF7FFFFFF;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 9 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xEFFFFFFF;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 9 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xDFFFFFFF;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 9 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0xBFFFFFF;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 9 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_10] &= (uint32_t)0x7FFFFFFF;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 9 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_11] &= (uint32_t)0xFFFFFFFE;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 9 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_11] &= (uint32_t)0xFFFFFFFD;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 9 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_11] &= (uint32_t)0xFFFFFFFB;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 9 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_11] &= (uint32_t)0xFFFFFFF7;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 9 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_11] &= (uint32_t)0xFFFFFFEF;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 9 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_11] &= (uint32_t)0xFFFFFFDF;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 9 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_11] &= (uint32_t)0xFFFFFFBF;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 9 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_11] &= (uint32_t)0xFFFFFF7F;
          break;

        default:
          break;
      }
      break;

    case PIXELROW_10:
      switch (PixelColumn)
      {
        case PIXELCOLUMN_1:
          /* Position : Row = 10 , Column = 1 */
          LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFDFFFFF;
          break;

        case PIXELCOLUMN_2:
          /* Position : Row = 10 , Column = 2 */
          LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFFBFFFFF;
          break;

        case PIXELCOLUMN_3:
          /* Position : Row = 10 , Column = 3 */
          LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFF7FFFFF;
          break;

        case PIXELCOLUMN_4:
          /* Position : Row = 10 , Column = 4 */
          LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFEFFFFFF;
          break;

        case PIXELCOLUMN_5:
          /* Position : Row = 10 , Column = 5 */
          LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFDFFFFFF;
          break;

        case PIXELCOLUMN_6:
          /* Position : Row = 10 , Column = 6 */
          LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xFBFFFFFF;
          break;

        case PIXELCOLUMN_7:
          /* Position : Row = 10 , Column = 7 */
          LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xF7FFFFFF;
          break;

        case PIXELCOLUMN_8:
          /* Position : Row = 10 , Column = 8 */
          LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xEFFFFFFF;
          break;

        case PIXELCOLUMN_9:
          /* Position : Row = 10 , Column = 9*/
          LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xDFFFFFFF;
          break;

        case PIXELCOLUMN_10:
          /* Position : Row = 10 , Column = 10*/
          LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0xBFFFFFFF;
          break;

        case PIXELCOLUMN_11:
          /* Position : Row = 10 , Column = 11*/
          LCD->RAM[LCD_RAMRegister_8] &= (uint32_t)0x7FFFFFFF;
          break;

        case PIXELCOLUMN_12:
          /* Position : Row = 10 , Column = 12*/
          LCD->RAM[LCD_RAMRegister_9] &= (uint32_t)0xFFFFFFFE;
          break;

        case PIXELCOLUMN_13:
          /* Position : Row = 10 , Column = 13*/
          LCD->RAM[LCD_RAMRegister_9] &= (uint32_t)0xFFFFFFFD;
          break;

        case PIXELCOLUMN_14:
          /* Position : Row = 10 , Column = 14*/
          LCD->RAM[LCD_RAMRegister_9] &= (uint32_t)0xFFFFFFFB;
          break;

        case PIXELCOLUMN_15:
          /* Position : Row = 10 , Column = 15*/
          LCD->RAM[LCD_RAMRegister_9] &= (uint32_t)0xFFFFFFF7;
          break;

        case PIXELCOLUMN_16:
          /* Position : Row = 10 , Column = 16*/
          LCD->RAM[LCD_RAMRegister_9] &= (uint32_t)0xFFFFFFEF;
          break;

        case PIXELCOLUMN_17:
          /* Position : Row = 10 , Column = 17*/
          LCD->RAM[LCD_RAMRegister_9] &= (uint32_t)0xFFFFFFDF;
          break;

        case PIXELCOLUMN_18:
          /* Position : Row = 10 , Column = 18*/
          LCD->RAM[LCD_RAMRegister_9] &= (uint32_t)0xFFFFFFBF;
          break;

        case PIXELCOLUMN_19:
          /* Position : Row = 10 , Column = 19 */
          LCD->RAM[LCD_RAMRegister_9] &= (uint32_t)0xFFFFFF7F;
          break;

        default:
          break;
      }
    break;
  }

  /*!< Request LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  Converts an ascii char to the a LCD Digit (previous coding).
  * @param  c: a char to display.
  * @param  Point: a point to add in front of a char
  *         This parameter can be: POINT_OFF or POINT_ON
  * @param  DoublePoint: flag indicating if Double Point has to be added in front
  *         of the displayed character.
  *         This parameter can be: DOUBLEPOINT_ON or DOUBLEPOINT_OFF.
  * @retval None
  */
static void Convert(uint8_t* c, Point_Typedef Point, DoublePoint_Typedef DoublePoint)
{
  uint32_t ch = 0 , tmp = 0;
  uint8_t i = 0;

  /* The character c is a letter in upper case*/
  if ((*c < (uint8_t)0x5B)&(*c > (uint8_t)0x40))
  {
    ch = LetterMap[*c-(uint8_t)0x41];
  }
  /* The character c is a number*/
  if ((*c < (uint8_t)0x3A)&(*c > (uint8_t)0x2F))
  {
    ch = NumberMap[*c-(uint8_t)0x30];
  }
  /* The character c is a space character */
  if (*c == (uint8_t)0x20)
  {
    ch = (uint32_t)0x00;
  }
  /* Set the Q pixel in the character that can be displayed if the point is on */
  if (Point == POINT_ON)
  {
    ch |= (uint32_t)0x00400000;
  }

  /* Set the P pixel in the character that can be displayed if the double point is on */
  if (DoublePoint == DOUBLEPOINT_ON)
  {
    ch |= (uint32_t)0x00000040;
  }

  for (i = 0;i < 6; i++)
  {
    tmp = ch & Mask[i];
    Digit[i] = (uint8_t)(tmp >> (uint8_t)Shift[i]);
  }
}

/**
  * @brief  Configures the LCD Segments and Coms GPIOs.
  * @param  None 
  * @retval None
  */
static void LCD_GPIOConfig(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /*!< Enable GPIOA, GPIOB, GPIOC, GPIOD and GPIOE AHB Clocks */
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA | RCC_AHBPeriph_GPIOB | RCC_AHBPeriph_GPIOC
                        | RCC_AHBPeriph_GPIOD | RCC_AHBPeriph_GPIOE, ENABLE);

  /*!< Connect PA.08 to LCD COM0 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource8, GPIO_AF_LCD);

  /*!< Connect PA.09 to LCD COM1 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource9, GPIO_AF_LCD);

  /*!< Connect PA.10 to LCD COM2 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource10, GPIO_AF_LCD);

  /*!< Connect PB.09 to LCD COM3 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource9, GPIO_AF_LCD);

  /*!< Connect PA.01 to LCD SEG0 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource1, GPIO_AF_LCD);

  /*!< Connect PA.02 to LCD SEG1 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource2, GPIO_AF_LCD);

  /*!< Connect PA.03 to LCD SEG2 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource3, GPIO_AF_LCD);

  /*!< Connect PA.06 to LCD SEG3 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_LCD);

  /*!< Connect PA.07 to LCD SEG4 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_LCD);

  /*!< Connect PB.00 to LCD SEG5 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_LCD);

  /*!< Connect PB.01 to LCD SEG6 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource1, GPIO_AF_LCD);

  /*!< Connect PB.03 to LCD SEG7 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource3, GPIO_AF_LCD);

  /*!< Connect PB.04 to LCD SEG8 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource4, GPIO_AF_LCD);

  /*!< Connect PB.05 to LCD SEG9 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource5, GPIO_AF_LCD); 

  /*!< Connect PB.10 to LCD SEG10 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_LCD);

  /*!< Connect PB.11 to LCD SEG11 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_LCD);

  /*!< Connect PB.12 to LCD SEG12 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource12, GPIO_AF_LCD);

  /*!< Connect PB.13 to LCD SEG13 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource13, GPIO_AF_LCD);

  /*!< Connect PB.14 to LCD SEG14 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource14, GPIO_AF_LCD);

  /*!< Connect PB.15 to LCD SEG15 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource15, GPIO_AF_LCD);

  /*!< Connect PB.8 to LCD SEG16 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_LCD);

  /*!< Connect PA15 to LCD SEG17 */
  GPIO_PinAFConfig(GPIOA, GPIO_PinSource15, GPIO_AF_LCD);

  /*!< Connect PC.00 to LCD SEG18 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource0, GPIO_AF_LCD);

  /*!< Connect PC.01 to LCD SEG19 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource1, GPIO_AF_LCD);

  /*!< Connect PC.02 to LCD SEG20 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource2, GPIO_AF_LCD);

  /*!< Connect PC.03 to LCD SEG21 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource3, GPIO_AF_LCD);

  /*!< Connect PC.04 to LCD SEG22 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource4, GPIO_AF_LCD);

  /*!< Connect PC.05 to LCD SEG23 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource5, GPIO_AF_LCD);

  /*!< Connect PC.06 to LCD SEG24 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource6, GPIO_AF_LCD);

  /*!< Connect PC.07 to LCD SEG25 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource7, GPIO_AF_LCD);

  /*!< Connect PC.08 to LCD SEG26 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource8, GPIO_AF_LCD);

  /*!< Connect PC.09 to LCD SEG27 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource9, GPIO_AF_LCD);

  /*!< Connect PD.08 to LCD SEG28 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource8, GPIO_AF_LCD);

  /*!< Connect PD.09 to LCD SEG29 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource9, GPIO_AF_LCD);

  /*!< Connect PD.10 to LCD SEG30 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_LCD);

  /*!< Connect PD.11 to LCD SEG31 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_LCD);

  /*!< Connect PD.12 to LCD SEG32 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_LCD);

  /*!< Connect PD.13 to LCD SEG33 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource13, GPIO_AF_LCD);

  /*!< Connect PD.14 to LCD SEG34 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_LCD);

  /*!< Connect PD.15 to LCD SEG35 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_LCD);

  /*!< Connect PE.00 to LCD SEG36 */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource0, GPIO_AF_LCD);

  /*!< Connect PE.01 to LCD SEG37 */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource1, GPIO_AF_LCD);

  /*!< Connect PE.02 to LCD SEG38 */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource2, GPIO_AF_LCD);

  /*!< Connect PE.03 to LCD SEG39 */
  GPIO_PinAFConfig(GPIOE, GPIO_PinSource3, GPIO_AF_LCD);

  /*!< Connect PC.10 to LCD SEG40 or LCD COM4 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_LCD);

  /*!< Connect PC.11 to LCD SEG41 or LCD COM5 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_LCD);

  /*!< Connect PC.12 to LCD SEG42 or LCD COM6 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_LCD);

  /*!< Connect PD.02 to LCD SEG43 or LCD COM7 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_LCD);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | \
                                GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | \
                                GPIO_Pin_15;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_3 | GPIO_Pin_4 | \
                                GPIO_Pin_5 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | \
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | \
                                GPIO_Pin_15;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7 | \
                                GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11 | \
                                GPIO_Pin_12;
  GPIO_Init(GPIOC, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | \
                                GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | \
                                GPIO_Pin_15;
  GPIO_Init(GPIOD, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
}

/**
  * @brief  Inserts a delay time.
  * @param  nCount: specifies the delay time length.
  * @retval None
  */
static void delay(__IO uint32_t nCount)
{
  __IO uint32_t index = 0; 
  for(index = (0xFF * nCount); index != 0; index--)
  {
  }
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

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

