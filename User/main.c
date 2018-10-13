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

//extern uint16_t g_usPowerValue;

extern __IO uint16_t g_usPowerValue;

uint32_t s_ulCurrentDate = 0,s_ulCurrentTime = 0;
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
	LED_GPIO_Config();
	RS232_USART_Config();
	PublicIO_Init();
	TaskSysClk_Init(90-1,1000-1);//1ms
	while(1)
	{
		Task_Process();
	}

}
