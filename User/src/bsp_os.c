#include "bsp_os.h"
#include "bsp_led.h"
#include "bsp_publicIO.h"
#include "bsp_usart.h"
#include "bsp_dma.h"

u32 USER_FLAG=0;//用户标志
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
		if(start_off<10)
			start_off++;
		start_on=0;
	}  //扫描模拟PWM翻转
	else
	{
		if(start_on<10)
			start_on++;
		start_off=0;
	}
	if((start_on>2)&&(start_on<10))
	{
		start_on=10;
		USER_FLAG|=USER_START_ON;
	}
	if(USER_FLAG & USER_START_ON)
	{
	  if(START_In() == 1)start_cnt++;
	  else start_cnt = 0;
	  if(start_cnt>10)//10ms延时消抖
	  {
      start_cnt=0;
      USER_FLAG &= USER_START_OFF;
      if(USER_FLAG & USER_LASER_ON)//出光预备状态则切换为关闭激光预备状态
      {
        USER_FLAG &= USER_LASER_OFF;//关闭出光预备状态
        PANEL_STAR_OFF();
				STOP_CTRL_OFF();
				INSIDE_PWM_OFF();//内控PWM打开
				INSIDE_EN_OFF();//内控EN打开
      }
  		else //关闭出光预备状态则切换为出光预备状态
  		{
  		  USER_FLAG |= USER_LASER_ON;
  		  PANEL_STAR_ON();//面板灯打开
				STOP_CTRL_ON();//STOP打开
				INSIDE_PWM_ON();//内控PWM打开
				INSIDE_EN_ON();//内控EN打开
  		}
	  }

	}
  if((QBH1_In() == 0) && (QBH2_In() == 0) && (QBH3_In() == 0))
  {
    g_ulControlAlarm |= ERROR_QBH;
  }
  else
  {
		g_ulControlAlarm &= ERROR_QBH;
  }
  if(STOP_In() == 1)
  {
    g_ulControlAlarm |= ERROR_E_STOP;
  }
  else
  {
		g_ulControlAlarm &= ERROR_E_STOP;
  }
  if(INTERLOCKA_In() == 0)
  {
		g_ulControlAlarm |= ERROR_INTERLOCKA;
  }
  else
  {
		g_ulControlAlarm &= ERROR_INTERLOCKA;
  }
}


void task2(void)
{
  if(g_ulControlAlarm & ERROR_INTERLOCKA)
  {
    printf("ERROR_INTERLOCKA \r\n");
  }
  if(g_ulControlAlarm & ERROR_QBH)
  {
		printf("ERROR_QBH \r\n");
  }
  if(g_ulControlAlarm & ERROR_E_STOP)
  {
		printf("ERROR_E_STOP \r\n");
  }
	// printf("     g_ulFlow = %d\r\n", g_ulFlow);
}


//测试 RS232 DMA发送模式
void task3(void)
{
	if(DMA_GetFlagStatus(RS232_USART_DMA_STREAM,DMA_FLAG_TCIF6)!=RESET)//等待DMA1_Steam6传输完成
	{
		DMA_ClearFlag(RS232_USART_DMA_STREAM,DMA_FLAG_TCIF6);//清除DMA1_Steam6传输完成标志
	}
	DMA_Enable(RS232_USART_DMA_STREAM,SENDBUFF_SIZE);     //开始一次DMA传输！
}


void task4(void)
{
//   printf("task4.\r\n");
   LED4_Out() = !LED4_Out();
}

TaskStruct_T tasks[] =
{
   {0,10,10,task1},  //10ms 用各个任务的函数名初始化
   {0,500,500,task2},	//500ms
   {0,800,800,task3},	//800ms
   {0,1000,1000,task4}	//1000ms
};

//定义任务数量
u32 task_count = sizeof(tasks) / sizeof(TaskStruct_T);

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
	u8 i = 0;
	if (RESET != TIM_GetITStatus(TIM3,TIM_IT_Update))//检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM3,TIM_IT_Update);
		g_ulFlowTimerCnt++;//水流量检测计数
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
             tasks[i].TaskPointer(); // 运行任务
             tasks[i].isRun = 0;//将标志位清零
         }
     }
}
