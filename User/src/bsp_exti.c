/*
******************************************************************************
* @file    bap_exti.c
* @author  JPT
* @version V1.0
* @date    2018-xx-xx
* @brief   外部中断配置
******************************************************************************
*/
#include "bsp_exti.h"
#include "bsp_os.h"

/*
*********************************************************************************************************
*	函 数 名: EXTI_GPIO_Init
*	功能说明: 外部中断 GPIO初始化
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
static void EXTI_GPIO_Init(void)
{
  GPIO_InitTypeDef  GPIO_InitStructure;

  //使能GPIOA,GPIOE时钟
  RCC_AHB1PeriphClockCmd(WATER_FLOW_GPIO_CLK, ENABLE);

  //引脚配置，上升沿触发
  GPIO_InitStructure.GPIO_Pin = WATER_FLOW_GPIO_PIN;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;//普通输入模式
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//100M
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;//下拉
  GPIO_Init(WATER_FLOW_GPIO_PORT, &GPIO_InitStructure);

}
/*
*********************************************************************************************************
*	函 数 名: bspEXTI_Init
*	功能说明: 外部中断线 初始化
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void bspEXTI_Init(void)
{
  NVIC_InitTypeDef   NVIC_InitStructure;
  EXTI_InitTypeDef   EXTI_InitStructure;

  EXTI_GPIO_Init();

  /* 使能 SYSCFG 时钟 ，使用GPIO外部中断时必须使能SYSCFG时钟*/
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

  /* 连接 EXTI 中断源 到key1引脚 */
  SYSCFG_EXTILineConfig(WATER_FLOW_EXTI_PORTSOURCE,WATER_FLOW_EXTI_PINSOURCE);

  /* 配置EXTI_Line*/
  EXTI_InitStructure.EXTI_Line = WATER_FLOW_EXTI_LINE;//Line14
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;//中断事件
  EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿触发
  EXTI_InitStructure.EXTI_LineCmd = ENABLE;//使能line
  EXTI_Init(&EXTI_InitStructure);//配置

  NVIC_InitStructure.NVIC_IRQChannel = WATER_FLOW_EXTI_IRQ;//外部中断10-15
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;//抢占优先级0
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;//子优先级2
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;//使能外部中断通道
  NVIC_Init(&NVIC_InitStructure);//配置

}
/*
*********************************************************************************************************
*	函 数 名: WATER_FLOW_IRQHandler --> EXTI15_10_IRQHandler
*	功能说明: 外部中断服务函数
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void WATER_FLOW_IRQHandler(void)
{
  static uint16_t s_usFlowCnt = 0,s_usFlowAlarmCnt = 0;
  //确保是否产生了EXTI Line中断
  if(EXTI_GetITStatus(WATER_FLOW_EXTI_LINE) != RESET)
  {
    EXTI_ClearITPendingBit(WATER_FLOW_EXTI_LINE); //清除LINE0上的中断标志位
    s_usFlowCnt++;
    if(g_ulFlowTimerCnt>1000)//1ms * 1000 = 1s
    {
      g_ulFlowTimerCnt = 0;
      g_ulFlow = s_usFlowCnt*95;
      s_usFlowCnt = 0;
    }
  }

}
