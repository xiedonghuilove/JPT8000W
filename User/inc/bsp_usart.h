#ifndef __BSP_USART_H
#define	__BSP_USART_H

#include "stm32f4xx.h"
#include <stdio.h>
#include "bsp_sys.h"


//引脚定义
/*
	RS232_RX  --> PD6 --> USART2_RX
	RS232_TX  --> PD5 --> USART2_TX

	DMA1_Stream6
	DMA_CHANNEL_4

*/
/*******************************************************/
#define RS232_USART                             USART2
#define RS232_USART_CLK                         RCC_APB1Periph_USART2
#define RS232_USART_BAUDRATE                    115200  //串口波特率

//DMA
#define RS232_USART_DR_BASE               (USART2_BASE+0x04)//USART2->DR寄存器
#define SENDBUFF_SIZE                     15				//发送的数据量
#define RS232_USART_DMA_CLK               RCC_AHB1Periph_DMA1
#define RS232_USART_DMA_CHANNEL           DMA_Channel_4
#define RS232_USART_DMA_STREAM            DMA1_Stream6

#define RS232_USART_RX_GPIO_PORT                GPIOD
#define RS232_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define RS232_USART_RX_PIN                      GPIO_Pin_6
#define RS232_USART_RX_AF                       GPIO_AF_USART2
#define RS232_USART_RX_SOURCE                   GPIO_PinSource6

#define RS232_USART_TX_GPIO_PORT                GPIOD
#define RS232_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOD
#define RS232_USART_TX_PIN                      GPIO_Pin_5
#define RS232_USART_TX_AF                       GPIO_AF_USART2
#define RS232_USART_TX_SOURCE                   GPIO_PinSource5

#define RS232_USART_IRQHandler                  USART2_IRQHandler
#define RS232_USART_IRQ                 		    USART2_IRQn
/************************************************************/
//引脚定义
/*
	RS485_RX  --> PC11 --> USART3_RX
	RS485_TX  --> PC10 --> USART3_TX

	RS485_EN --> PA15

	DMA1_Stream3
	DMA_CHANNEL_4

*/
/*******************************************************/
#define RS485_USART                             USART3
#define RS485_USART_CLK                         RCC_APB1Periph_USART3
#define RS485_USART_BAUDRATE                    115200  //串口波特率

//DMA
#define RS485_USART_DR_BASE               (USART3_BASE+0x04)//USART2->DR寄存器
#define SENDBUFF_485_SIZE                     15				//发送的数据量
#define RS485_USART_DMA_CLK               RCC_AHB1Periph_DMA1
#define RS485_USART_DMA_CHANNEL           DMA_Channel_4
#define RS485_USART_DMA_STREAM            DMA1_Stream3

#define RS485_USART_RX_GPIO_PORT                GPIOC
#define RS485_USART_RX_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define RS485_USART_RX_PIN                      GPIO_Pin_11
#define RS485_USART_RX_AF                       GPIO_AF_USART3
#define RS485_USART_RX_SOURCE                   GPIO_PinSource11

#define RS485_USART_TX_GPIO_PORT                GPIOC
#define RS485_USART_TX_GPIO_CLK                 RCC_AHB1Periph_GPIOC
#define RS485_USART_TX_PIN                      GPIO_Pin_10
#define RS485_USART_TX_AF                       GPIO_AF_USART3
#define RS485_USART_TX_SOURCE                   GPIO_PinSource10

#define RS485_USART_IRQHandler                  USART3_IRQHandler
#define RS485_USART_IRQ                 		USART3_IRQn

#define RS485_EN_GPIO_PORT                      GPIOA
#define RS485_EN_GPIO_CLK                       RCC_AHB1Periph_GPIOA
#define RS485_EN_PIN                            GPIO_Pin_15

#define RS485EN_RX()       PAout(15)=0
#define RS485EN_TX()       PAout(15)=1
/************************************************************/


void RS232_USART_Config(void);
void RS485_USART_Init(void);


#endif /* __USART1_H */
