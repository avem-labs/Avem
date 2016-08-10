/**
  ******************************************************************************
  * @file    stm32l152_eval_glass_lcd.c
  * @author  MCD Application Team
  * @version V5.0.2
  * @date    09-March-2012
  * @brief   This file includes the LCD Glass driver for Pacific Display 
  *          (LCD_PD878, PD878-DP-FH-W-LV-6-RH) Module of STM32L152-EVAL board RevB.
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
#include "stm32l152_eval_glass_lcd.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
  * @{
  */ 

/** @addtogroup STM32L152_EVAL
  * @{
  */
  
/** @defgroup STM32L152_EVAL_GLASS_LCD 
  * @brief This file includes the LCD Glass driver for Pacific Display 
  *        (LCD_PD878, PD878-DP-FH-W-LV-6-RH) Module of STM32L152-EVAL board.
  * @{
  */ 

/** @defgroup STM32L152_EVAL_GLASS_LCD_Private_Types
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32L152_EVAL_GLASS_LCD_Private_Defines
  * @{
  */ 
/**
  * @}
  */   


/** @defgroup STM32L152_EVAL_GLASS_LCD_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup STM32L152_EVAL_GLASS_LCD_Private_Variables
  * @{
  */ 

/**
  @verbatim      
================================================================================
                              GLASS LCD MAPPING
================================================================================
               A
      --  ----------
    X \/  |\   |I  /|
         F| H  |  J |B
          |  \ | /  |
          --G-- --K--
          |   /| \  |
        E |  L |  N |C
          | /  |M  \|   _
          -----------  | |DP
              D         -

A LCD character coding is based on the following matrix:


{ X , F , E , D  }
{ I , J , K , N  }
{ A , B , C , DP }
{ H , G , L , M  }

The character A for example is:
-------------------------------
 { 0 , 1 , 1 , 0 }
 { 0 , 0 , 1 , 0 }
 { 1 , 1 , 1 , 0 }
 { 0 , 1 , 0 , 0 }
-------------------
=  4   D   7   0   hex

=> 'A' = 0x4D70

  @endverbatim
  */

/**
  * @brief LETTERS AND NUMBERS MAPPING DEFINITION
  */
uint8_t digit[4];     /* Digit LCD RAM buffer */
__I uint16_t mask[4] = {0xF000, 0x0F00, 0x00F0, 0x000F};
__I uint8_t shift[4] = {0x0C, 0x08, 0x04, 0x00};
  
/* Letters and number map of PD_878 LCD */
__I uint16_t LetterMap[26]=
{
/* A       B       C       D       E       F       G       H       I  */
0x4D70, 0x6469, 0x4111, 0x6449, 0x4911, 0x4910, 0x4171, 0x0D70, 0x6009,
/* J       K       L       M       N       O       P       Q       R  */
0x0451, 0x0B12, 0x0111, 0x8750, 0x8552, 0x4551, 0x4D30, 0x4553, 0x4D32,
/* S       T       U       V       W       X       Y       Z  */
0x4961, 0x6008, 0x0551, 0x0390, 0x05D2, 0x8282, 0x8208, 0x4281
};

__I uint16_t NumberMap[10]= 
{
/* 0       1       2       3       4       5       6       7       8       9  */
0x47D1, 0x0640, 0x4C31, 0x4C61, 0x0D60, 0x4961, 0x4971, 0x4440, 0x4D71, 0x4D61
};

/**
  * @}
  */ 
  

/** @defgroup STM32L152_EVAL_LCD_Private_Function_Prototypes
  * @{
  */ 
static void Convert(uint8_t* c, Point_Typedef point, Apostrophe_Typedef apostrophe);
static void delay(__IO uint32_t nCount);
static void LCD_GPIOConfig(void);

/**
  * @}
  */ 
  
/** @defgroup STM32L152_EVAL_LCD_Private_Functions
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
  
  LCD_InitStructure.LCD_Prescaler = LCD_Prescaler_8;
  LCD_InitStructure.LCD_Divider = LCD_Divider_16;
  LCD_InitStructure.LCD_Duty = LCD_Duty_1_4;
  LCD_InitStructure.LCD_Bias = LCD_Bias_1_3;
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
  * @param  ch: The character to dispaly.
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
void LCD_GLASS_DisplayChar(uint8_t* ch, Point_Typedef point, Apostrophe_Typedef apostrophe, uint8_t position)
{
  /*!< Wait Until the last LCD RAM update finish */
  while(LCD_GetFlagStatus(LCD_FLAG_UDR) != RESET)
  {
  }
  /*!< LCD Write Char */
  LCD_GLASS_WriteChar(ch, point, apostrophe, position);

  /*!< Requesy LCD RAM update */
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
    LCD_GLASS_WriteChar(ptr, POINT_OFF, APOSTROPHE_OFF, i);
    /*!< Point on the next character */
    ptr++;
    /*!< Increment the character counter */
    i++;
  }
  /*!< Requesy LCD RAM update */
  LCD_UpdateDisplayRequest();
}

