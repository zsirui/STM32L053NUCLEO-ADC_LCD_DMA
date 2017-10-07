/**
	******************************************************************************
	* File Name          : LCD.c
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

/* Includes ------------------------------------------------------------------*/
#include "lcd.h"
#include <stdio.h>
#include <string.h>
#include "gpio.h"

/* USER CODE BEGIN 0 */
static void Convert(uint8_t Char, Point_Typedef Point);
static void WriteFloat(float src);
void HAL_LCD_MspInit(LCD_HandleTypeDef* lcdHandle);
#define			ASCII_CHAR_0		0x30  /* 0 */
/**
================================================================================
															GLASS LCD MAPPING
================================================================================
LCD allows to display informations on six 8-segment digits and 4 bars:

	1      2      3      4
 ----   ----   ----   ----
 |  |   |  | o |  |   |  |
 ----   ----   ----   ----
 |  |   |  | o |  |   |  |
 ---- * ---- * ---- * ----
 
LCD segment mapping:
--------------------
	---A---        _ 
	|     |    P1 |_|
	F     B          
	|     |        _ 
	---G---    P1 |_|
	|     |          
	E     C          
	|     |    _ 
	---D---   |_| P(2,3,4)
An LCD character coding is based on the following matrix:
COM          0   1   2   3
SEG(n)     { A , F , E , P }
SEG(n+1)   { B , G , C , D }

The character 'A' for example is:
	-------------------------------
LSB        { 1 , 1 , 1 , 0 }
MSB 	   { 1 , 1 , 1 , 0 }
					-------------------
	'A' =  0xEE   hex
*/
/* Constant table for number '0' --> '9' 'a' --> 'f' */
const uint16_t NumberMap[16]=
{
	/* 0      1      2      3      4      5      6      7      8      9      a      b      c      d      e      f  */
	0xBE,  0xA0,  0xDA,  0xF8,  0xE4,  0x7C  ,0x7E,  0xA8,  0xFE,  0xFC,  0xEE,  0x76,  0x1E,  0xF2,  0x5E,  0x4E
};
const uint16_t dot = 0x01;
uint32_t Digit[2];     /* Digit frame buffer */
uint32_t DigitData[8];

/**
	* @brief  Configure the LCD Blink mode and Blink frequency.
	* @param  BlinkMode: specifies the LCD blink mode.
	*   This parameter can be one of the following values:
	*     @arg LCD_BLINKMODE_OFF:           Blink disabled
	*     @arg LCD_BLINKMODE_SEG0_COM0:     Blink enabled on SEG[0], COM[0] (1 pixel)
	*     @arg LCD_BLINKMODE_SEG0_ALLCOM:   Blink enabled on SEG[0], all COM (up to 8 
	*                                       pixels according to the programmed duty)
	*     @arg LCD_BLINKMODE_ALLSEG_ALLCOM: Blink enabled on all SEG and all COM 
	*                                       (all pixels)
	* @param  BlinkFrequency: specifies the LCD blink frequency.
	*     @arg LCD_BLINKFREQUENCY_DIV8:    The Blink frequency = fLcd/8
	*     @arg LCD_BLINKFREQUENCY_DIV16:   The Blink frequency = fLcd/16
	*     @arg LCD_BLINKFREQUENCY_DIV32:   The Blink frequency = fLcd/32
	*     @arg LCD_BLINKFREQUENCY_DIV64:   The Blink frequency = fLcd/64 
	*     @arg LCD_BLINKFREQUENCY_DIV128:  The Blink frequency = fLcd/128
	*     @arg LCD_BLINKFREQUENCY_DIV256:  The Blink frequency = fLcd/256
	*     @arg LCD_BLINKFREQUENCY_DIV512:  The Blink frequency = fLcd/512
	*     @arg LCD_BLINKFREQUENCY_DIV1024: The Blink frequency = fLcd/1024
	* @retval None
	*/
void LCD_GLASS_BlinkConfig(void)
{
	__HAL_LCD_BLINK_CONFIG(&hlcd, LCD_BLINKMODE_ALLSEG_ALLCOM, LCD_BLINKFREQUENCY_DIV1024);
}

void LCD_GLASS_DisplayFloat(float src)
{
	WriteFloat(src);
	/* Update the LCD display */
	HAL_LCD_UpdateDisplayRequest(&hlcd);
}

void LCD_GLASS_Clear(void)
{
	HAL_LCD_Clear(&hlcd); 
}

