/**
  ******************************************************************************
  * @file    lcd_log.c
  * @author  MCD Application Team
  * @version V5.0.2
  * @date    05-March-2012
  * @brief   This file provides all the LCD Log firmware functions.
  *          
  *          The LCD Log module allows to automatically set a header and footer
  *          on any application using the LCD display and allows to dump user,
  *          debug and error messages by using the following macros: LCD_ErrLog(),
  *          LCD_UsrLog() and LCD_DbgLog().
  *         
  *          It supports also the scroll feature by embedding an internal software
  *          cache for display. This feature allows to dump message sequentially
  *          on the display even if the number of displayed lines is bigger than
  *          the total number of line allowed by the display.
  *      
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
#include  "lcd_log.h"

/** @addtogroup Utilities
  * @{
  */

/** @addtogroup STM32_EVAL
* @{
*/

/** @addtogroup Common
  * @{
  */

/** @defgroup LCD_LOG 
* @brief LCD Log LCD_Application module
* @{
*/ 

/** @defgroup LCD_LOG_Private_Types
* @{
*/ 
/**
* @}
*/ 


/** @defgroup LCD_LOG_Private_Defines
* @{
*/ 

/**
* @}
*/ 


/** @defgroup LCD_LOG_Private_Macros
* @{
*/ 
/**
* @}
*/ 


/** @defgroup LCD_LOG_Private_Variables
* @{
*/ 

LCD_LOG_line LCD_CacheBuffer [LCD_CACHE_DEPTH]; 
uint16_t LCD_LineColor;
uint16_t LCD_CacheBuffer_xptr;
uint16_t LCD_CacheBuffer_yptr_top;
uint16_t LCD_CacheBuffer_yptr_bottom;

uint16_t LCD_CacheBuffer_yptr_top_bak;
uint16_t LCD_CacheBuffer_yptr_bottom_bak;

FunctionalState LCD_CacheBuffer_yptr_invert;
FunctionalState LCD_ScrollActive;
FunctionalState LCD_Lock;
FunctionalState LCD_Scrolled;
uint16_t LCD_ScrollBackStep;

/**
* @}
*/ 


/** @defgroup LCD_LOG_Private_FunctionPrototypes
* @{
*/ 
static void LCD_LOG_UpdateDisplay (void);
/**
* @}
*/ 


/** @defgroup LCD_LOG_Private_Functions
* @{
*/ 


/**
* @brief  Init the LCD Log module 
* @param  None
* @retval None
*/

void LCD_LOG_Init ( void)
{
  /* Deinit LCD cache */
  LCD_LOG_DeInit();
  /* Clear the LCD */
  LCD_Clear(Black);  
}

/**
* @brief DeInitializes the LCD Log module. 
* @param  None
* @retval None
*/
void LCD_LOG_DeInit(void)
{
  LCD_LineColor = LCD_LOG_DEFAULT_COLOR;
  LCD_CacheBuffer_xptr = 0;
  LCD_CacheBuffer_yptr_top = 0;
  LCD_CacheBuffer_yptr_bottom = 0;
  
  LCD_CacheBuffer_yptr_top_bak = 0;
  LCD_CacheBuffer_yptr_bottom_bak = 0;
  
  LCD_CacheBuffer_yptr_invert= ENABLE;
  LCD_ScrollActive = DISABLE;
  LCD_Lock = DISABLE;
  LCD_Scrolled = DISABLE;
  LCD_ScrollBackStep = 0;
}

/**
* @brief  Display the application header (title) on the LCD screen 
* @param  Title :  pointer to the string to be displayed
* @retval None
*/
void LCD_LOG_SetHeader (uint8_t *Title)
{
  sFONT *cFont;
  uint32_t size = 0 , idx; 
  uint8_t  *ptr = Title;
  uint8_t  tmp[27];
  
  /* center the header */
  while (*ptr++) size ++ ;

  /* truncate extra text */
  if(size > 26)
  {
     size = 26;
  }
  
  for (idx = 0 ; idx < 27 ; idx ++)
  { 
    tmp[idx] = ' '; 
  }

  for (idx = 0 ; idx < size ; idx ++)
  { 
    tmp[idx + (27 - size)/2] = Title[idx];
  }
  
  /* Clear the LCD */
  LCD_Clear(Black);
    
  /* Set the LCD Font */
  LCD_SetFont (&Font12x12);

  cFont = LCD_GetFont();  
  /* Set the LCD Text Color */
  LCD_SetTextColor(White);
  LCD_SetBackColor(Blue);
  LCD_ClearLine(0);
  LCD_DisplayStringLine(cFont->Height, tmp);
  LCD_ClearLine(2 * cFont->Height);

  LCD_SetBackColor(Black);
  LCD_SetFont (&Font8x12);
}

