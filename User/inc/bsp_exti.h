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

//引脚定义 水流量检测 FT1 --> PH15  沿触发
#define WATER_FLOW_FT1_GPIO_PIN                 GPIO_Pin_15
#define WATER_FLOW_FT1_EXTI_PORTSOURCE          EXTI_PortSourceGPIOH
#define WATER_FLOW_FT1_EXTI_PINSOURCE           EXTI_PinSource15
#define WATER_FLOW_FT1_EXTI_LINE                EXTI_Line15

enum WATER_FLOW_E
{
    WATER_FLOW_FT0 = 0,
    WATER_FLOW_FT1,
    WATER_FLOW_NUM    
};

#define WATER_FLOW_CLEAR(i)         if(i) tMasterData.QBHWaterFlow = 0;\
                                    else tMasterData.WaterFlow = 0;

#define WATER_ALARM(i)              if(i) tMasterData.AlarmLowBit |= ERROR_MASTER_QBHWATER;\
                                    else tMasterData.AlarmLowBit |= ERROR_MASTER_WATER;

void bspEXTI_Init(void);

/*******************************************************/

#endif
