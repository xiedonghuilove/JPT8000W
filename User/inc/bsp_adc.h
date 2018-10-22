#ifndef __BSP_ADC_H
#define __BSP_ADC_H

#include "stm32f4xx.h"
#include "bsp_sys.h"

// 功率采集 ADC GPIO 宏定义  PB1 --> ADC12_CH9
#define POWER_ADC_GPIO_PORT    GPIOB
#define POWER_ADC_GPIO_PIN     GPIO_Pin_1
#define POWER_ADC_GPIO_CLK     RCC_AHB1Periph_GPIOB

// 功率监控 GPIO 宏定义  PF3 --> ADC3_CH9
#define POWER_PD_GPIO_PORT    GPIOF
#define POWER_PD_GPIO_PIN     GPIO_Pin_3
#define POWER_PD_GPIO_CLK     RCC_AHB1Periph_GPIOF

// ADC 序号宏定义
#define POWER_ADC              ADC1
#define POWER_ADC_CLK          RCC_APB2Periph_ADC1
#define POWER_ADC_CHANNEL      ADC_Channel_9

// ADC DR寄存器宏定义，ADC转换后的数字值则存放在这里
#define POWER_ADC_DR_ADDR    ((u32)ADC1+0x4c)

// ADC DMA 通道宏定义，这里我们使用DMA传输
#define POWER_ADC_DMA_CLK      RCC_AHB1Periph_DMA2
#define POWER_ADC_DMA_CHANNEL  DMA_Channel_0
#define POWER_ADC_DMA_STREAM   DMA2_Stream0

//采集16路温度 ADC初始化
// MUX_S0 --> PC12
// MUX_S1 --> PD0
// MUX_S1 --> PD1
#define MUX_S0_GPIO_PIN        GPIO_Pin_12
#define MUX_S0_GPIO_PORT       GPIOC
#define MUX_S0_GPIO_CLK        RCC_AHB1Periph_GPIOC

#define MUX_S1_GPIO_PIN        GPIO_Pin_0
#define MUX_S1_GPIO_PORT       GPIOD
#define MUX_S1_GPIO_CLK        RCC_AHB1Periph_GPIOD

#define MUX_S2_GPIO_PIN        GPIO_Pin_1
#define MUX_S2_GPIO_PORT       GPIOD
#define MUX_S2_GPIO_CLK        RCC_AHB1Periph_GPIOD

#define AD_S0_H()							PCout(12)=1
#define AD_S0_L()							PCout(12)=0

#define AD_S1_H()							PDout(0)=1
#define AD_S1_L()							PDout(0)=0

#define AD_S2_H()							PDout(1)=1
#define AD_S2_L()							PDout(1)=0

// MUX1 --> PF7 --> ADC3_IN5
// MUX2 --> PF8 --> ADC3_IN6
#define MUX1_GPIO_PIN        GPIO_Pin_7
#define MUX1_GPIO_PORT       GPIOF
#define MUX1_GPIO_CLK        RCC_AHB1Periph_GPIOF

#define MUX2_GPIO_PIN        GPIO_Pin_8
#define MUX2_GPIO_PORT       GPIOF
#define MUX2_GPIO_CLK        RCC_AHB1Periph_GPIOF

//熔点盒温度  PC2
#define TEMP17_GPIO_PIN        GPIO_Pin_2
#define TEMP17_GPIO_PORT       GPIOC
#define TEMP17_GPIO_CLK        RCC_AHB1Periph_GPIOC

#define BUF_LEN(buf)					sizeof(buf)/sizeof(*buf)

#define TEMP_NUM						16

extern uint32_t g_ulaTempBuf[TEMP_NUM];

/*****************************************************************
*
*	宏定义 温度与丝印温度对应一致

*	软件读取顺序和硬件对应如下：
	GD_TEMP_BUF[0] = TEMP2		GD_TEMP_BUF[8] = TEMP10
	GD_TEMP_BUF[1] = TEMP3		GD_TEMP_BUF[9] = TEMP11
	GD_TEMP_BUF[2] = TEMP4		GD_TEMP_BUF[10] = TEMP12
	GD_TEMP_BUF[3] = TEMP1		GD_TEMP_BUF[11] = TEMP9
	GD_TEMP_BUF[4] = TEMP8		GD_TEMP_BUF[12] = TEMP16
	GD_TEMP_BUF[5] = TEMP5		GD_TEMP_BUF[13] = TEMP13
	GD_TEMP_BUF[6] = TEMP7		GD_TEMP_BUF[14] = TEMP15
	GD_TEMP_BUF[7] = TEMP6		GD_TEMP_BUF[15] = TEMP14
*******************************************************************/
#define TEMP1 		g_ulaTempBuf[3]
#define TEMP2		  g_ulaTempBuf[0]
#define TEMP3		  g_ulaTempBuf[1]
#define TEMP4		  g_ulaTempBuf[2]
#define TEMP5		  g_ulaTempBuf[5]
#define TEMP6		  g_ulaTempBuf[7]
#define TEMP7		  g_ulaTempBuf[6]
#define TEMP8		  g_ulaTempBuf[4]
#define TEMP9		  g_ulaTempBuf[11]
#define TEMP10		g_ulaTempBuf[8]
#define TEMP11		g_ulaTempBuf[9]
#define TEMP12		g_ulaTempBuf[10]
#define TEMP13		g_ulaTempBuf[13]
#define TEMP14		g_ulaTempBuf[15]
#define TEMP15		g_ulaTempBuf[14]
#define TEMP16		g_ulaTempBuf[12]

#define GetRongDianHe_Temp()		      TEMP5
#define GetHeShuQi_Temp()			        TEMP1
#define GetHeShuQi_Water_Temp()		    TEMP2

void Power_ADC_Init(void);
void Temperature_ADC_Init(void);
void Get_16_Temp(void);
uint16_t Get_Power_PD_Valtage(void);
uint16_t Get_Adc3(uint8_t ch);
uint16_t Get_Adc3_Average(uint8_t ch,uint8_t times);

#endif
