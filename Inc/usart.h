/**
  ******************************************************************************
  * File Name          : USART.h
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __usart_H
#define __usart_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define RECEIVELEN 64
#define USART_DMA_SENDING 1//发送未完成
#define USART_DMA_SENDOVER 0//发送完成
	 
#define MODBUD_SENDMODE 1	 
	 
typedef struct
{
	uint8_t Receive:1;//空闲接收标记
	uint8_t Send:1;//发送完成标记
	uint16_t RxLen;//接收长度
	uint8_t RxBuf[RECEIVELEN];//DMA接收缓存
}UART_T;

extern UART_T xtUart1,xtUart3;
/* USER CODE END Private defines */

void MX_USART1_UART_Init(void);

/* USER CODE BEGIN Prototypes */
void Usart1SendData_DMA(uint8_t *pdata, uint16_t Length);
void Usart3SendData_DMA(uint8_t *pdata, uint16_t Length);
void UsartReceive_IDLE(UART_HandleTypeDef *huart);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ usart_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
