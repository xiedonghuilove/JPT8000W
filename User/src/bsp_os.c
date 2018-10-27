#include "bsp_os.h"
#include "bsp_led.h"
#include "bsp_publicIO.h"
#include "bsp_usart.h"
#include "bsp_dma.h"
#include "bsp_adc.h"
#include "data.h"
#include "bsp_sht10.h"
#include "bsp_heshuqi_sth10.h"
#include "bsp_usart.h"
#include "command_parse.h"
#include "bsp_i2c_gpio.h"
#include "bsp_eeprom_24xx.h"
#include "bsp_sys.h"

/*
	全局运行时间，单位1ms
	最长可以表示 24.85天，如果你的产品连续运行时间超过这个数，则必须考虑溢出问题
*/
__IO int32_t g_iRunTime = 0;


uint32_t g_ulUserFlag=0;//用户标志
uint32_t g_ulControlAlarm = 0;
uint32_t g_ulFlowTimerCnt = 0;
uint32_t g_ulFlow = 0;
void task1(void)
{
	static u8 start_cnt=0;
	static u8 start_on=0;
	static u8 start_off=0;
	if(START_In()==0)
	{
		if(start_off<10)start_off++;
		start_on=0;
	}  //扫描模拟PWM翻转
	else
	{
		if(start_on<10)start_on++;
		start_off=0;
	}
	if((start_on>2)&&(start_on<10))
	{
		start_on=10;
		g_ulUserFlag|=USER_START_ON;
	}
	if(g_ulUserFlag & USER_START_ON)
	{
	  if(START_In() == 1)start_cnt++;
	  else start_cnt = 0;
	  if(start_cnt>10)//10ms延时消抖
	  {
      start_cnt=0;
      g_ulUserFlag &= USER_START_OFF;
      if(g_ulUserFlag & USER_LASER_ON)//出光预备状态则切换为关闭激光预备状态
      {
        g_ulUserFlag &= USER_LASER_OFF;//关闭出光预备状态
        PANEL_STAR_OFF();
				STOP_CTRL_OFF();
				tMasterData.TestSwitch = 0;
				INSIDE_PWM_OFF();//内控PWM关闭
				// INSIDE_EN_OFF();//内控EN关闭
      }
  		else //关闭出光预备状态则切换为出光预备状态
  		{
  		  g_ulUserFlag |= USER_LASER_ON;
  		  PANEL_STAR_ON();//面板灯打开
				STOP_CTRL_ON();//STOP打开
				// INSIDE_PWM_ON();//内控PWM打开
				// INSIDE_EN_ON();//内控EN打开
  		}
	  }

	}
  if((QBH1_In() == 0) && (QBH2_In() == 0) && (QBH3_In() == 0))
  {
    g_ulControlAlarm |= ERROR_QBH;
  }
  else
  {
		g_ulControlAlarm &= ~ERROR_QBH;
  }
  if(STOP_In() == 1)
  {
    g_ulControlAlarm |= ERROR_E_STOP;
  }
  else
  {
		g_ulControlAlarm &= ~ERROR_E_STOP;
  }
  if(INTERLOCKA_In() == 0)
  {
		g_ulControlAlarm |= ERROR_INTERLOCKA;
  }
  else
  {
		g_ulControlAlarm &= ~ERROR_INTERLOCKA;
  }
}


void osGetTemp_500MS_Task(void)
{
	uint8_t i = 0;
	uint32_t tempvalue = 0;

	static uint8_t s_ucIndex = 0;

	//采集温度
	for(i=0;i<8;i++)// 8 * 2 = 16
	{
		Get_16_Temp(); //读取温度,一次读2个数据，总共读取8次。
	}
	//状态机编程 释放CPU
	switch (s_ucIndex)
	{
		case 0: if(SHT10_Get_Temp(&tempvalue) == 0)
					tMasterData.ControlTemp = tempvalue;
				s_ucIndex++;
				break;
		case 1: if(SHT10_Get_Humi(&tempvalue) == 0)
					tMasterData.ControlHumi = tempvalue;
				s_ucIndex++;
				break;
		case 2: if(SHT10_Heshuqi_Get_Temp(&tempvalue) == 0)
					tMasterData.HeshuqiModeTemp = tempvalue;
				s_ucIndex++;
				break;
		case 3: if(SHT10_Heshuqi_Get_Humi(&tempvalue) == 0)
					tMasterData.HeshuqiModeHumi = tempvalue;
				s_ucIndex++;
				break;
		case 4: tMasterData.RongdianheTemp = Get_Adc3(ADC_Channel_12);
				tMasterData.HeshuqiTemp = GetHeShuQi_Temp();//合束器温度
				tMasterData.HeshuqiWaterTemp = GetHeShuQi_Water_Temp();//合束器水冷板温度
				s_ucIndex++;
				break;
		default:
				s_ucIndex = 0;
				break;
	}
	LED4_Out() = !LED4_Out();

	//判断写flash标志
	if(g_ucWriteFlashFlag>0)
	{
		g_ulaBufFlash[Index_CheckData] = 0x55AA;
		g_ulaBufFlash[Index_FlashWaterFlow] = tMasterData.FlashWaterFlow;
		g_ulaBufFlash[Index_FlashModeSelection] = tMasterData.FlashModeSelection;
		g_ulaBufFlash[Index_FlashHeshuqiTemp] = tMasterData.FlashHeshuqiTemp;
		g_ulaBufFlash[Index_FlashHeshuqiWaterTemp] = tMasterData.FlashHeshuqiWaterTemp;
		g_ulaBufFlash[Index_FlashRongdianheTemp] = tMasterData.FlashRongdianheTemp;
		g_ulaBufFlash[Index_FlashRedUserState] = tMasterData.FlashRedUserState;
		g_ulaBufFlash[Index_FlashHeshuqiRedCurrent] = tMasterData.FlashHeshuqiRedCurrent;
	}
	switch (g_ucWriteFlashFlag) {
		case 1:
		ee_WriteBytes(1, NULL,0, EE_SIZE);
//		ee_WriteBytes(0,(uint8_t*)(&FLASH_CHECK),0,4);
		g_ucWriteFlashFlag = 0;
		break;
		case 2:
		ee_WriteBytes(0, (uint8_t*)g_ulaBufFlash,0,sizeof(g_ulaBufFlash));
		g_ucWriteFlashFlag = 0;
		break;
		default:
		break;
	}
}


