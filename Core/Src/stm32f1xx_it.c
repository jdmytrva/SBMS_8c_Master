/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    stm32f1xx_it.c
  * @brief   Interrupt Service Routines.
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
#include "stm32f1xx_it.h"
/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

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
/* USER CODE BEGIN PFP */
	void TIM7_Callback();
	void SysTick_Callback();
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/* External variables --------------------------------------------------------*/

/* USER CODE BEGIN EV */

/* USER CODE END EV */

/******************************************************************************/
/*           Cortex-M3 Processor Interruption and Exception Handlers          */
/******************************************************************************/
/**
  * @brief This function handles Non maskable interrupt.
  */
void NMI_Handler(void)
{
  /* USER CODE BEGIN NonMaskableInt_IRQn 0 */

  /* USER CODE END NonMaskableInt_IRQn 0 */
  /* USER CODE BEGIN NonMaskableInt_IRQn 1 */
  while (1)
  {
  }
  /* USER CODE END NonMaskableInt_IRQn 1 */
}

/**
  * @brief This function handles Hard fault interrupt.
  */
void HardFault_Handler(void)
{
  /* USER CODE BEGIN HardFault_IRQn 0 */

  /* USER CODE END HardFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_HardFault_IRQn 0 */
    /* USER CODE END W1_HardFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Memory management fault.
  */
void MemManage_Handler(void)
{
  /* USER CODE BEGIN MemoryManagement_IRQn 0 */

  /* USER CODE END MemoryManagement_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_MemoryManagement_IRQn 0 */
    /* USER CODE END W1_MemoryManagement_IRQn 0 */
  }
}

/**
  * @brief This function handles Prefetch fault, memory access fault.
  */
void BusFault_Handler(void)
{
  /* USER CODE BEGIN BusFault_IRQn 0 */

  /* USER CODE END BusFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_BusFault_IRQn 0 */
    /* USER CODE END W1_BusFault_IRQn 0 */
  }
}

/**
  * @brief This function handles Undefined instruction or illegal state.
  */
void UsageFault_Handler(void)
{
  /* USER CODE BEGIN UsageFault_IRQn 0 */

  /* USER CODE END UsageFault_IRQn 0 */
  while (1)
  {
    /* USER CODE BEGIN W1_UsageFault_IRQn 0 */
    /* USER CODE END W1_UsageFault_IRQn 0 */
  }
}

/**
  * @brief This function handles System service call via SWI instruction.
  */
void SVC_Handler(void)
{
  /* USER CODE BEGIN SVCall_IRQn 0 */

  /* USER CODE END SVCall_IRQn 0 */
  /* USER CODE BEGIN SVCall_IRQn 1 */

  /* USER CODE END SVCall_IRQn 1 */
}

/**
  * @brief This function handles Debug monitor.
  */
void DebugMon_Handler(void)
{
  /* USER CODE BEGIN DebugMonitor_IRQn 0 */

  /* USER CODE END DebugMonitor_IRQn 0 */
  /* USER CODE BEGIN DebugMonitor_IRQn 1 */

  /* USER CODE END DebugMonitor_IRQn 1 */
}

/**
  * @brief This function handles Pendable request for system service.
  */
void PendSV_Handler(void)
{
  /* USER CODE BEGIN PendSV_IRQn 0 */

  /* USER CODE END PendSV_IRQn 0 */
  /* USER CODE BEGIN PendSV_IRQn 1 */

  /* USER CODE END PendSV_IRQn 1 */
}

/**
  * @brief This function handles System tick timer.
  */
void SysTick_Handler(void)
{
  /* USER CODE BEGIN SysTick_IRQn 0 */

  /* USER CODE END SysTick_IRQn 0 */

  /* USER CODE BEGIN SysTick_IRQn 1 */

  /* USER CODE END SysTick_IRQn 1 */
}

/******************************************************************************/
/* STM32F1xx Peripheral Interrupt Handlers                                    */
/* Add here the Interrupt Handlers for the used peripherals.                  */
/* For the available peripheral interrupt handler names,                      */
/* please refer to the startup file (startup_stm32f1xx.s).                    */
/******************************************************************************/

/**
  * @brief This function handles DMA1 channel1 global interrupt.
  */
void DMA1_Channel1_IRQHandler(void)
{
  /* USER CODE BEGIN DMA1_Channel1_IRQn 0 */

  /* USER CODE END DMA1_Channel1_IRQn 0 */

  /* USER CODE BEGIN DMA1_Channel1_IRQn 1 */

  /* USER CODE END DMA1_Channel1_IRQn 1 */
}

/**
  * @brief This function handles USART1 global interrupt.
  */
void USART1_IRQHandler(void)
{
  /* USER CODE BEGIN USART1_IRQn 0 */

  /* USER CODE END USART1_IRQn 0 */
  /* USER CODE BEGIN USART1_IRQn 1 */
	if ( (USART1->SR & USART_SR_TXE) != 0 ) //if(LL_USART_IsActiveFlag_TXE(USART2)) //прерывание по передачи
	{
		if (bufferUart1.tx_counter > 0) //е�?ли е�?ть что передать
	    {
	      --bufferUart1.tx_counter; // уменьшаем количе�?тво не переданных данных
	      //LL_USART_TransmitData8(USART3,tx_buffer[tx_rd_index++]);
	      USART1->DR = bufferUart1.tx_buffer[bufferUart1.tx_rd_index++]; //передаем данные инкрементиру�? хво�?т буфера
	      if (bufferUart1.tx_rd_index == TX_BUFFER_SIZE) bufferUart1.tx_rd_index=0; //идем по кругу
	    }
	    else //е�?ли нечего передать, запрещаем прерывание по передачи
	    {
	    	USART1->CR1 &= ~USART_CR1_TXEIE;  // Interrupt Disable
	    	//LL_USART_DisableIT_TXE(USART2);
	    }
	}
  /* USER CODE END USART1_IRQn 1 */
}

