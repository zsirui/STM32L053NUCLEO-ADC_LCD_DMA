/**
	******************************************************************************
	* File Name          : LCD.h
	* Description        : This file provides code for the configuration
	*                      of the LCD instances.
	******************************************************************************
	** This notice applies to any and all portions of this file
	* that are not between comment pairs USER CODE BEGIN and
	* USER CODE END. Other portions of this file, whether 
	* inserted by the user or by software development tools
	* are owned by their respective copyright owners.
	*
	* COPYRIGHT(c) 2017 STMicroelectronics
	*
	* Redistribution and use in source and binary forms, with or without modification,
	* are permitted provided that the following conditions are met:
	*   1. Redistributions of source code must retain the above copyright notice,
	*      this list of conditions and the following disclaimer.
	*   2. Redistributions in binary form must reproduce the above copyright notice,
	*      this list of conditions and the following disclaimer in the documentation
	*      and/or other materials provided with the distribution.
	*   3. Neither the name of STMicroelectronics nor the names of its contributors
	*      may be used to endorse or promote products derived from this software
	*      without specific prior written permission.
	*
	* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
	* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
	* IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
	* DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
	* FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
	* DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
	* SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
	* CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
	* OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
	* OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
	*
	******************************************************************************
	*/
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __lcd_H
#define __lcd_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32l0xx_hal.h"
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern LCD_HandleTypeDef hlcd;

/* USER CODE BEGIN Private defines */
typedef enum
{
	POINT_OFF = 0,
	POINT_ON = 1
}Point_Typedef;

#define MCU_LCD_COM0          LCD_RAM_REGISTER0
#define MCU_LCD_COM1          LCD_RAM_REGISTER2
#define MCU_LCD_COM2          LCD_RAM_REGISTER4
#define MCU_LCD_COM3          LCD_RAM_REGISTER6

#define MCU_LCD_SEG0          (1U << MCU_LCD_SEG0_SHIFT)
#define MCU_LCD_SEG1          (1U << MCU_LCD_SEG1_SHIFT)
#define MCU_LCD_SEG2          (1U << MCU_LCD_SEG2_SHIFT)
#define MCU_LCD_SEG3          (1U << MCU_LCD_SEG3_SHIFT)
#define MCU_LCD_SEG4          (1U << MCU_LCD_SEG4_SHIFT)
#define MCU_LCD_SEG5          (1U << MCU_LCD_SEG5_SHIFT)
#define MCU_LCD_SEG6          (1U << MCU_LCD_SEG6_SHIFT)
#define MCU_LCD_SEG7          (1U << MCU_LCD_SEG7_SHIFT)

//#define MCU_LCD_SEG0_SHIFT    0
//#define MCU_LCD_SEG1_SHIFT    1
//#define MCU_LCD_SEG2_SHIFT    2
//#define MCU_LCD_SEG3_SHIFT    3
//#define MCU_LCD_SEG4_SHIFT    4
//#define MCU_LCD_SEG5_SHIFT    5
//#define MCU_LCD_SEG6_SHIFT    6
//#define MCU_LCD_SEG7_SHIFT    7

#define MCU_LCD_SEG0_SHIFT    18
#define MCU_LCD_SEG1_SHIFT    19
#define MCU_LCD_SEG2_SHIFT    20
#define MCU_LCD_SEG3_SHIFT    21
#define MCU_LCD_SEG4_SHIFT    22
#define MCU_LCD_SEG5_SHIFT    23
#define MCU_LCD_SEG6_SHIFT    24
#define MCU_LCD_SEG7_SHIFT    25

#define LCD_COM0          MCU_LCD_COM0
#define LCD_COM1          MCU_LCD_COM1
#define LCD_COM2          MCU_LCD_COM2
#define LCD_COM3          MCU_LCD_COM3

#define LCD_SEG0          MCU_LCD_SEG0
#define LCD_SEG1          MCU_LCD_SEG1
#define LCD_SEG2          MCU_LCD_SEG2
#define LCD_SEG3          MCU_LCD_SEG3
#define LCD_SEG4          MCU_LCD_SEG4
#define LCD_SEG5          MCU_LCD_SEG5
#define LCD_SEG6          MCU_LCD_SEG6
#define LCD_SEG7          MCU_LCD_SEG7

#define LCD_DIGIT_COM0				LCD_COM0
#define LCD_DIGIT_COM0_SEG_MASK		~(LCD_SEG0 | LCD_SEG1 | LCD_SEG2 | LCD_SEG3 | LCD_SEG4 | LCD_SEG5 | LCD_SEG6 | LCD_SEG7)
#define LCD_DIGIT_COM1				LCD_COM1
#define LCD_DIGIT_COM1_SEG_MASK		~(LCD_SEG0 | LCD_SEG1 | LCD_SEG2 | LCD_SEG3 | LCD_SEG4 | LCD_SEG5 | LCD_SEG6 | LCD_SEG7)
#define LCD_DIGIT_COM2				LCD_COM2
#define LCD_DIGIT_COM2_SEG_MASK		~(LCD_SEG0 | LCD_SEG1 | LCD_SEG2 | LCD_SEG3 | LCD_SEG4 | LCD_SEG5 | LCD_SEG6 | LCD_SEG7)
#define LCD_DIGIT_COM3				LCD_COM3
#define LCD_DIGIT_COM3_SEG_MASK		~(LCD_SEG0 | LCD_SEG1 | LCD_SEG2 | LCD_SEG3 | LCD_SEG4 | LCD_SEG5 | LCD_SEG6 | LCD_SEG7)

#define LCD_SEG0_SHIFT          MCU_LCD_SEG0_SHIFT
#define LCD_SEG1_SHIFT          MCU_LCD_SEG1_SHIFT
#define LCD_SEG2_SHIFT          MCU_LCD_SEG2_SHIFT
#define LCD_SEG3_SHIFT          MCU_LCD_SEG3_SHIFT
#define LCD_SEG4_SHIFT          MCU_LCD_SEG4_SHIFT
#define LCD_SEG5_SHIFT          MCU_LCD_SEG5_SHIFT
#define LCD_SEG6_SHIFT          MCU_LCD_SEG6_SHIFT
#define LCD_SEG7_SHIFT          MCU_LCD_SEG7_SHIFT
/* USER CODE END Private defines */

extern void _Error_Handler(char *, int);
void MX_LCD_Init(void);
void LCD_GLASS_DisplayFloat(float src);
void LCD_GLASS_Clear(void);
void LCD_GLASS_BlinkConfig(void);

/* USER CODE BEGIN Prototypes */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ lcd_H */

/**
	* @}
	*/

/**
	* @}
	*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