//测试 RS232 DMA发送模式
void task3(void)
{
//	Master_SendPackagetoSubcotrol();
}


void task4(void)
{
}

TaskStruct_T tasks[] =
{
   {0,10,10,task1},  //10ms 用各个任务的函数名初始化
   {0,900,900,osGetTemp_500MS_Task},	//500ms
   {0,50,50,task3},	//50ms
   {0,1000,1000,task4}	//1000ms
};

//定义任务数量
u32 task_count = sizeof(tasks) / sizeof(TaskStruct_T);

/*
*********************************************************************************************************
*	函 数 名: bsp_GetRunTime
*	功能说明: 获取CPU运行时间，单位1ms。最长可以表示 24.85天，如果你的产品连续运行时间超过这个数，则必须考虑溢出问题
*	形    参:  无
*	返 回 值: CPU运行时间，单位1ms
*********************************************************************************************************
*/
int32_t bsp_GetRunTime(void)
{
	int32_t runtime;

	DISABLE_INT();  	/* 关中断 */

	runtime = g_iRunTime;	/* 这个变量在Systick中断中被改写，因此需要关中断进行保护 */

	ENABLE_INT();  		/* 开中断 */

	return runtime;
}

void TaskSysClk_Init(u16 period, u16 prescaler)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3,ENABLE);

    TIM_TimeBaseInitStruct.TIM_Prescaler = prescaler;//定时器的预分频系数
    TIM_TimeBaseInitStruct.TIM_Period = period;//定时器的计数值
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM3,&TIM_TimeBaseInitStruct);
    TIM_ITConfig(TIM3,TIM_IT_Update,ENABLE); //允许更新中断

    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_Init(&NVIC_InitStructure); //中断初始化

    TIM_Cmd(TIM3,ENABLE);//使能定时器
 }



 //TIMER3中断  1ms一次
void TIM3_IRQHandler(void)
{
	static uint8_t OS_timerCnt = 0;
	u8 i = 0;
	if (RESET != TIM_GetITStatus(TIM3,TIM_IT_Update))//检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);

		g_ulFlowTimerCnt++;//水流量检测计数
		OS_timerCnt++;
		if(OS_timerCnt>37)
		{
			Master_SendPackagetoSubcotrol();
			OS_timerCnt = 0;
		}

		/* 全局运行时间每1ms增1 */
		g_iRunTime++;
		if (g_iRunTime == 0x7FFFFFFF)	/* 这个变量是 int32_t 类型，最大数为 0x7FFFFFFF */
		{
			g_iRunTime = 0;
		}

		for (i=0; i < task_count; ++i) //遍历任务数组
		{
		 if (tasks[i].TimerSlice)  //判断时间片是否到了
		 {
			  --tasks[i].TimerSlice;
			  if (0 == tasks[i].TimerSlice) //时间片到了
			  {
				   tasks[i].isRun = 0x01;//置位  表示任务可以执行
				   tasks[i].TimerSlice = tasks[i].SliceNumber; //重新加载时间片值，为下次做准备
			  }
		 }
		}
	}
}


void Task_Process(void)
{
     u8 i = 0;
     for (i=0; i < task_count; ++i) //遍历任务数组
     {
         if (tasks[i].isRun) //若任务可执行，则执行任务
         {
             tasks[i].TaskPointer(); // 运行任务  --> 改变指针函数
             tasks[i].isRun = 0;//将标志位清零
         }
     }
}
