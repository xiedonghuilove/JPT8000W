#ifndef __BSP_EXTI_H
#define __BSP_EXTI_H

#include "stm32f4xx.h"

//引脚定义  水流量检测  FT  --> PH14  沿触发
#define WATER_FLOW_GPIO_PORT                GPIOH
#define WATER_FLOW_GPIO_CLK                 RCC_AHB1Periph_GPIOH
#define WATER_FLOW_GPIO_PIN                 GPIO_Pin_14
#define WATER_FLOW_EXTI_PORTSOURCE          EXTI_PortSourceGPIOH
#define WATER_FLOW_EXTI_PINSOURCE           EXTI_PinSource14
#define WATER_FLOW_EXTI_LINE                EXTI_Line14
#define WATER_FLOW_EXTI_IRQ                 EXTI15_10_IRQn

#define WATER_FLOW_IRQHandler               EXTI15_10_IRQHandler



void bspEXTI_Init(void);

/*******************************************************/

#endif