static void Convert(uint8_t Char, Point_Typedef Point)
{
	uint16_t ch = 0 ;
	uint8_t loop = 0, index = 0;
	switch (Char)
	{
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':      
			ch = NumberMap[Char - ASCII_CHAR_0];    
			break;
		default:
			ch = 0x00;
			break;
	}
	if (Point == POINT_ON)
	{
		ch |= dot;
	}
	for (loop = 4, index=0; index < 2; loop -= 4, index++)
	{
		Digit[index] = (ch >> loop) & 0x0f; /*To isolate the less significant digit */
	}
}

/**
PIN		1		2		3		4		5		6		7		8		9		10		11		12
COM1   COM1								1A		1B		2A		2B		3A		3B		4A		4B
COM2		   COM2						1F		1G		2F		2G		3F		3G		4F		4G
COM3				   COM3				1E		1C		2E		2C		3E		3C		4E		4C
COM4						   COM4		1P		1D		2P		2D		3P		3D		4P		4D
*/

static void WriteFloat(float src)
{
	uint32_t data = 0x00;
	char ch[10];
	int i, j = 0;
	sprintf(ch, "%5.3f", src);
	for(i = 0; i < strlen(ch); i++)
	{
		if(ch[i] == '.')
		{
			Convert((uint8_t)ch[i + 1], POINT_ON);
			i += 1;
		}else
		{
			Convert((uint8_t)ch[i], POINT_OFF);
		}
		DigitData[j] = Digit[0];
		DigitData[j + 1] = Digit[1];
		j += 2;
	}
	data = (((DigitData[1] & 0x8) >> 3) << LCD_SEG0_SHIFT) | (((DigitData[0] & 0x8) >> 3) << LCD_SEG1_SHIFT) |
		   (((DigitData[3] & 0x8) >> 3) << LCD_SEG2_SHIFT) | (((DigitData[2] & 0x8) >> 3) << LCD_SEG3_SHIFT) |
		   (((DigitData[5] & 0x8) >> 3) << LCD_SEG4_SHIFT) | (((DigitData[4] & 0x8) >> 3) << LCD_SEG5_SHIFT) |
		   (((DigitData[7] & 0x8) >> 3) << LCD_SEG6_SHIFT) | (((DigitData[6] & 0x8) >> 3) << LCD_SEG7_SHIFT);
	HAL_LCD_Write(&hlcd, LCD_DIGIT_COM0, LCD_DIGIT_COM0_SEG_MASK, data);
	data = (((DigitData[1] & 0x4) >> 2) << LCD_SEG0_SHIFT) | (((DigitData[0] & 0x4) >> 2) << LCD_SEG1_SHIFT) |
		   (((DigitData[3] & 0x4) >> 2) << LCD_SEG2_SHIFT) | (((DigitData[2] & 0x4) >> 2) << LCD_SEG3_SHIFT) |
		   (((DigitData[5] & 0x4) >> 2) << LCD_SEG4_SHIFT) | (((DigitData[4] & 0x4) >> 2) << LCD_SEG5_SHIFT) |
		   (((DigitData[7] & 0x4) >> 2) << LCD_SEG6_SHIFT) | (((DigitData[6] & 0x4) >> 2) << LCD_SEG7_SHIFT);
	HAL_LCD_Write(&hlcd, LCD_DIGIT_COM1, LCD_DIGIT_COM1_SEG_MASK, data);
	data = (((DigitData[1] & 0x2) >> 1) << LCD_SEG0_SHIFT) | (((DigitData[0] & 0x2) >> 1) << LCD_SEG1_SHIFT) |
		   (((DigitData[3] & 0x2) >> 1) << LCD_SEG2_SHIFT) | (((DigitData[2] & 0x2) >> 1) << LCD_SEG3_SHIFT) |
		   (((DigitData[5] & 0x2) >> 1) << LCD_SEG4_SHIFT) | (((DigitData[4] & 0x2) >> 1) << LCD_SEG5_SHIFT) |
		   (((DigitData[7] & 0x2) >> 1) << LCD_SEG6_SHIFT) | (((DigitData[6] & 0x2) >> 1) << LCD_SEG7_SHIFT);
	HAL_LCD_Write(&hlcd, LCD_DIGIT_COM2, LCD_DIGIT_COM2_SEG_MASK, data);
	data = ((DigitData[1] & 0x1) << LCD_SEG0_SHIFT) | ((DigitData[0] & 0x1) << LCD_SEG1_SHIFT) |
		   ((DigitData[3] & 0x1) << LCD_SEG2_SHIFT) | ((DigitData[2] & 0x1) << LCD_SEG3_SHIFT) |
		   ((DigitData[5] & 0x1) << LCD_SEG4_SHIFT) | ((DigitData[4] & 0x1) << LCD_SEG5_SHIFT) |
		   ((DigitData[7] & 0x1) << LCD_SEG6_SHIFT) | ((DigitData[6] & 0x1) << LCD_SEG7_SHIFT);
	HAL_LCD_Write(&hlcd, LCD_DIGIT_COM3, LCD_DIGIT_COM3_SEG_MASK, data);
}
/* USER CODE END 0 */