/**
  * @brief  This function writes a char in the LCD RAM.
  * @param  ch: The character to dispaly.
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
void LCD_GLASS_WriteChar(uint8_t* ch, Point_Typedef point, Apostrophe_Typedef apostrophe, uint8_t position)
{
  uint32_t tmp = 0x00;
    
  Convert(ch, point, apostrophe); /*!< Convert the corresponding character */

  switch (position)
  {
    case 7:
      /*!< Clear the corresponding segments (SEG0, SEG1, SEG2, SEG3) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFFFFFFF0);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFFFFFFF0);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFFFFFFF0);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFFFFFFF0);
  
      /*!< Write the corresponding segments (SEG0, SEG1, SEG2, SEG3) */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)(digit[0]);
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)(digit[1]);
      LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)(digit[2]);
      LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)(digit[3]);
      break;
      
    case 6:
      /*!< Clear the corresponding segments (SEG4, SEG5, SEG6, SEG10) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFFFFFB8F);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFFFFFB8F);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFFFFFB8F);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFFFFFB8F);
  
      /*!< Write the corresponding segments (SEG4, SEG5, SEG6, SEG10) */
      tmp = (((digit[0] & 0x8) << 7) | ((digit[0]& 0x7) << 4));
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t) tmp;
      tmp = (((digit[1] & 0x8) << 7) | ((digit[1]& 0x7) << 4));
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t) tmp;
      tmp = (((digit[2] & 0x8) << 7) | ((digit[2]& 0x7) << 4));
      LCD->RAM[LCD_RAMRegister_4] |= (uint32_t) tmp;
      tmp = (((digit[3] & 0x8) << 7) | ((digit[3]& 0x7) << 4));
      LCD->RAM[LCD_RAMRegister_6] |= (uint32_t) tmp;
      break;
      
    case 5:
      /*!< Clear the corresponding segments (SEG11, SEG16, SEG18, SEG19) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFFF2F7FF);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFFF2F7FF);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFFF2F7FF);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFFF2F7FF);
  
      /*!< Write the corresponding segments (SEG11, SEG16, SEG18, SEG19) */
      tmp = (((digit[0] & 0x1) << 11) | ((digit[0]& 0x2) << 15) | ((digit[0]& 0xC) << 16));
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)(tmp);
      tmp = (((digit[1] & 0x1) << 11) | ((digit[1]& 0x2) << 15) | ((digit[1]& 0xC) << 16));
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)(tmp);
      tmp = (((digit[2] & 0x1) << 11) | ((digit[2]& 0x2) << 15) | ((digit[2]& 0xC) << 16));
      LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)(tmp);
      tmp = (((digit[3] & 0x1) << 11) | ((digit[3]& 0x2) << 15) | ((digit[3]& 0xC) << 16));
      LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)(tmp);
      break;
      
    case 4:
      /*!< Clear the corresponding segments (SEG20, SEG21, SEG22, SEG23) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFF0FFFFF);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFF0FFFFF);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFF0FFFFF);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFF0FFFFF);
  
      /*!< Write the corresponding segments (SEG20, SEG21, SEG22, SEG23) */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)(digit[0] << 20);
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)(digit[1] << 20);
      LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)(digit[2] << 20);
      LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)(digit[3] << 20);
      break;
      
    case 3:
      /*!< Clear the corresponding segments (SEG28, SEG29, SEG30, SEG31) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0x0FFFFFFF);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0x0FFFFFFF);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0x0FFFFFFF);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0x0FFFFFFF);
  
      /*!< Write the corresponding segments (SEG28, SEG29, SEG30, SEG31) */
      LCD->RAM[LCD_RAMRegister_0] |= (uint32_t)(digit[0] << 28);
      LCD->RAM[LCD_RAMRegister_2] |= (uint32_t)(digit[1] << 28);
      LCD->RAM[LCD_RAMRegister_4] |= (uint32_t)(digit[2] << 28);
      LCD->RAM[LCD_RAMRegister_6] |= (uint32_t)(digit[3] << 28);
      break;
      
    case 2:
      /*!< Clear the corresponding segments (SEG32, SEG33, SEG34, SEG35) */
      LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)(0xFFFFFFF0);
      LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)(0xFFFFFFF0);
      LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)(0xFFFFFFF0);
      LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)(0xFFFFFFF0);
  
      /*!< Write the corresponding segments (SEG32, SEG33, SEG34, SEG35) */
      LCD->RAM[LCD_RAMRegister_1] |= (uint32_t)(digit[0] << 0);
      LCD->RAM[LCD_RAMRegister_3] |= (uint32_t)(digit[1] << 0);
      LCD->RAM[LCD_RAMRegister_5] |= (uint32_t)(digit[2] << 0);
      LCD->RAM[LCD_RAMRegister_7] |= (uint32_t)(digit[3] << 0);
      break;
      
    case 1:
      /*!< Clear the corresponding segments (SEG36, SEG37, SEG38, SEG39) */
      LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)(0xFFFFFF0F);
      LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)(0xFFFFFF0F);
      LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)(0xFFFFFF0F);
      LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)(0xFFFFFF0F);

      /*!< Write the corresponding segments (SEG36, SEG37, SEG38, SEG39) */
      LCD->RAM[LCD_RAMRegister_1] |= (uint32_t)(digit[0] << 4);
      LCD->RAM[LCD_RAMRegister_3] |= (uint32_t)(digit[1] << 4);
      LCD->RAM[LCD_RAMRegister_5] |= (uint32_t)(digit[2] << 4);
      LCD->RAM[LCD_RAMRegister_7] |= (uint32_t)(digit[3] << 4);
      
      break;
      
    case 0:
      /*!< Clear the corresponding segments (SEG40, SEG41, SEG42, SEG43) */
      LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)(0xFFFFF0FF);
      LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)(0xFFFFF0FF);
      LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)(0xFFFFF0FF);
      LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)(0xFFFFF0FF);
  
      /*!< Write the corresponding segments (SEG40, SEG41, SEG42, SEG43) */
      LCD->RAM[LCD_RAMRegister_1] |= (uint32_t)(digit[0] << 8);
      LCD->RAM[LCD_RAMRegister_3] |= (uint32_t)(digit[1] << 8);
      LCD->RAM[LCD_RAMRegister_5] |= (uint32_t)(digit[2] << 8);
      LCD->RAM[LCD_RAMRegister_7] |= (uint32_t)(digit[3] << 8);
      break;
  }
}

