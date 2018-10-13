#ifndef __LED_H
#define	__LED_H

#include "stm32f4xx.h"

/*
RUN_LED --> PG2
ERR_LED --> PD15
STATE1 --> PD14
STATE2 --> PD13

*/
//引脚定义
/*******************************************************/
//LED1
#define LED1_PIN                  GPIO_Pin_2
#define LED1_GPIO_PORT            GPIOG
#define LED1_GPIO_CLK             RCC_AHB1Periph_GPIOG

//LED2
#define LED2_PIN                  GPIO_Pin_15
#define LED2_GPIO_PORT            GPIOD
#define LED2_GPIO_CLK             RCC_AHB1Periph_GPIOD

//LED3
#define LED3_PIN                  GPIO_Pin_14
#define LED3_GPIO_PORT            GPIOD
#define LED3_GPIO_CLK             RCC_AHB1Periph_GPIOD

//LED4
#define LED4_PIN                  GPIO_Pin_13
#define LED4_GPIO_PORT            GPIOD
#define LED4_GPIO_CLK             RCC_AHB1Periph_GPIOD

/************************************************************/

#define LED1_Out()  PGout(2)   //LED1
#define LED2_Out()  PDout(15)   //LED2
#define LED3_Out()  PDout(14) //LED3
#define LED4_Out()  PDout(13) //LED4
/************************************************************/

void LED_GPIO_Config(void);

#endif /* __LED_H */
