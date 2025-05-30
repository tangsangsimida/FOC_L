/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "crc.h"
#include "dma.h"
#include "i2c.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */


#include "as5600.h"
#include "ts_foc.h"
#include "lowpass_filter.h"
#include "pid.h"
#include "inlinecurrent.h"
#include "user_api.h"
#include "get_cmd.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
//Serial port redirection
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1,2000);
    //重定向到对应的串口即可
    return ch;
}

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

float target_angle = 0; //目标角度
float target_torque = 0; //目标力矩
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
  MX_DMA_Init();
  MX_CRC_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_ADC1_Init();
  MX_TIM3_Init();
  MX_TIM2_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */


  // HAL_ADC_Start_DMA(&hadc1, (uint32_t*)&adc_buffer, 1); // Start ADC DMA

  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1); // Start PWM timer
  HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_3); // Start PWM timer  
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_1); // Start PWM timer
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2); // Start PWM timer
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_3); // Start PWM timer
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_4); // Start PWM timer




  Motor * motor1;
  motor1 = (Motor *)malloc(sizeof(Motor) * 1);
  motor1->Motor_ID = 1;
  motor1->PP = 7;  
  motor1->DIR = 1;
  motor1->As5600_Sensor = (As5600_Sensor_Typedef *)malloc(sizeof(As5600_Sensor_Typedef) * 1);
  motor1->As5600_Sensor->Rotations = 0;
  Read_AS5600_Angle(motor1->As5600_Sensor);
  motor1->Motor_Set_Compare1 = Motor1_Set_Compare1;
  motor1->Motor_Set_Compare2 = Motor1_Set_Compare2;
  motor1->Motor_Set_Compare3 = Motor1_Set_Compare3;
  motor1->voltage_power_supply = 12.0;
  motor1->zero_electric_angle = 0;
  motor1->Ualpha = motor1->Ubeta = motor1->Ua = motor1->Ub = motor1->Uc = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  Read_AS5600_Angle(motor1->As5600_Sensor);
  //计算0电角度->后续规整到电机初始化当中
  setTorque(motor1,2, _3PI_2);
  HAL_Delay(1000);  
  Read_AS5600_Angle(motor1->As5600_Sensor);
  motor1->zero_electric_angle = _electricalAngle(motor1);
  setTorque(motor1,0, _3PI_2);
  printf("0电角度:%d\r\n",(int)(motor1->zero_electric_angle*1000));
  HAL_Delay(1000);
  printf("init_ok\r\n");

  float Kp = 0.133; //位置环比例系数

  while (1)
  {
    // HAL_Delay(100);

    HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);

    Read_AS5600_Angle(motor1->As5600_Sensor);   // Read the angle of the AS5600 sensor

    // printf("Angle:%d,圈数:%d\r\n",(int)(Get_AS5600_Angle(motor1->As5600_Sensor)*1000),(int)(motor1->As5600_Sensor->Rotations));

    Get_Cmd(&huart1); //获取指令
    // setTorque(motor1, target_torque, _electricalAngle(motor1));   //力矩闭环
    setTorque(motor1,Kp*(target_angle-motor1->DIR*Get_AS5600_Angle(motor1->As5600_Sensor))*180./PI,_electricalAngle(motor1));   //位置闭环




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

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 99;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
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
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM11 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  /* USER CODE BEGIN Callback 0 */

  /* USER CODE END Callback 0 */
  if (htim->Instance == TIM11) {
    HAL_IncTick();
  }
  /* USER CODE BEGIN Callback 1 */

  /* USER CODE END Callback 1 */
}

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
