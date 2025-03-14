/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LCD_EN_Pin GPIO_PIN_1
#define LCD_EN_GPIO_Port GPIOA
#define LCD_RS_Pin GPIO_PIN_2
#define LCD_RS_GPIO_Port GPIOA
#define bbt_Pin GPIO_PIN_3
#define bbt_GPIO_Port GPIOA
#define LCD_D4_Pin GPIO_PIN_4
#define LCD_D4_GPIO_Port GPIOA
#define LCD_D5_Pin GPIO_PIN_5
#define LCD_D5_GPIO_Port GPIOA
#define LCD_D6_Pin GPIO_PIN_6
#define LCD_D6_GPIO_Port GPIOA
#define LCD_D7_Pin GPIO_PIN_7
#define LCD_D7_GPIO_Port GPIOA
#define Brl_Pin GPIO_PIN_0
#define Brl_GPIO_Port GPIOB
#define R1_Pin GPIO_PIN_1
#define R1_GPIO_Port GPIOB
#define R2_Pin GPIO_PIN_2
#define R2_GPIO_Port GPIOB
#define R3_Pin GPIO_PIN_3
#define R3_GPIO_Port GPIOB
#define R4_Pin GPIO_PIN_4
#define R4_GPIO_Port GPIOB
#define C1_Pin GPIO_PIN_5
#define C1_GPIO_Port GPIOB
#define C2_Pin GPIO_PIN_6
#define C2_GPIO_Port GPIOB
#define C3_Pin GPIO_PIN_7
#define C3_GPIO_Port GPIOB
#define C4_Pin GPIO_PIN_8
#define C4_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