/**
  * @brief This function handles USART3 global interrupt.
  */
void USART3_IRQHandler(void)
{
  /* USER CODE BEGIN USART3_IRQn 0 */
	//if ( (USART1->SR & USART_SR_RXNE) !=0 ) //Ждем по�?туплени�? данных от компьютера
	volatile uint8_t data;
	if (LL_USART_IsActiveFlag_RXNE(USART3))
	{
	  data = (uint8_t) (USART3->DR & 0xFF);
	  //
	  if ( (ParsingData.IsPassedPackageLengthFlag == 1)  )
	  {
		  //logDebug("PD");
		  //logDebugD("i=",ParsingData.i,0);
		  //logDebugD("Le=",ParsingData.Length,0)
		  //logDebugD("Data=",data,0);
		  if (ParsingData.i < ParsingData.Length-2)
		  {
			  ParsingData.i++;
			  bufferUart2.rx_buffer[bufferUart2.rx_wr_index++] = data; //�?читываем данные в буфер, инкрементиру�? хво�?т буфера

			  if (bufferUart2.rx_wr_index == RX_BUFFER_SIZE) bufferUart2.rx_wr_index=0; //идем по кругу
			  if (++bufferUart2.rx_counter == RX_BUFFER_SIZE) //переполнение буфера
			  {
				  bufferUart2.rx_counter=0; //начинаем �?начала (удал�?ем в�?е данные)
				  bufferUart2.rx_buffer_overflow=1;  //�?ообщаем о переполнении
			  }
		  }
		  else
		  {
			  //logDebug("else");
			  ParsingData.i = 0;
			  ParsingData.IsPassedPackageLengthFlag = 0;
			  ParsingData.IsPassedPackageBeginFlag = 0;
			  ParsingData.IsDataReadyReadFromBuffer = 1;
		  }
	  }
	  //
	  if ( (ParsingData.IsPassedPackageBeginFlag == 1) && (ParsingData.IsPassedPackageLengthFlag == 0) )
	  {
		  //logDebug("Length");
		  ParsingData.IsPassedPackageLengthFlag = 1;
		  bufferUart2.rx_buffer[bufferUart2.rx_wr_index++] = data; //�?читываем данные в буфер, инкрементиру�? хво�?т буфера
		  ParsingData.Length = data+1+1;

		  if (bufferUart2.rx_wr_index == RX_BUFFER_SIZE) bufferUart2.rx_wr_index=0; //идем по кругу
		  if (++bufferUart2.rx_counter == RX_BUFFER_SIZE) //переполнение буфера
		  {
			  bufferUart2.rx_counter=0; //начинаем �?начала (удал�?ем в�?е данные)
			  bufferUart2.rx_buffer_overflow=1;  //�?ообщаем о переполнении
		  }
	  }
	  //
	  if ( (data == PACKAGE_BEGIN) && (ParsingData.IsPassedPackageBeginFlag == 0) )
	  {
		  //logDebug("PB");
		  ParsingData.IsPassedPackageBeginFlag = 1;
		  bufferUart2.rx_buffer[bufferUart1.rx_wr_index++] = data; //�?читываем данные в буфер, инкрементиру�? хво�?т буфера

		  if (bufferUart2.rx_wr_index == RX_BUFFER_SIZE) bufferUart2.rx_wr_index=0; //идем по кругу
		  if (++bufferUart2.rx_counter == RX_BUFFER_SIZE) //переполнение буфера
		  {
			  bufferUart2.rx_counter=0; //начинаем �?начала (удал�?ем в�?е данные)
			  bufferUart2.rx_buffer_overflow=1;  //�?ообщаем о переполнении
		  }
	  }

	}

	if ( (USART3->SR & USART_SR_TXE) != 0 ) //if(LL_USART_IsActiveFlag_TXE(USART1)) //прерывание по передачи
	{
		vard2++;
		if (bufferUart2.tx_counter > 0) //е�?ли е�?ть что передать
	    {
	      --bufferUart2.tx_counter; // уменьшаем количе�?тво не переданных данных
	      //LL_USART_TransmitData8(USART1,tx_buffer[tx_rd_index++]);
	      USART3->DR = bufferUart2.tx_buffer[bufferUart2.tx_rd_index++]; //передаем данные инкрементиру�? хво�?т буфера
	      if (bufferUart2.tx_rd_index == TX_BUFFER_SIZE) bufferUart2.tx_rd_index=0; //идем по кругу
	    }
	    else //е�?ли нечего передать, запрещаем прерывание по передачи
	    {
	    	USART3->CR1 &= ~USART_CR1_TXEIE;  // Interrupt Disable
	    	//LL_USART_DisableIT_TXE(USART1);
	    }
	}
  /* USER CODE END USART3_IRQn 0 */
  /* USER CODE BEGIN USART3_IRQn 1 */

  /* USER CODE END USART3_IRQn 1 */
}

/**
  * @brief This function handles TIM7 global interrupt.
  */
void TIM7_IRQHandler(void)
{
  /* USER CODE BEGIN TIM7_IRQn 0 */
	  if(LL_TIM_IsActiveFlag_UPDATE(TIM7))
	  {
		LL_TIM_ClearFlag_UPDATE(TIM7);
		TIM7_Callback();
	  }
  /* USER CODE END TIM7_IRQn 0 */
  /* USER CODE BEGIN TIM7_IRQn 1 */

  /* USER CODE END TIM7_IRQn 1 */
}

/* USER CODE BEGIN 1 */

/* USER CODE END 1 */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
