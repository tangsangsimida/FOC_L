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
#include "can.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

#include "ts_foc.h"
#include "user_api.h"


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
    HAL_UART_Transmit(&huart1 , (uint8_t *)&ch, 1, 0xFFFF);
    return ch;
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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
float target_angle1 = 0,target_angle2 = 0;
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
  MX_ADC1_Init();
  MX_USART1_UART_Init();
  MX_CAN_Init();
  MX_SPI1_Init();
  MX_TIM2_Init();
  MX_TIM3_Init();
  MX_USB_PCD_Init();
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  HAL_ADCEx_Calibration_Start(&hadc1);
  HAL_ADC_Start_DMA(&hadc1,(uint32_t *)Samp_volts, 4); // 启动 ADC DMA 传输

  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_1);              //使能PWM
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_2);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_3);
  HAL_TIM_PWM_Start(&htim2,TIM_CHANNEL_4);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_1);
  HAL_TIM_PWM_Start(&htim3,TIM_CHANNEL_2);


  HAL_Delay(2000);

  Motor* Motor1 = (Motor*)malloc(sizeof(Motor)*1);
  Motor1->As5600_Sensor = (As5600_Sensor_Typedef*) malloc(sizeof(As5600_Sensor_Typedef)*1);
  Motor1->Pid_Vel = (Pid_Typedef*) malloc(sizeof(Pid_Typedef)*1);
  Motor1->Pid_Ang = (Pid_Typedef*) malloc(sizeof(Pid_Typedef)*1);
  Motor1->Pid_Curr = (Pid_Typedef*) malloc(sizeof(Pid_Typedef)*1);
  Motor1->Lowpass_Filter_Vel = (Lowpass_Filter_Typedef*) malloc(sizeof(Lowpass_Filter_Typedef)*1);
  Motor1->Lowpass_Filter_Curr = (Lowpass_Filter_Typedef*) malloc(sizeof(Lowpass_Filter_Typedef)*1);
  Motor1->Inlinecurrent = (Inlinecurrent_Typedef *) malloc(sizeof(Inlinecurrent_Typedef)*1);
  Motor1->Inlinecurrent->A = 0;
  Motor1->Inlinecurrent->B = 1;
  Motor1->Ualpha = Motor1->Ubeta = Motor1->Ua = Motor1->Ub = Motor1->Uc = 0;
  Motor1->Motor_ID = 1;
  Motor1->voltage_power_supply = 12.;
  Motor1->PP = 7;
  Motor1->DIR = 1;
  Motor1->zero_electric_angle = 0;
  Motor1->As5600_Sensor->Port = GPIOB;
  Motor1->As5600_Sensor->SCL_PIN = GPIO_PIN_10;
  Motor1->As5600_Sensor->SDA_PIN = GPIO_PIN_11;
  Motor1->Motor_Set_Compare1 = Motor1_Set_Compare1;
  Motor1->Motor_Set_Compare2 = Motor1_Set_Compare2;
  Motor1->Motor_Set_Compare3 = Motor1_Set_Compare3;
  Pid_Ang_Init(Motor1->Pid_Ang,1,0,0,100000,10);
  Pid_Vel_Init(Motor1->Pid_Vel,0.02,1,0,100000,0.1);
  Pid_Curr_Init(Motor1->Pid_Curr,5,200,0.00,100000,12.6);
  Lowpass_Filter_Init(Motor1->Lowpass_Filter_Vel, 0.01);
  Lowpass_Filter_Init(Motor1->Lowpass_Filter_Curr, 0.05);


  Motor* Motor2 = (Motor*)malloc(sizeof(Motor)*1);
  Motor2->As5600_Sensor = (As5600_Sensor_Typedef*) malloc(sizeof(As5600_Sensor_Typedef)*1);
  Motor2->Pid_Vel = (Pid_Typedef*) malloc(sizeof(Pid_Typedef)*1);
  Motor2->Pid_Ang = (Pid_Typedef*) malloc(sizeof(Pid_Typedef)*1);
  Motor2->Pid_Curr = (Pid_Typedef*) malloc(sizeof(Pid_Typedef)*1);
  Motor2->Lowpass_Filter_Vel = (Lowpass_Filter_Typedef*) malloc(sizeof(Lowpass_Filter_Typedef)*1);
  Motor2->Lowpass_Filter_Curr = (Lowpass_Filter_Typedef*) malloc(sizeof(Lowpass_Filter_Typedef)*1);
  Motor2->Inlinecurrent = (Inlinecurrent_Typedef *) malloc(sizeof(Inlinecurrent_Typedef)*1);
  Motor2->Inlinecurrent->A = 2;
  Motor2->Inlinecurrent->B = 3;
  Motor2->Ualpha = Motor2->Ubeta = Motor2->Ua = Motor2->Ub = Motor2->Uc = 0;
  Motor2->Motor_ID = 2;
  Motor2->voltage_power_supply = 12.;
  Motor2->PP = 7;
  Motor2->DIR = 1;
  Motor2->zero_electric_angle = 0;
  Motor2->As5600_Sensor->Port = GPIOB;
  Motor2->As5600_Sensor->SCL_PIN = GPIO_PIN_6;
  Motor2->As5600_Sensor->SDA_PIN = GPIO_PIN_7;
  Motor2->Motor_Set_Compare1 = Motor2_Set_Compare1;
  Motor2->Motor_Set_Compare2 = Motor2_Set_Compare2;
  Motor2->Motor_Set_Compare3 = Motor2_Set_Compare3;
  Pid_Ang_Init(Motor2->Pid_Ang,1,0,0,100000,10);
  Pid_Vel_Init(Motor2->Pid_Vel,0.02,1,0,100000,0.1);
  Pid_Curr_Init(Motor2->Pid_Curr,5,200,0,100000,12.6);
  Lowpass_Filter_Init(Motor2->Lowpass_Filter_Vel, 0.01);
  Lowpass_Filter_Init(Motor2->Lowpass_Filter_Curr, 0.05);
  
  HAL_Delay(1000);

  //初始化as5600
  AS5600_Init(Motor1->As5600_Sensor);
  AS5600_Init(Motor2->As5600_Sensor);
  CurrSense_Init(Motor1->Inlinecurrent);
  CurrSense_Init(Motor2->Inlinecurrent);


  aligns_Motor_Zero_Angle(Motor1);
  aligns_Motor_Zero_Angle(Motor2);

  printf("everythin_init_ok\r\n");



  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  HAL_Delay(1000);

  while (1)
  {

    AS5600_Sensor_update(Motor1->As5600_Sensor);
    AS5600_Sensor_update(Motor2->As5600_Sensor);
    Current_Update(Motor1->Inlinecurrent);
    Current_Update(Motor2->Inlinecurrent);
    Get_Cmd(&huart1);

    // Set_Velocity(Motor1,target_angle1);
    // Set_Velocity(Motor2,target_angle1);
    
    // Set_Force_Angle(Motor1,target_angle1);
    // Set_Force_Angle(Motor2,target_angle1);

    // setTorque_with_Curr(Motor1,target_angle1);
    // setTorque_with_Curr(Motor2,target_angle1);


    set_Velocity_Angle_Curr(Motor1,target_angle1);
    set_Velocity_Angle_Curr(Motor2,target_angle1);
      


      // 调试信息打印
      // printf("%.4f,%.4f\r\n",Motor1->Inlinecurrent->I_Q,Motor2->Inlinecurrent->I_Q);
      // printf("%d,%d,%d,%d\r\n",Samp_volts[0],Samp_volts[1],Samp_volts[2],Samp_volts[3]);
      printf("%.2f,%.2f,%.2f\r\n",target_angle1,AS5600_getAngle(Motor1->As5600_Sensor),AS5600_getAngle(Motor2->As5600_Sensor));





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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
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

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

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
