/**
  ******************************************************************************
  * File Name          : USART.c
  * Description        : This file provides code for the configuration
  *                      of the USART instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usart.h"

/* USER CODE BEGIN 0 */
UART_T xtUart1, xtUart3;
/* USER CODE END 0 */

UART_HandleTypeDef huart1;
DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart1_tx;

/* USART1 init function */

void MX_USART1_UART_Init(void)
{

  huart1.Instance = USART1;
  huart1.Init.BaudRate = 115200;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart1) != HAL_OK)
  {
    Error_Handler();
  }

}

void HAL_UART_MspInit(UART_HandleTypeDef* uartHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspInit 0 */

  /* USER CODE END USART1_MspInit 0 */
    /* USART1 clock enable */
    __HAL_RCC_USART1_CLK_ENABLE();
  
    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**USART1 GPIO Configuration    
    PC4     ------> USART1_TX
    PC5     ------> USART1_RX 
    */
    GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    GPIO_InitStruct.Alternate = GPIO_AF7_USART1;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* USART1 DMA Init */
    /* USART1_RX Init */
    hdma_usart1_rx.Instance = DMA1_Channel5;
    hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
    hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_rx.Init.Mode = DMA_NORMAL;
    hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmarx,hdma_usart1_rx);

    /* USART1_TX Init */
    hdma_usart1_tx.Instance = DMA1_Channel4;
    hdma_usart1_tx.Init.Direction = DMA_MEMORY_TO_PERIPH;
    hdma_usart1_tx.Init.PeriphInc = DMA_PINC_DISABLE;
    hdma_usart1_tx.Init.MemInc = DMA_MINC_ENABLE;
    hdma_usart1_tx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
    hdma_usart1_tx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
    hdma_usart1_tx.Init.Mode = DMA_NORMAL;
    hdma_usart1_tx.Init.Priority = DMA_PRIORITY_LOW;
    if (HAL_DMA_Init(&hdma_usart1_tx) != HAL_OK)
    {
      Error_Handler();
    }

    __HAL_LINKDMA(uartHandle,hdmatx,hdma_usart1_tx);

    /* USART1 interrupt Init */
    HAL_NVIC_SetPriority(USART1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspInit 1 */

  /* USER CODE END USART1_MspInit 1 */
  }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef* uartHandle)
{

  if(uartHandle->Instance==USART1)
  {
  /* USER CODE BEGIN USART1_MspDeInit 0 */

  /* USER CODE END USART1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_USART1_CLK_DISABLE();
  
    /**USART1 GPIO Configuration    
    PC4     ------> USART1_TX
    PC5     ------> USART1_RX 
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_4|GPIO_PIN_5);

    /* USART1 DMA DeInit */
    HAL_DMA_DeInit(uartHandle->hdmarx);
    HAL_DMA_DeInit(uartHandle->hdmatx);

    /* USART1 interrupt Deinit */
    HAL_NVIC_DisableIRQ(USART1_IRQn);
  /* USER CODE BEGIN USART1_MspDeInit 1 */

  /* USER CODE END USART1_MspDeInit 1 */
  }
} 

/* USER CODE BEGIN 1 */
#ifdef __GNUC__

/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
 set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else

#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

PUTCHAR_PROTOTYPE
{
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xFFFF);
  return ch;
}

//DMA发送函数
void Usart1SendData_DMA(uint8_t *pdata, uint16_t Length)
{
  if (Length == 0)
    return;
  while (xtUart1.Send == USART_DMA_SENDING)
  {
  }
  xtUart1.Send = USART_DMA_SENDING;
  HAL_UART_Transmit_DMA(&huart1, pdata, Length);
}

// void Usart3SendData_DMA(uint8_t *pdata, uint16_t Length)
// {
// 	if(Length == 0 || xtUart3.Send == USART_DMA_SENDING)
// 		return;
// 	while(xtUart3.Send == USART_DMA_SENDING){}
// 	TXMODE_485();
// 	xtUart3.Send = USART_DMA_SENDING;
// 	HAL_UART_Transmit_DMA(&huart3, pdata, Length);
// }

//DMA发送完成中断回调函数
void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
  __HAL_DMA_DISABLE(huart->hdmatx);

  if (huart->Instance == huart1.Instance)
    xtUart1.Send = USART_DMA_SENDOVER;
  // else if(huart->Instance == huart3.Instance)
  // {
  // 	xtUart3.Send = USART_DMA_SENDOVER;
  // }
}

static void HAL_UART_DMAStopRX(UART_HandleTypeDef *huart)
{
  CLEAR_BIT(huart->Instance->CR3, USART_CR3_DMAR);
  HAL_DMA_Abort(huart->hdmarx);
  CLEAR_BIT(huart->Instance->CR1, (USART_CR1_RXNEIE | USART_CR1_PEIE));
  CLEAR_BIT(huart->Instance->CR3, USART_CR3_EIE);
  /* At end of Rx process, restore huart->RxState to Ready */
  huart->RxState = HAL_UART_STATE_READY;
}

//串口接收空闲中断
void UsartReceive_IDLE(UART_HandleTypeDef *huart)
{
  if ((__HAL_UART_GET_FLAG(huart, UART_FLAG_IDLE) != RESET))
  {
    //		HAL_UART_DMAStop(huart);

    HAL_UART_DMAStopRX(huart);
    __HAL_UART_CLEAR_IDLEFLAG(huart);
    if (huart->Instance == huart1.Instance)
    {
      xtUart1.RxLen = RECEIVELEN - __HAL_DMA_GET_COUNTER(huart->hdmarx);
      xtUart1.Receive = 1;
      HAL_UART_Receive_DMA(&huart1, xtUart1.RxBuf, RECEIVELEN);
    }
    // else if(huart->Instance == huart3.Instance)
    // {
    // 	xtUart3.RxLen =  RECEIVELEN - __HAL_DMA_GET_COUNTER(huart->hdmarx);
    // 	xtUart3.Receive=1;
    // 	HAL_UART_Receive_DMA(&huart3,xtUart3.RxBuf,RECEIVELEN);
    // }
  }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
  //	__HAL_UART_GET_IT_SOURCE(huart,UART_IT_ERR);

  __HAL_UART_CLEAR_PEFLAG(huart);

  //UART_FLAG_ORE/UART_FLAG_NE/UART_FLAG_FE/UART_FLAG_PE
  //	if(__HAL_UART_GET_FLAG(huart,UART_FLAG_ORE))
  //		__HAL_UART_CLEAR_OREFLAG(huart);
  //	if(__HAL_UART_GET_FLAG(huart,UART_FLAG_NE))
  //		__HAL_UART_CLEAR_NEFLAG(huart);
  //	if(__HAL_UART_GET_FLAG(huart,UART_FLAG_FE))
  //		__HAL_UART_CLEAR_FEFLAG(huart);
  //	if(__HAL_UART_GET_FLAG(huart,UART_FLAG_PE))
  //		__HAL_UART_CLEAR_PEFLAG(huart);
}

/* USER CODE END 1 */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
