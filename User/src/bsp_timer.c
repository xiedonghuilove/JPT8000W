/*
******************************************************************************
* @file    bsp_timer.c
* @author  JPT
* @version V1.0
* @date    2018-xx-xx
* @brief   定时器配置
******************************************************************************
*/
#include "bsp_timer.h"
#include "bsp_pd.h"
#include "bsp_publicIO.h"
#include "bsp_led.h"
#include "data.h"
#include "bsp_os.h"
#include "bsp_adc.h"
#include "bsp_led.h"



/*
*********************************************************************************************************
*	函 数 名: Timer_CS_Condition_Init
*	功能说明: 定时器CS 条件检测
*	形    参:  _period      计数值   
*              _prescaler   预分频系数
*	返 回 值:  无
*********************************************************************************************************
*/
static void Timer_CS_Condition_Init(uint16_t _period,uint16_t _prescaler)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);

    TIM_TimeBaseInitStruct.TIM_Prescaler = _prescaler;//定时器的预分频系数
    TIM_TimeBaseInitStruct.TIM_Period = _period;//定时器的计数值
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStruct);
    TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE); //允许更新中断

    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure); //中断初始化

    TIM_Cmd(TIM2,ENABLE);//启动定时器
}

/*
*********************************************************************************************************
*	函 数 名: Timer_CS_Alarm_Init 
*	功能说明: 定时器CS 检测报警 
*	形    参:  _period      计数值   
*              _prescaler   预分频系数
*	返 回 值:  无
*********************************************************************************************************
*/
static void Timer_CS_Alarm_Init(uint16_t _period,uint16_t _prescaler)
{
    TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStruct;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);

    TIM_TimeBaseInitStruct.TIM_Prescaler = _prescaler;//定时器的预分频系数
    TIM_TimeBaseInitStruct.TIM_Period = _period;//定时器的计数值
    TIM_TimeBaseInitStruct.TIM_ClockDivision = TIM_CKD_DIV1;
    TIM_TimeBaseInitStruct.TIM_CounterMode = TIM_CounterMode_Up;
    TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStruct);
    TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE); //允许更新中断

    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 3;//抢占优先级
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_Init(&NVIC_InitStructure); //中断初始化

    TIM_Cmd(TIM4,ENABLE);//启动定时器
}

/*
*********************************************************************************************************
*	函 数 名: TIM2_IRQHandler 
*	功能说明: 定时器2中断函数 定时5us 用于报警检测
*	形    参: 无
*	返 回 值:  无
*********************************************************************************************************
*/
void TIM2_IRQHandler(void)
{
    static uint8_t s_ucCsCnt[MODULE_NUM+1];
    int i = 0;
    if (RESET != TIM_GetITStatus(TIM2,TIM_IT_Update))//检查TIM更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM2,TIM_IT_Update);
        //子模块CS条件检测 --> 用于检测CS
        for(i=0; i < MODULE_NUM; i++)
        {
            if((MS_In(i))&&(tMasterData.FlashModeSelection&(1<<i)))
            {
                s_ucCsCnt[i]++;
                if(s_ucCsCnt[i]>=3)g_ucaCsDetectionFlag[i] = 1;
            }
            else
            {
                g_ucaCsDetectionFlag[i] = 0;
                s_ucCsCnt[i] = 0;
            } 
        }
        if(((CS1_In() == PD_FORWARD_ON) && (tMasterData.FlashModeSelection&MODULE_ID_1))||\
            ((CS2_In() == PD_FORWARD_ON)&&(tMasterData.FlashModeSelection&MODULE_ID_2))||\
            ((CS3_In() == PD_FORWARD_ON)&&(tMasterData.FlashModeSelection&MODULE_ID_3))||\
            ((CS4_In() == PD_FORWARD_ON)&&(tMasterData.FlashModeSelection&MODULE_ID_4))||\
            ((CS5_In() == PD_FORWARD_ON)&&(tMasterData.FlashModeSelection&MODULE_ID_5))||\
            ((CS6_In() == PD_FORWARD_ON)&&(tMasterData.FlashModeSelection&MODULE_ID_6))||\
            ((CS7_In() == PD_FORWARD_ON)&&(tMasterData.FlashModeSelection&MODULE_ID_7))||\
            ((CS8_In() == PD_FORWARD_ON)&&(tMasterData.FlashModeSelection&MODULE_ID_8)))
        {
            s_ucCsCnt[8]++;
            if(s_ucCsCnt[8]>=4) g_ucaCsDetectionFlag[8] = 1;
        }
        else
        {
            s_ucCsCnt[8] = 0;
            g_ucaCsDetectionFlag[8] = 0;
        }
        //CS结果检测
        for(i = 0; i < MODULE_NUM+1; i++)
        {
            if(CS_In(i))g_ucaCsForwordCnt[i] = 0;
        }

    }
}

/*
*********************************************************************************************************
*	函 数 名: TIM4_IRQHandler 
*	功能说明: 定时器4中断函数 定时300us 用于报警判断
*	形    参: 无
*	返 回 值:  无
*********************************************************************************************************
*/
void TIM4_IRQHandler(void)
{
    int i = 0;
    if (RESET != TIM_GetITStatus(TIM4,TIM_IT_Update))//检查TIM3更新中断发生与否
	{
		TIM_ClearITPendingBit(TIM4,TIM_IT_Update);
        //START按钮按下
        if(g_ulUserFlag & USER_LASER_ON)
        {
            for(i = 0; i < MODULE_NUM+1; i++)
            {
                if(g_ucaCsDetectionFlag[i] == 1)
                {
                    g_ucaCsForwordCnt[i]++;
                    if(CS_In(i))g_ucaCsForwordCnt[i] = 0;
                    if(g_ucaCsForwordCnt[i]>4)
                    {
                        tMasterData.AlarmLowBit |= 1<<i;
                        Alarm_Close_Laser();
                    }
                }
            }
        }
        //顶部指示灯检测(针对外部)
        if((PWM_EN_In() == 1)&&(((g_usPowerValue*100)>>12)>=10))
        {
            g_ulTopGuideLEDStateCnt = 0;
        }
        else
        {
            g_ulTopGuideLEDStateCnt++;
        }
    }
}
/*
*********************************************************************************************************
*	函 数 名: Timer_Init
*	功能说明: 定时器初始化
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void Timer_Init(void)
{
    Timer_CS_Condition_Init(10-1,90-1);//条件  Timer2 10us
    Timer_CS_Alarm_Init(300-1,90-1);//检测    Timer4  300us
}