LCD_HandleTypeDef hlcd;

/* LCD init function */
void MX_LCD_Init(void)
{
	hlcd.Instance = LCD;
	hlcd.Init.Prescaler = LCD_PRESCALER_1;
	hlcd.Init.Divider = LCD_DIVIDER_16;
	hlcd.Init.Duty = LCD_DUTY_1_4;
	hlcd.Init.Bias = LCD_BIAS_1_3;
	hlcd.Init.VoltageSource = LCD_VOLTAGESOURCE_INTERNAL;
	hlcd.Init.Contrast = LCD_CONTRASTLEVEL_5;
	hlcd.Init.DeadTime = LCD_DEADTIME_0;
	hlcd.Init.PulseOnDuration = LCD_PULSEONDURATION_4;
	hlcd.Init.HighDrive = LCD_HIGHDRIVE_0;
	hlcd.Init.BlinkMode = LCD_BLINKMODE_OFF;
	hlcd.Init.BlinkFrequency = LCD_BLINKFREQUENCY_DIV1024;
	hlcd.Init.MuxSegment = LCD_MUXSEGMENT_DISABLE;
	HAL_LCD_MspInit(&hlcd);
	if (HAL_LCD_Init(&hlcd) != HAL_OK)
	{
		_Error_Handler(__FILE__, __LINE__);
	}
	/**configure the VLCD Decoupling capacitance connection*/
	__HAL_SYSCFG_VLCD_CAPA_CONFIG(SYSCFG_VLCD_PB12_EXT_CAPA_ON);
	LCD_GLASS_Clear();
}

void HAL_LCD_MspInit(LCD_HandleTypeDef* lcdHandle)
{

	GPIO_InitTypeDef GPIO_InitStruct;
	if(lcdHandle->Instance==LCD)
	{
	/* USER CODE BEGIN LCD_MspInit 0 */

	/* USER CODE END LCD_MspInit 0 */
	/* LCD clock enable */
	__HAL_RCC_LCD_CLK_ENABLE();

	/**LCD GPIO Configuration    
	PC0     ------> LCD_SEG18
	PC1     ------> LCD_SEG19
	PC2     ------> LCD_SEG20
	PC3     ------> LCD_SEG21
	PC4     ------> LCD_SEG22
	PC5     ------> LCD_SEG23
	PB12    ------> LCD_VLCD2
	PC6     ------> LCD_SEG24
	PC7     ------> LCD_SEG25
	PA8     ------> LCD_COM0
	PA9     ------> LCD_COM1
	PA10    ------> LCD_COM2
	PB9     ------> LCD_COM3 
	*/
	
	GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
												|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_LCD;
	HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_12;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_LCD;
	HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

	GPIO_InitStruct.Pin = GPIO_PIN_9;
	GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
	GPIO_InitStruct.Pull = GPIO_NOPULL;
	GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	GPIO_InitStruct.Alternate = GPIO_AF1_LCD;
	HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

	/* USER CODE BEGIN LCD_MspInit 1 */

	/* USER CODE END LCD_MspInit 1 */
	}
}

void HAL_LCD_MspDeInit(LCD_HandleTypeDef* lcdHandle)
{

	if(lcdHandle->Instance==LCD)
	{
	/* USER CODE BEGIN LCD_MspDeInit 0 */

	/* USER CODE END LCD_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_LCD_CLK_DISABLE();
	
		/**LCD GPIO Configuration    
		PC0     ------> LCD_SEG18
		PC1     ------> LCD_SEG19
		PC2     ------> LCD_SEG20
		PC3     ------> LCD_SEG21
		PC4     ------> LCD_SEG22
		PC5     ------> LCD_SEG23
		PB12    ------> LCD_VLCD2
		PC6     ------> LCD_SEG24
		PC7     ------> LCD_SEG25
		PA8     ------> LCD_COM0
		PA9     ------> LCD_COM1
		PA10    ------> LCD_COM2
		PB9     ------> LCD_COM3 
		*/
		HAL_GPIO_DeInit(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3 
													|GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

		HAL_GPIO_DeInit(GPIOB, GPIO_PIN_12|GPIO_PIN_9);

		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_8|GPIO_PIN_9|GPIO_PIN_10);

	/* USER CODE BEGIN LCD_MspDeInit 1 */

	/* USER CODE END LCD_MspDeInit 1 */
	}
} 

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */

/**
	* @}
	*/

/**
	* @}
	*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