/**
  * @brief  Display a string in scrolling mode
  * @note   The LCD should be cleared before to start the write operation.  
  * @param  ptr: Pointer to string to display on the LCD Glass.
  * @param  nScroll: Specifies how many time the message will be scrolled
  * @param  ScrollSpeed: Speciifes the speed of the scroll.
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
    LCD_GLASS_Clear();
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
    LCD_GLASS_Clear();
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
    LCD_GLASS_Clear();
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
    LCD_GLASS_Clear();
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
    LCD_GLASS_Clear();
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
    LCD_GLASS_Clear();
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
    LCD_GLASS_Clear();
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
    LCD_GLASS_Clear();
    LCD_GLASS_DisplayString(str);
    delay(ScrollSpeed);
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
  switch (position)
  {
    case 7:
      /*!< Clear the corresponding segments (SEG0, SEG1, SEG2, SEG3) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFFFFFFF0);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFFFFFFF0);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFFFFFFF0);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFFFFFFF0);
      break;
      
    case 6:
      /*!< Clear the corresponding segments (SEG4, SEG5, SEG6, SEG10) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFFFFFB8F);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFFFFFB8F);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFFFFFB8F);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFFFFFB8F);
      break;
      
    case 5:
      /*!< Clear the corresponding segments (SEG11, SEG16, SEG18, SEG19) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFFF2F7FF);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFFF2F7FF);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFFF2F7FF);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFFF2F7FF);
      break;
      
    case 4:
      /*!< Clear the corresponding segments (SEG20, SEG21, SEG22, SEG23) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0xFF0FFFFF);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0xFF0FFFFF);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0xFF0FFFFF);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0xFF0FFFFF);
      break;
      
    case 3:
      /*!< Clear the corresponding segments (SEG28, SEG29, SEG30, SEG31) */
      LCD->RAM[LCD_RAMRegister_0] &= (uint32_t)(0x0FFFFFFF);
      LCD->RAM[LCD_RAMRegister_2] &= (uint32_t)(0x0FFFFFFF);
      LCD->RAM[LCD_RAMRegister_4] &= (uint32_t)(0x0FFFFFFF);
      LCD->RAM[LCD_RAMRegister_6] &= (uint32_t)(0x0FFFFFFF);
      break;
      
    case 2:
      /*!< Clear the corresponding segments (SEG32, SEG33, SEG34, SEG35) */
      LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)(0xFFFFFFF0);
      LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)(0xFFFFFFF0);
      LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)(0xFFFFFFF0);
      LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)(0xFFFFFFF0);
      break;
      
    case 1:
      /*!< Clear the corresponding segments (SEG36, SEG37, SEG38, SEG39) */
      LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)(0xFFFFFF0F);
      LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)(0xFFFFFF0F);
      LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)(0xFFFFFF0F);
      LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)(0xFFFFFF0F);
      break;
      
    case 0:
      /*!< Clear the corresponding segments (SEG40, SEG41, SEG42, SEG43) */
      LCD->RAM[LCD_RAMRegister_1] &= (uint32_t)(0xFFFFF0FF);
      LCD->RAM[LCD_RAMRegister_3] &= (uint32_t)(0xFFFFF0FF);
      LCD->RAM[LCD_RAMRegister_5] &= (uint32_t)(0xFFFFF0FF);
      LCD->RAM[LCD_RAMRegister_7] &= (uint32_t)(0xFFFFF0FF);
      break;
  }
}

