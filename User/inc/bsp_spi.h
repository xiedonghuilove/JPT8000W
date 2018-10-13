#ifndef __BSP_SPI_
#define __BSP_SPI_

#include "stm32f4xx.h"

/*
**************SPI接口定义-开头***************************
  移植修改文件
  SD_CS  --> PB12  --> SPI2_NSS
  SD_CLK --> PB13  --> SPI2_SCK
  SD_MISO --> PB14  --> SPI2_MISO
  SD_MOSI --> PB15  --> SPI2_MOSI
********************************************************
*/
#define SPI                           SPI2
#define SPI_CLK                       RCC_APB1Periph_SPI2

#define SPI_SCK_PIN                   GPIO_Pin_13
#define SPI_SCK_GPIO_PORT             GPIOB
#define SPI_SCK_GPIO_CLK              RCC_AHB1Periph_GPIOB
#define SPI_SCK_PINSOURCE             GPIO_PinSource13
#define SPI_SCK_AF                    GPIO_AF_SPI2

#define SPI_MISO_PIN                  GPIO_Pin_14
#define SPI_MISO_GPIO_PORT            GPIOB
#define SPI_MISO_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define SPI_MISO_PINSOURCE            GPIO_PinSource14
#define SPI_MISO_AF                   GPIO_AF_SPI2

#define SPI_MOSI_PIN                  GPIO_Pin_15
#define SPI_MOSI_GPIO_PORT            GPIOB
#define SPI_MOSI_GPIO_CLK             RCC_AHB1Periph_GPIOB
#define SPI_MOSI_PINSOURCE            GPIO_PinSource15
#define SPI_MOSI_AF                   GPIO_AF_SPI2

#define SPI_CS_PIN                        GPIO_Pin_12
#define SPI_CS_GPIO_PORT                  GPIOB
#define SPI_CS_GPIO_CLK                   RCC_AHB1Periph_GPIOB


void Bsp_SPI_Init(SPI_TypeDef* SPIx);
void SPI_SetSpeed(SPI_TypeDef* SPIx,uint8_t SPI_BaudRatePrescaler);
uint8_t SPI_ReadWriteByte(SPI_TypeDef* SPIx,uint8_t TxData);

#endif