/**
* @brief  Display the application footer (status) on the LCD screen 
* @param  Status :  pointer to the string to be displayed
* @retval None
*/
void LCD_LOG_SetFooter(uint8_t *Status)
{
  sFONT *cFont;
  uint8_t  tmp[40], i;
  LCD_SetBackColor(Blue);
  cFont = LCD_GetFont();
  
  for (i= 0; i< (320/cFont->Width)-1 ; i++)
  {
    tmp[i] = ' ';
  }
  
  tmp[(320/cFont->Width)-1] = 0;
  LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - cFont->Height, tmp);
  LCD_DisplayStringLine(LCD_PIXEL_HEIGHT - cFont->Height, Status);
  LCD_SetBackColor(Black);  
}

/**
* @brief  Clear the Text Zone 
* @param  None 
* @retval None
*/
void LCD_LOG_ClearTextZone(void)
{
  uint8_t i=0;
  sFONT *cFont = LCD_GetFont();
  
  for (i= 0 ; i < YWINDOW_SIZE; i++)
  {
    LCD_ClearLine((i + YWINDOW_MIN) * cFont->Height);
  }
  
  LCD_LOG_DeInit();
}

/**
* @brief  Redirect the printf to the lcd 
* @param  c: character to be displayed
* @param  f: output file pointer
* @retval None
*/
PUTCHAR_PROTOTYPE
{
  
  sFONT *cFont = LCD_GetFont();
  uint32_t idx;
  
  if(LCD_Lock == DISABLE)
  {
    if((LCD_ScrollActive == ENABLE)||(LCD_ScrollActive == ENABLE))
    {
      LCD_CacheBuffer_yptr_bottom = LCD_CacheBuffer_yptr_bottom_bak;
      LCD_CacheBuffer_yptr_top    = LCD_CacheBuffer_yptr_top_bak;
      LCD_ScrollActive = DISABLE;
      LCD_Scrolled = DISABLE;
      LCD_ScrollBackStep = 0;
      
    }
    
    if(( LCD_CacheBuffer_xptr < LCD_PIXEL_WIDTH /cFont->Width ) &&  ( ch != '\n'))
    {
      LCD_CacheBuffer[LCD_CacheBuffer_yptr_bottom].line[LCD_CacheBuffer_xptr++] = (uint16_t)ch;
    }   
    else 
    {
      if(LCD_CacheBuffer_yptr_top >= LCD_CacheBuffer_yptr_bottom)
      {
        
        if(LCD_CacheBuffer_yptr_invert == DISABLE)
        {
          LCD_CacheBuffer_yptr_top++;
          
          if(LCD_CacheBuffer_yptr_top == LCD_CACHE_DEPTH)
          {
            LCD_CacheBuffer_yptr_top = 0;  
          }
        }
        else
        {
          LCD_CacheBuffer_yptr_invert= DISABLE;
        }
      }
      
      for(idx = LCD_CacheBuffer_xptr ; idx < LCD_PIXEL_WIDTH /cFont->Width; idx++)
      {
        LCD_CacheBuffer[LCD_CacheBuffer_yptr_bottom].line[LCD_CacheBuffer_xptr++] = ' ';
      }   
      LCD_CacheBuffer[LCD_CacheBuffer_yptr_bottom].color = LCD_LineColor;  
      
      LCD_CacheBuffer_xptr = 0;
      
      LCD_LOG_UpdateDisplay (); 
      
      LCD_CacheBuffer_yptr_bottom ++; 
      
      if (LCD_CacheBuffer_yptr_bottom == LCD_CACHE_DEPTH) 
      {
        LCD_CacheBuffer_yptr_bottom = 0;
        LCD_CacheBuffer_yptr_top = 1;    
        LCD_CacheBuffer_yptr_invert = ENABLE;
      }
      
      if( ch != '\n')
      {
        LCD_CacheBuffer[LCD_CacheBuffer_yptr_bottom].line[LCD_CacheBuffer_xptr++] = (uint16_t)ch;
      }
      
    }
  }
  return ch;
}
  
/**
* @brief  Update the text area display
* @param  None
* @retval None
*/
static void LCD_LOG_UpdateDisplay (void)
{
  uint8_t cnt = 0 ;
  uint16_t length = 0 ;
  uint16_t ptr = 0, index = 0;
  
  sFONT *cFont = LCD_GetFont();
  
  if((LCD_CacheBuffer_yptr_bottom  < (YWINDOW_SIZE -1)) && 
     (LCD_CacheBuffer_yptr_bottom  >= LCD_CacheBuffer_yptr_top))
  {
    LCD_SetTextColor(LCD_CacheBuffer[cnt + LCD_CacheBuffer_yptr_bottom].color);
    LCD_DisplayStringLine ((YWINDOW_MIN + LCD_CacheBuffer_yptr_bottom) * cFont->Height,
                           (uint8_t *)(LCD_CacheBuffer[cnt + LCD_CacheBuffer_yptr_bottom].line));
  }
  else
  {
    
    if(LCD_CacheBuffer_yptr_bottom < LCD_CacheBuffer_yptr_top)
    {
      /* Virtual length for rolling */
      length = LCD_CACHE_DEPTH + LCD_CacheBuffer_yptr_bottom ;
    }
    else
    {
      length = LCD_CacheBuffer_yptr_bottom;
    }
    
    ptr = length - YWINDOW_SIZE + 1;
    
    for  (cnt = 0 ; cnt < YWINDOW_SIZE ; cnt ++)
    {
      
      index = (cnt + ptr )% LCD_CACHE_DEPTH ;
      
      LCD_SetTextColor(LCD_CacheBuffer[index].color);
      LCD_DisplayStringLine ((cnt + YWINDOW_MIN) * cFont->Height, 
                             (uint8_t *)(LCD_CacheBuffer[index].line));
      
    }
  }
  
}

