/**
  ******************************************************************************
  * @file    bsp_publicIO.c
  * @author  JPT
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   公共的IO定义
  ******************************************************************************
  */
#include "bsp_publicIO.h"
/*
*********************************************************************************************************
*	函 数 名: Buzzer_Panel_Init
*	功能说明: 初始化串口硬件，并对全局变量赋初值.
*	形    参:  无
*	返 回 值: 无
*********************************************************************************************************
*/
void PublicIO_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
  //使能时钟
	RCC_AHB1PeriphClockCmd ( Buzzer_GPIO_CLK|
                           RUN_LED_GPIO_CLK|
                           ERR_LED_GPIO_CLK|
                           TOP_RED_GPIO_CLK|
                           RELAY_3_GPIO_CLK|
                           RELAY_4_GPIO_CLK|
														LEAK_WATER_CLK|
														RED_CTR_CLK, ENABLE);

  //推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //引脚
	GPIO_InitStructure.GPIO_Pin = Buzzer_PIN;
  GPIO_Init(Buzzer_GPIO_PORT, &GPIO_InitStructure);

  //运行灯
  GPIO_InitStructure.GPIO_Pin = RUN_LED_PIN;
  GPIO_Init(RUN_LED_GPIO_PORT, &GPIO_InitStructure);

  //报警灯
  GPIO_InitStructure.GPIO_Pin = ERR_LED_PIN;
  GPIO_Init(ERR_LED_GPIO_PORT, &GPIO_InitStructure);

  //三色灯 红色
  GPIO_InitStructure.GPIO_Pin = TOP_RED_PIN;
  GPIO_Init(TOP_RED_GPIO_PORT, &GPIO_InitStructure);

  //三色灯 黄色
  GPIO_InitStructure.GPIO_Pin = TOP_YELLOW_PIN;
  GPIO_Init(TOP_YELLOW_GPIO_PORT, &GPIO_InitStructure);

  //三色灯 绿色
  GPIO_InitStructure.GPIO_Pin = TOP_GREEN_PIN;
  GPIO_Init(TOP_GREEN_GPIO_PORT, &GPIO_InitStructure);

  //面板STAR灯
  GPIO_InitStructure.GPIO_Pin = PANEL_STAR_PIN;
  GPIO_Init(PANEL_STAR_GPIO_PORT, &GPIO_InitStructure);

  //继电器 PA
  GPIO_InitStructure.GPIO_Pin = RELAY_1_PIN|RELAY_2_PIN;
  GPIO_Init(RELAY_1_GPIO_PORT, &GPIO_InitStructure);

  //继电器 PH
  GPIO_InitStructure.GPIO_Pin = RELAY_3_PIN;
  GPIO_Init(RELAY_3_GPIO_PORT, &GPIO_InitStructure);

  //继电器 PD
  GPIO_InitStructure.GPIO_Pin = RELAY_4_PIN|RELAY_5_PIN|RELAY_6_PIN|RELAY_7_PIN|RELAY_8_PIN;
  GPIO_Init(RELAY_4_GPIO_PORT, &GPIO_InitStructure);

	//报警输出灯
	GPIO_InitStructure.GPIO_Pin = WARNING_PIN;
  GPIO_Init(WARNING_PORT, &GPIO_InitStructure);

	//STOP_CTRL
	GPIO_InitStructure.GPIO_Pin = STOP_CTRL_PIN;
  GPIO_Init(STOP_CTRL_PORT, &GPIO_InitStructure);

	//内控使能  PH3  推挽输出
	GPIO_InitStructure.GPIO_Pin = INSIDE_EN_PIN;
	GPIO_Init(INSIDE_EN_PORT, &GPIO_InitStructure);

	//内控PWM
	GPIO_InitStructure.GPIO_Pin = INSIDE_PWM_PIN;
	GPIO_Init(INSIDE_PWM_PORT, &GPIO_InitStructure);

	//出光模式选择
	GPIO_InitStructure.GPIO_Pin = LASER_MODE_PIN;
	GPIO_Init(LASER_MODE_PORT, &GPIO_InitStructure);

	//指示激光控制引脚
	GPIO_InitStructure.GPIO_Pin = GUIDE_LASER_PIN;
	GPIO_Init(GUIDE_LASER_PORT, &GPIO_InitStructure);
	
  //漏水检测 上拉输入，检测低电平
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Pin = LEAK_WATER_PIN;
  GPIO_Init(LEAK_WATER_PORT, &GPIO_InitStructure);

	//水冷机故障 PH6
	GPIO_InitStructure.GPIO_Pin = INTERLOCKA_PIN;
  GPIO_Init(INTERLOCKA_PORT, &GPIO_InitStructure);

	//QBH2  PH8
	GPIO_InitStructure.GPIO_Pin = QBH2_PIN;
	GPIO_Init(QBH2_PORT, &GPIO_InitStructure);

	//QBH3  PH10
	GPIO_InitStructure.GPIO_Pin = QBH3_PIN;
	GPIO_Init(QBH3_PORT, &GPIO_InitStructure);

	//QBH1  PH11
	GPIO_InitStructure.GPIO_Pin = QBH1_PIN;
	GPIO_Init(QBH1_PORT, &GPIO_InitStructure);

	//K1 检测低电平
	GPIO_InitStructure.GPIO_Pin = KEY_K1_PIN;
	GPIO_Init(KEY_K1_PORT, &GPIO_InitStructure);

	//K2 检测低电平
	GPIO_InitStructure.GPIO_Pin = KEY_K2_PIN;
	GPIO_Init(KEY_K2_PORT, &GPIO_InitStructure);

	//START 下拉输入  检测高电平
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Pin = START_PIN;
  GPIO_Init(START_PORT, &GPIO_InitStructure);

	//STOP 下拉输入 PH9
	GPIO_InitStructure.GPIO_Pin = STOP_PIN;
  GPIO_Init(STOP_PORT, &GPIO_InitStructure);

	//检测外部红光控制信号。  下拉输入 PG0
	GPIO_InitStructure.GPIO_Pin = RED_CTR_PIN;
  GPIO_Init(RED_CTR_PORT, &GPIO_InitStructure);

	//模式检测   下拉输入
	GPIO_InitStructure.GPIO_Pin = MODE_PIN;
	GPIO_Init(MODE_PORT, &GPIO_InitStructure);

	//远程START检测  下拉输入
	GPIO_InitStructure.GPIO_Pin = REMOTE_PIN;
	GPIO_Init(REMOTE_PORT, &GPIO_InitStructure);

	//INTERLOCKB   下拉输入
	GPIO_InitStructure.GPIO_Pin = INTERLOCKB_PIN;
	GPIO_Init(INTERLOCKB_PORT, &GPIO_InitStructure);

	//PWM&EN   下拉输入
	GPIO_InitStructure.GPIO_Pin = PWM_EN_PIN;
	GPIO_Init(PWM_EN_PORT, &GPIO_InitStructure);


}
