/**
  ******************************************************************************
  * @file    main.c
  * @author  JPT
  * @version V1.0
  * @date    2018-xx-xx
  ******************************************************************************
  */

#include "stm32f4xx.h"
#include "bsp_led.h"
#include "bsp_sys.h"
#include "stdio.h"
#include "bsp_usart.h"
#include "bsp_dma.h"
#include "bsp_publicIO.h"
#include "iic.h"
#include "pcf8563.h"
#include "ff.h"
#include "mmc_sd.h"
#include "bsp_sht10.h"
#include "bsp_heshuqi_sth10.h"
#include "bsp_adc.h"
#include "bsp_os.h"
#include "bsp_exti.h"
#include "data.h"

//extern uint16_t g_usPowerValue;

extern __IO uint16_t g_usPowerValue;

uint32_t g_ulTimer_500Ms = 0,g_ulTimer_1s = 0,g_ulTimer_250Ms = 0;
FATFS 	fs;
FIL 	file;
UINT 	br;
char  txt_name[120]; //

int main(void)
{
	/* 程序来到main函数之前，启动文件：statup_stm32f429xx.s已经调用
	* SystemInit()函数把系统时钟初始化成180MHZ
	* SystemInit()在system_stm32f4xx.c中定义
	* 如果用户想修改系统时钟，可自行编写程序修改
	*/
	delay_init(180);
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_1);  //配置中断优先级分组
	LED_GPIO_Config();
	RS232_USART_Config();						//配置232串口 DMA发送并且使能DMA
	RS485_USART_Init();
	PublicIO_Init();
	Scan_Mode();												//扫描模式
	TaskSysClk_Init(90-1,1000-1);				//1ms任务时钟
	bspEXTI_Init();								//外部中断初始化
	LASER_MODE_INSIDE();						//开机默认切换为内控
	GUIDE_LASER_ON();							//上电默认开指示光
	Power_ADC_Init();							//采集外控功率初始化
	Temperature_ADC_Init();						//温度采集ADC初始化
	SHT10_Heshuqi_Config();						//合束器SHT10初始化
	SHT10_Config();								//机柜SHT10初始化
	PCF8563_IIC_Init();             			//需要使用PCF8563时，先初始化IIC时钟
	
	USART_DMACmd(RS232_USART,USART_DMAReq_Tx,ENABLE);  //使能串口的DMA发送。
	USART_DMACmd(RS485_USART,USART_DMAReq_Tx,ENABLE);  //使能串口的DMA发送。
	taModuleData[0].ID = 1;
	taModuleData[1].ID = 2;
	taModuleData[2].ID = 4;
	taModuleData[3].ID = 8;
	taModuleData[4].ID = 16;
	taModuleData[5].ID = 32;
	taModuleData[6].ID = 64;
	taModuleData[7].ID = 128;
	while(1)
	{
		Task_Process();
	}

}
