/**
  ******************************************************************************
  * @file    bap_adc.c
  * @author  JPT
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   ADC初始化
  ******************************************************************************
  */
#include "bsp_adc.h"
#include <stdint.h>

__IO uint32_t g_usPowerValue;
uint32_t g_ulaTempBuf[TEMP_NUM] = {0};

/*
*********************************************************************************************************
*	函 数 名: Power_ADC_GPIO_Init
*	功能说明: Power_ADC_GPIO_Init GPIO初始化  其中包含功率监控  与 功率采集
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Power_ADC_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // 使能 GPIO 时钟
  RCC_AHB1PeriphClockCmd(POWER_ADC_GPIO_CLK|
                          POWER_PD_GPIO_CLK, ENABLE);

  // 功率采集 配置 IO
  GPIO_InitStructure.GPIO_Pin = POWER_ADC_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
  GPIO_Init(POWER_ADC_GPIO_PORT, &GPIO_InitStructure);

  //功率监控
  GPIO_InitStructure.GPIO_Pin = POWER_PD_GPIO_PIN;
  GPIO_Init(POWER_PD_GPIO_PORT, &GPIO_InitStructure);

}
/*
*********************************************************************************************************
*	函 数 名: Power_ADC_Mode_Init
*	功能说明: 功率采集ADC初始化
*           ADC1使用DMA2，数据流0，通道0，这个是手册固定死的
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Power_ADC_Mode_Init(void)
{
  DMA_InitTypeDef DMA_InitStructure;
	ADC_InitTypeDef ADC_InitStructure;
	ADC_CommonInitTypeDef ADC_CommonInitStructure;
  // ------------------DMA Init 结构体参数 初始化--------------------------
  // 开启DMA时钟
  RCC_AHB1PeriphClockCmd(POWER_ADC_DMA_CLK, ENABLE);
  DMA_InitStructure.DMA_PeripheralBaseAddr = POWER_ADC_DR_ADDR;  // 外设基址为：ADC 数据寄存器地址 ADC1->DR
  DMA_InitStructure.DMA_Memory0BaseAddr = (u32)&g_usPowerValue;  // 存储器地址，实际上就是一个内部SRAM的变量
  DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralToMemory;	          //数据传输方向为外设到存储器
  DMA_InitStructure.DMA_BufferSize = 1;                             // 缓冲区大小为，指一次传输的数据量
  DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;  // 外设寄存器只有一个，地址不用递增
  DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;          // 存储器地址固定
  DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;   //设数据大小为半字，即两个字节
  DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;         //存储器数据大小也为半字，跟外设数据大小相同
  DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;                     // 循环传输模式
  DMA_InitStructure.DMA_Priority = DMA_Priority_High;             // DMA 传输通道优先级为高，当使用一个DMA通道时，优先级设置不影响
  DMA_InitStructure.DMA_FIFOMode = DMA_FIFOMode_Disable;        // 禁止DMA FIFO	，使用直连模式
  DMA_InitStructure.DMA_FIFOThreshold = DMA_FIFOThreshold_HalfFull; // FIFO 大小，FIFO模式禁止时，这个不用配置
  DMA_InitStructure.DMA_MemoryBurst = DMA_MemoryBurst_Single;
  DMA_InitStructure.DMA_PeripheralBurst = DMA_PeripheralBurst_Single;
  DMA_InitStructure.DMA_Channel = POWER_ADC_DMA_CHANNEL;         // 选择 DMA 通道，通道存在于流中
  
  //初始化DMA流，流相当于一个大的管道，管道里面有很多通道
  DMA_Init(POWER_ADC_DMA_STREAM, &DMA_InitStructure);

  // 使能DMA流
  DMA_Cmd(POWER_ADC_DMA_STREAM, ENABLE);

  // 开启ADC时钟
  RCC_APB2PeriphClockCmd(POWER_ADC_CLK , ENABLE);

  // -------------------ADC Init 结构体 参数 初始化--------------------------
  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;          // ADC 分辨率
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;                   // 禁止扫描模式，多通道采集才需要
  ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;              // 连续转换	 开启一次即可
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;  //禁止外部边沿触发
  //使用软件触发，外部触发不用配置，注释掉即可
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;         //数据右对齐
  ADC_InitStructure.ADC_NbrOfConversion = 1;                     //转换通道 1个

  //初始化ADC
  ADC_Init(POWER_ADC, &ADC_InitStructure);

  // -------------------ADC Common 结构体 参数 初始化------------------------
  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;                          // 独立ADC模式
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div2;                       // 时钟为fpclk x分频
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;           // 禁止DMA直接访问模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_20Cycles;     // 采样时间间隔
  ADC_CommonInit(&ADC_CommonInitStructure);

  // 配置 ADC 通道转换顺序为1，第一个转换，采样时间为3个时钟周期
  ADC_RegularChannelConfig(POWER_ADC, POWER_ADC_CHANNEL, 1, ADC_SampleTime_56Cycles);

  // 使能DMA请求 after last transfer (Single-ADC mode)
  ADC_DMARequestAfterLastTransferCmd(POWER_ADC, ENABLE);

  // 使能ADC DMA
  ADC_DMACmd(POWER_ADC, ENABLE);

  // 使能ADC
  ADC_Cmd(POWER_ADC, ENABLE);

  //开始adc转换，软件触发
  ADC_SoftwareStartConv(POWER_ADC);
}
/*
*********************************************************************************************************
*	函 数 名: Power_ADC_Init
*	功能说明: 功率ADC初始化
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void Power_ADC_Init(void)
{
  Power_ADC_GPIO_Init();
  Power_ADC_Mode_Init();
}
/*
*********************************************************************************************************
*	函 数 名: Temperature_ADC_GPIO_Init
*	功能说明: Temperature_ADC_GPIO_Init GPIO初始化
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void Temperature_ADC_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  // 使能 GPIO 时钟
  RCC_AHB1PeriphClockCmd(MUX_S0_GPIO_CLK|
                         MUX_S1_GPIO_CLK|
                         MUX1_GPIO_CLK, ENABLE);

  // 配置 IO  模拟输入
  GPIO_InitStructure.GPIO_Pin = MUX1_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL ; //不上拉不下拉
  GPIO_Init(MUX1_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = MUX2_GPIO_PIN;
  GPIO_Init(MUX2_GPIO_PORT, &GPIO_InitStructure);

  //熔点盒温度 TEMP17
  GPIO_InitStructure.GPIO_Pin = TEMP17_GPIO_PIN;
  GPIO_Init(TEMP17_GPIO_PORT, &GPIO_InitStructure);

  //推挽输出
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Pin = MUX_S0_GPIO_PIN;
  GPIO_Init(MUX_S0_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = MUX_S1_GPIO_PIN;
  GPIO_Init(MUX_S1_GPIO_PORT, &GPIO_InitStructure);

  GPIO_InitStructure.GPIO_Pin = MUX_S2_GPIO_PIN;
  GPIO_Init(MUX_S2_GPIO_PORT, &GPIO_InitStructure);

}
/*
*********************************************************************************************************
*	函 数 名: ADC3_Init
*	功能说明: ADC3_Init 初始化
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void ADC3_Init(void)
{
  ADC_CommonInitTypeDef ADC_CommonInitStructure;
  ADC_InitTypeDef       ADC_InitStructure;
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC3, ENABLE); //使能ADC3时钟

  RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,ENABLE);	  //ADC1复位
	RCC_APB2PeriphResetCmd(RCC_APB2Periph_ADC3,DISABLE);	//复位结束

  ADC_CommonInitStructure.ADC_Mode = ADC_Mode_Independent;//独立模式
  ADC_CommonInitStructure.ADC_TwoSamplingDelay = ADC_TwoSamplingDelay_5Cycles;  //两个采样阶段之间的延迟5个时钟
  ADC_CommonInitStructure.ADC_DMAAccessMode = ADC_DMAAccessMode_Disabled;    //DMA失能
  ADC_CommonInitStructure.ADC_Prescaler = ADC_Prescaler_Div4;         //预分频4分频。ADCCLK=PCLK2/4=84/4=21Mhz,ADC时钟最好不要超过36Mhz
  ADC_CommonInit(&ADC_CommonInitStructure);   //初始化

  ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;//12位模式
  ADC_InitStructure.ADC_ScanConvMode = DISABLE;//非扫描模式
  ADC_InitStructure.ADC_ContinuousConvMode = DISABLE;//关闭连续转换
  ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;//禁止触发检测，使用软件触发
	  //使用软件触发，外部触发不用配置，注释掉即可
  ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
  ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;//右对齐
  ADC_InitStructure.ADC_NbrOfConversion = 1;//1个转换在规则序列中 也就是只转换规则序列1
  ADC_Init(ADC3, &ADC_InitStructure);//ADC初始化

  ADC_Cmd(ADC3, ENABLE);//开启AD转换器

}
/*
*********************************************************************************************************
*	函 数 名: Get_Adc
*	功能说明: 获得ADC值
*	形    参:  ch: ADC_Channel_0~ADC_Channel_16
*	返 回 值:转换结果
*********************************************************************************************************
*/
uint16_t Get_Adc3(uint8_t ch)
{
	  	//设置指定ADC的规则组通道，一个序列，采样时间
	ADC_RegularChannelConfig(ADC3, ch, 1, ADC_SampleTime_480Cycles );	//ADC1,ADC通道,480个周期,提高采样时间可以提高精确度

	ADC_SoftwareStartConv(ADC3);		//使能指定的ADC1的软件转换启动功能

	while(!ADC_GetFlagStatus(ADC3, ADC_FLAG_EOC ));//等待转换结束

	return ADC_GetConversionValue(ADC3);	//返回最近一次ADC1规则组的转换结果
}