/**
  * @brief  This function Clear the whole LCD RAM.
  * @param  None 
  * @retval None
  */
void LCD_GLASS_Clear(void)
{
  uint32_t counter = 0;

  for (counter = 0; counter < 0x0F; counter++)
  {
    LCD->RAM[counter] =  (uint32_t)0x00;
  }
}

/**
  * @brief  Converts an ascii char to an LCD digit.
  * @param  c: Char to display.
  * @param  point: A point to add in front of char.
  *          This parameter  can be one of the following values:  
  *              @arg POINT_OFF : No point to add in front of char.
  *              @arg POINT_ON : Add a point in front of char.
  * @param  apostrophe: Flag indicating if a apostrophe has to be add in front 
  *                     of displayed character.
  *          This parameter  can be one of the following values:
  *              @arg APOSTROPHE_OFF : No apostrophe to add in back of char.
  *              @arg APOSTROPHE_ON : Add an apostrophe in back of char.
  * @retval None
  */
static void Convert(uint8_t* c, Point_Typedef point, Apostrophe_Typedef apostrophe)
{
  uint16_t ch = 0, tmp = 0;
  uint8_t i = 0;

  /*!< The character c is a letter in upper case*/
  if ((*c < 0x5B) & (*c > 0x40))
  {
    ch = LetterMap[*c - 0x41];
  }
  
  /*!< The character c is a number*/
  if ((*c < 0x3A) & (*c > 0x2F))
  {
    ch = NumberMap[*c - 0x30];
  }
  
  /*!< The character c is a space character */
  if (*c == 0x20)
  {
    ch =0x00;
  }
  
  /*!< Set the DP seg in the character that can be displayed if the point is on */
  if (point == POINT_ON)
  {
    ch |= 0x0004;
  }

  /*!< Set the X seg in the character that can be displayed if the apostrophe is on */
  if (apostrophe == APOSTROPHE_ON)
  {
    ch |= 0x1000;
  }

  for (i = 0; i < 4; i++)
  {
    tmp = ch & mask[i];
    digit[i] =(uint8_t)(tmp >> shift[i]);
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

  /*!< Connect PB.10 to LCD SEG10 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_LCD);

  /*!< Connect PB.11 to LCD SEG11 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource11, GPIO_AF_LCD);

  /*!< Connect PB.08 to LCD SEG16 */
  GPIO_PinAFConfig(GPIOB, GPIO_PinSource8, GPIO_AF_LCD);

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

  /*!< Connect PC.10 to LCD SEG40 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_LCD);

  /*!< Connect PC.11 to LCD SEG41 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_LCD);

  /*!< Connect PC.12 to LCD SEG42 */
  GPIO_PinAFConfig(GPIOC, GPIO_PinSource12, GPIO_AF_LCD);

  /*!< Connect PD.02 to LCD SEG43 */
  GPIO_PinAFConfig(GPIOD, GPIO_PinSource2, GPIO_AF_LCD);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | GPIO_Pin_6 | \
                                GPIO_Pin_7 | GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_40MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  GPIO_Init(GPIOA, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 | \
                                GPIO_Pin_10 | GPIO_Pin_11;
  GPIO_Init(GPIOB, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_2 | GPIO_Pin_3 | \
                                GPIO_Pin_4 | GPIO_Pin_5 | GPIO_Pin_10 | GPIO_Pin_11 | \
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