#ifdef LCD_SCROLL_ENABLED
/**
* @brief  Display previous text frame
* @param  None
* @retval Status
*/
ErrorStatus LCD_LOG_ScrollBack (void)
{
    
  if(LCD_ScrollActive == DISABLE)
  {
    
    LCD_CacheBuffer_yptr_bottom_bak = LCD_CacheBuffer_yptr_bottom;
    LCD_CacheBuffer_yptr_top_bak    = LCD_CacheBuffer_yptr_top;
    
    
    if(LCD_CacheBuffer_yptr_bottom > LCD_CacheBuffer_yptr_top) 
    {
      
      if ((LCD_CacheBuffer_yptr_bottom - LCD_CacheBuffer_yptr_top) <=  YWINDOW_SIZE)
      {
        LCD_Lock = DISABLE;
        return ERROR;
      }
    }
    LCD_ScrollActive = ENABLE;
    
    if((LCD_CacheBuffer_yptr_bottom  > LCD_CacheBuffer_yptr_top)&&
       (LCD_Scrolled == DISABLE ))
    {
      LCD_CacheBuffer_yptr_bottom--;
      LCD_Scrolled = ENABLE;
    }
    
  }
  
  if(LCD_ScrollActive == ENABLE)
  {
    LCD_Lock = ENABLE;
    
    if(LCD_CacheBuffer_yptr_bottom > LCD_CacheBuffer_yptr_top) 
    {
      
      if((LCD_CacheBuffer_yptr_bottom  - LCD_CacheBuffer_yptr_top) <  YWINDOW_SIZE )
      {
        LCD_Lock = DISABLE;
        return ERROR;
      }
      
      LCD_CacheBuffer_yptr_bottom --;
    }
    else if(LCD_CacheBuffer_yptr_bottom <= LCD_CacheBuffer_yptr_top)
    {
      
      if((LCD_CACHE_DEPTH  - LCD_CacheBuffer_yptr_top + LCD_CacheBuffer_yptr_bottom) < YWINDOW_SIZE)
      {
        LCD_Lock = DISABLE;
        return ERROR;
      }
      LCD_CacheBuffer_yptr_bottom --;
      
      if(LCD_CacheBuffer_yptr_bottom == 0xFFFF)
      {
        LCD_CacheBuffer_yptr_bottom = LCD_CACHE_DEPTH - 2;
      }
    }
    LCD_ScrollBackStep++;
    LCD_LOG_UpdateDisplay();
    LCD_Lock = DISABLE;
  }
  return SUCCESS;
}

/**
* @brief  Display next text frame
* @param  None
* @retval Status
*/
ErrorStatus LCD_LOG_ScrollForward (void)
{
  
  if(LCD_ScrollBackStep != 0)
  {
    if(LCD_ScrollActive == DISABLE)
    {
      
      LCD_CacheBuffer_yptr_bottom_bak = LCD_CacheBuffer_yptr_bottom;
      LCD_CacheBuffer_yptr_top_bak    = LCD_CacheBuffer_yptr_top;
      
      if(LCD_CacheBuffer_yptr_bottom > LCD_CacheBuffer_yptr_top) 
      {
        
        if ((LCD_CacheBuffer_yptr_bottom - LCD_CacheBuffer_yptr_top) <=  YWINDOW_SIZE)
        {
          LCD_Lock = DISABLE;
          return ERROR;
        }
      }
      LCD_ScrollActive = ENABLE;
      
      if((LCD_CacheBuffer_yptr_bottom  > LCD_CacheBuffer_yptr_top)&&
         (LCD_Scrolled == DISABLE ))
      {
        LCD_CacheBuffer_yptr_bottom--;
        LCD_Scrolled = ENABLE;
      }
      
    }
    
    if(LCD_ScrollActive == ENABLE)
    {
      LCD_Lock = ENABLE;
      LCD_ScrollBackStep--;
      
      if(++LCD_CacheBuffer_yptr_bottom == LCD_CACHE_DEPTH)
      {
        LCD_CacheBuffer_yptr_bottom = 0;
      }
      
      LCD_LOG_UpdateDisplay();
      LCD_Lock = DISABLE;
      
    }  
    return SUCCESS;
  }
  else // LCD_ScrollBackStep == 0 
  {
    LCD_Lock = DISABLE;
    return ERROR;
  }  
}
#endif /* LCD_SCROLL_ENABLED */

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