/*
*********************************************************************************************************
*	函 数 名: Get_Power_PD_Valtage
*	功能说明: 获得功率监控PD电压值
*	形    参:  无
*	返 回 值: uint16_t 功率监控电压AD值
*********************************************************************************************************
*/
uint16_t Get_Power_PD_Valtage(void)
{
  return Get_Adc3(ADC_Channel_9);
}
/*
*********************************************************************************************************
*	函 数 名: Get_Adc3_Average
*	功能说明: 获得ADC值 取平均
*	形    参:  ch: ADC_Channel_0~ADC_Channel_16
*	返 回 值:转换结果
*********************************************************************************************************
*/
uint16_t Get_Adc3_Average(uint8_t ch,uint8_t times)
{
	u32 temp_val=0;
	u8 t;
	for(t=0;t<times;t++)
	{
		temp_val+=Get_Adc3(ch);
		delay_ms(5);
	}
	return temp_val/times;
}

/*
*********************************************************************************************************
*	函 数 名: Get_16_Temp
*	功能说明: 得到16路温度值，通过设置3-8译码器电路来选择对应的通道
*           MUX1 --> ADC3_Ch5
*           MUX2 --> ADC3_CH6
*           MUX3 --> ADC_CH4
*	形    参: 无
*	返 回 值: 无
*********************************************************************************************************
*/
void Get_16_Temp(void)
{
    static u8 index = 0;

    if (index & 1) AD_S0_H();
    else AD_S0_L();
    if (index & 2) AD_S1_H();
    else AD_S1_L();
    if (index & 4) AD_S2_H();
    else AD_S2_L();

    delay_ms(3);
    g_ulaTempBuf[index] = Get_Adc3(ADC_Channel_5);
    delay_ms(2);
    g_ulaTempBuf[index+8] = Get_Adc3(ADC_Channel_6);
  	++index;
  	index = index%8;
}
/*
*********************************************************************************************************
*	函 数 名: Temperature_ADC_Init
*	功能说明: 温度ADC初始化
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void Temperature_ADC_Init(void)
{
  Temperature_ADC_GPIO_Init();
  ADC3_Init();
}
