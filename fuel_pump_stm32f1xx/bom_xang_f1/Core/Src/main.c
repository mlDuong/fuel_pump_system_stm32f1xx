/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
//#include "LCD.h"
#include "stdio.h"
#include "lcd_txt.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
UART_HandleTypeDef huart1;

/* USER CODE BEGIN PV */
uint8_t numkey, row, flag_lit = 255, flag_tien= 255, flag_gia= 255,flag_lban = 0;
uint32_t gia, tien,thanhtien, mqtt_tien ;
char buffer[20];
float lit;
float count_lit;
float lit_ban;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART1_UART_Init(void);
/* USER CODE BEGIN PFP */
void matrix(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
unsigned char tong_tien[20];
unsigned char tong_lit[20];
unsigned char mqtt[50];
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
	
	
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
 
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
	lcd_init();
	lcd_puts(0,0,(int8_t*) "Nhap Gia 1 Lit");
	HAL_GPIO_WritePin(Brl_GPIO_Port,Brl_Pin, GPIO_PIN_SET);
	
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
		matrix();
		//---------------kiem tra trang thai co bom va so lit can ban-------------------------------- 
		if ((!HAL_GPIO_ReadPin(bbt_GPIO_Port,bbt_Pin))&&(lit_ban != 0))  // Kiem tra co bom
		{
			HAL_GPIO_WritePin(Brl_GPIO_Port,Brl_Pin,GPIO_PIN_RESET); // mo van va dong co bom xang 
		}
		//---------------kiem tra tin hieu tra ve cua bo dong  -------------------------------
		if ((!HAL_GPIO_ReadPin(GPIOB,GPIO_PIN_15))&&(lit_ban != 0))  // Kiem tra bo dong
		{
			count_lit ++;
			lcd_clear();
			//------ kiem tra so lit da ban 
			if (count_lit < lit_ban)
			{
				// chua ban du so luong can ban 
				lcd_puts(0,0,(int8_t*) "Lit Da Ban");
				sprintf(buffer, "%0.2f", count_lit);
				lcd_puts(1,0,(int8_t*) buffer);
							
			}else 
			{
				// da ban du so luong can ban 
				HAL_GPIO_WritePin(Brl_GPIO_Port,Brl_Pin,GPIO_PIN_SET); // khoa van va dong co bom xang 
				// hien thi so lit da ban len lcd 
				lcd_puts(0,0,(int8_t*) "Lit: ");
				sprintf(buffer, "%0.2f", count_lit);
				lcd_puts(0,5,(int8_t*) buffer);
				// hien thi so tien nguoi dung can tra len lcd 
				thanhtien = count_lit * gia;
				lcd_puts(1,0,(int8_t*) "Tien: ");
				sprintf(buffer, "%d", thanhtien);
				lcd_puts(1,6,(int8_t*) buffer);			
				
				// cho gia tri lit can ban ve 0 		
				lit_ban = 0;
				lit = 0;
				tien = 0;
			}	
		}
		if (flag_gia == 0)  // user nhap xong gia
		{
			// hien thi lai gia moi nhap 
			lcd_clear();
			lcd_puts(0,0,(int8_t*) "Gia");
			sprintf(buffer, "%d", gia);
			lcd_puts(1,0,(int8_t*) buffer);
			//--------------------
			flag_gia = 255;
			flag_lban = 0;
			HAL_Delay(50);
			// hien thi buoc tiep theo 
			lcd_clear();
			lcd_puts(0,0,(int8_t*) "Nhap Tien hoac ");
			lcd_puts(1,0,(int8_t*) "nhap Lit");
		} 
		HAL_Delay(50);
		if (flag_tien == 0) // user nhap tien
		{
			// hien thi so tien nguoi dung muon mua
			lcd_clear();
			lcd_puts(0,0,(int8_t*) "Tien");
			sprintf(buffer, "%d", tien); // So tien ng dung muon mua
			lcd_puts(1,0,(int8_t*) buffer);
			
			flag_tien =255;
			flag_lban = 1;
			count_lit = 0;
		} 
		HAL_Delay(50);
		if (flag_lit == 0)  // user nhap lit
		{
			// hien thi so lit nguoi dung muon mua 
			lcd_clear();
			lcd_puts(0,0,(int8_t*) "Lit");
			sprintf(buffer, "%0.1f", lit);
			lcd_puts(1,0,(int8_t*) buffer);
			//HAL_UART_Transmit(&huart1,(unsigned char*)buffer,1,0);	
			
			flag_lit =255;
			flag_lban = 1;
			count_lit = 0;
			HAL_Delay(50);
		}
		// da nhap xong gia va so lit hoac tien ma nguoi dung can mua 
		if (flag_lban == 1) 
		{
			if (!lit)
			{
				// nguoi dung nhap so tien muon mua, tinh so lit can ban 
				lit_ban  = (float) tien / (float) gia;
			}else 
			{
				// so lit can ban bang so lit nguoi dung muon mua 
				lit_ban = lit;
			}
			
			lcd_clear();
			lcd_puts(0,0,(int8_t*) "Lit Can Ban");
			sprintf(buffer, "%0.1f", lit_ban);
			lcd_puts(1,0,(int8_t*) buffer);
			// gui gia tri len server 
			strcpy(tong_lit, "Lit:");
			strcat(tong_lit, buffer);
			
			
			// Send data toi ESP qua UART
			mqtt_tien = lit_ban * gia ;
			sprintf(buffer, "%d", mqtt_tien);
			
			strcpy(tong_tien, "Tien:");
			strcat(tong_tien, buffer);
			strcat(tong_tien, ";");

			strcpy(mqtt, tong_tien);
			strcat(mqtt, tong_lit);
			HAL_UART_Transmit(&huart1,mqtt,sizeof(mqtt),HAL_MAX_DELAY);

			flag_lban = 0;
			lcd_clear();
		}
		
		
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
	
}

/**
  * @brief System Clock Configuration
  * @retval None
  */

void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL2;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief USART1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART1_UART_Init(void)
{

  /* USER CODE BEGIN USART1_Init 0 */

  /* USER CODE END USART1_Init 0 */

  /* USER CODE BEGIN USART1_Init 1 */

  /* USER CODE END USART1_Init 1 */
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART1_Init 2 */

  /* USER CODE END USART1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOA, LCD_EN_Pin|LCD_RS_Pin|LCD_D4_Pin|LCD_D5_Pin
                          |LCD_D6_Pin|LCD_D7_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, Brl_Pin|GPIO_PIN_15, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, C1_Pin|C2_Pin|C3_Pin|C4_Pin, GPIO_PIN_SET);

  /*Configure GPIO pins : LCD_EN_Pin LCD_RS_Pin LCD_D4_Pin LCD_D5_Pin
                           LCD_D6_Pin LCD_D7_Pin */
  GPIO_InitStruct.Pin = LCD_EN_Pin|LCD_RS_Pin|LCD_D4_Pin|LCD_D5_Pin
                          |LCD_D6_Pin|LCD_D7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : bbt_Pin */
  GPIO_InitStruct.Pin = bbt_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(bbt_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : Brl_Pin PB15 */
  GPIO_InitStruct.Pin = Brl_Pin|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : R1_Pin R2_Pin R3_Pin R4_Pin */
  GPIO_InitStruct.Pin = R1_Pin|R2_Pin|R3_Pin|R4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : C1_Pin C2_Pin C3_Pin C4_Pin */
  GPIO_InitStruct.Pin = C1_Pin|C2_Pin|C3_Pin|C4_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/*
		ham quet ban phim 
*/
void matrix(void)
{
	//-------------quet hang 1 
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_SET);
		HAL_Delay(1);
	// -- so 7 	
		if (!HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin))
		{   
			lcd_puts(1,row, (int8_t*) "7");
			numkey = 1;
			if(flag_gia == 1)
			{
				gia = gia * 10 + 7;
			}else if (flag_tien == 1)
			{
				tien = tien *10 +7;
			}else if (flag_lit == 1)
			{
				lit = lit *10 +7;
			}
			row++;
		}
		// -- so 9
		if (!HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin)){
			
			lcd_puts(1,row, (int8_t*) "8");
			//lcd_print(2,row,"8");
			numkey = 2;
			if(flag_gia == 1)
			{
				gia = gia * 10 + 8;
			}else if (flag_tien == 1)
			{
				tien = tien *10 +8;
			}else if (flag_lit == 1)
			{
				lit = lit *10 +8;
			}	
			row++;
		}
		// -- so 9
		if (!HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin)){
			lcd_puts(1,row, (int8_t*) "9");
			row++;
			numkey = 3;
			if(flag_gia == 1)
			{
				gia = gia * 10 + 9;
			}else if (flag_tien == 1)
			{
				tien = tien *10 +9;
			}else if (flag_lit == 1)
			{
				lit = lit *10 +9;
			}
		}
		// -- so L
		if (!HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin)){
			lcd_clear();
			row = 0;
			lcd_puts(0,0,(int8_t *) "Lit");
			numkey = 4;
			flag_lit = 1;
			flag_gia =	255;
			flag_tien = 255;
			tien = 0;
		}
		HAL_GPIO_WritePin(C1_GPIO_Port, C1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(C2_GPIO_Port, C2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(C3_GPIO_Port, C3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(C4_GPIO_Port, C4_Pin, GPIO_PIN_SET);
		
		//---------------quet hang 2 
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_SET);
		HAL_Delay(1);
		// -- so 4 
		if (!HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin)){
			lcd_puts(1,row, (int8_t*) "4");
			row++;
			numkey = 5;
			if(flag_gia == 1)
			{
				gia = gia * 10 + 4;
			}else if (flag_tien == 1)
			{
				tien = tien *10 +4;
			}else if (flag_lit == 1)
			{
				lit = lit *10 +4;
			}
		}
		// -- so 5 
		if (!HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin)){
			lcd_puts(1,row, (int8_t*) "5");
			row++;
			numkey = 6;
			if(flag_gia == 1)
			{
				gia = gia * 10 + 5;
			}else if (flag_tien == 1)
			{
				tien = tien *10 +5;
			}else if (flag_lit == 1)
			{
				lit = lit *10 +5;
			}
		}
		// -- so 6 
		if (!HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin)){
			lcd_puts(1,row, (int8_t*) "6");
			row++;
			numkey = 7;			
			if(flag_gia == 1)
			{
				gia = gia * 10 + 6;
			}else if (flag_tien == 1)
			{
				tien = tien *10 +6;
			}else if (flag_lit == 1)
			{
				lit = lit *10 +6;
			}
		}
		// -- so T
		if (!HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin)){
			lcd_clear();
			lcd_puts(0,0, (int8_t*) "Tien");
			row = 0;
			numkey = 8;
			flag_tien = 1;
			flag_gia =255;
			flag_lit = 255;
			lit =0;
		}
		HAL_GPIO_WritePin(C1_GPIO_Port, C1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(C2_GPIO_Port, C2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(C3_GPIO_Port, C3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(C4_GPIO_Port, C4_Pin, GPIO_PIN_SET);
		
		// ---------quet hang 3
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_SET);
		HAL_Delay(1);
		// -- so 1 
		if (!HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin)){
			lcd_puts(1,row, (int8_t*) "1");
			row++;
			numkey = 9;
			if(flag_gia == 1)
			{
				gia = gia * 10 + 1;
			}else if (flag_tien == 1)
			{
				tien = tien *10 +1;
			}else if (flag_lit == 1)
			{
				lit = lit *10 +1;
			}
		}
		// -- so 2
		if (!HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin)){
			lcd_puts(1,row, (int8_t*) "2");
			row++;
			numkey = 10;	
			if(flag_gia == 1)
			{
				gia = gia * 10 + 2;
			}else if (flag_tien == 1)
			{
				tien = tien *10 +2;
			}else if (flag_lit == 1)
			{
				lit = lit *10 +2;
			}
		}
		// -- so 3 
		if (!HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin)){
			lcd_puts(1,row, (int8_t*) "3");
			row++;
			numkey = 11;	
			if(flag_gia == 1)
			{
				gia = gia * 10 + 3;
			}else if (flag_tien == 1)
			{
				tien = tien *10 +3;
			}else if (flag_lit == 1)
			{
				lit = lit *10 +3;
			}			
		}
		// -- so G 
		if (!HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin)){
			lcd_clear();
			row = 0;
			lcd_puts(0,0, (int8_t*) "Gia");
			numkey = 12;	
			flag_gia = 1;
			flag_lit = 255;
			flag_tien = 255;
		}
		
		HAL_GPIO_WritePin(C1_GPIO_Port, C1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(C2_GPIO_Port, C2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(C3_GPIO_Port, C3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(C4_GPIO_Port, C4_Pin, GPIO_PIN_SET);
		
		//----------quet hang 4
		
		HAL_GPIO_WritePin(R1_GPIO_Port, R1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R2_GPIO_Port, R2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R3_GPIO_Port, R3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(R4_GPIO_Port, R4_Pin, GPIO_PIN_RESET);
		HAL_Delay(1);
		// -- so . ( chua dung) 
		if (!HAL_GPIO_ReadPin(C1_GPIO_Port, C1_Pin)){
			lcd_puts(1,row, (int8_t*) ".");
			row++;
			numkey = 13;		
		}
		// -- so 0
		if (!HAL_GPIO_ReadPin(C2_GPIO_Port, C2_Pin)){
			lcd_puts(1,row, (int8_t*) "0");
			row++;
			numkey = 14;		
			if(flag_gia == 1)
			{
				gia = gia * 10;
			}else if (flag_tien == 1)
			{
				tien = tien *10;
			}else if (flag_lit == 1)
			{
				lit = lit *10;
			}
		}
		// --so OK
		if (!HAL_GPIO_ReadPin(C3_GPIO_Port, C3_Pin)){
			lcd_clear();
			numkey = 15;	// phim OK		
			if(flag_gia == 1)
			{
				flag_gia = 0;
			}
			if(flag_lit == 1)
			{
				flag_lit = 0;
			}
			if(flag_tien == 1)
			{
				flag_tien = 0;
			}
		}
		// -- so ESC 
		if (!HAL_GPIO_ReadPin(C4_GPIO_Port, C4_Pin)){
			lcd_clear();
			numkey = 16;  // Phim ESC - Cancel het cac flag
			lit = 0;
			gia = 0;
			tien = 0;
			flag_gia = 255;
			flag_lit = 255;
			flag_tien = 255;	
			lcd_puts(0,0,(int8_t*) "Nhap Gia 1 Lit");			
		}
		HAL_GPIO_WritePin(C1_GPIO_Port, C1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(C2_GPIO_Port, C2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(C3_GPIO_Port, C3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(C4_GPIO_Port, C4_Pin, GPIO_PIN_SET);
		
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
