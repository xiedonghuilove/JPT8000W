/*
******************************************************************************
* @file    bsp_pd.c
* @author  JPT
* @version V1.0
* @date    2018-xx-xx
* @brief   PD检测 
******************************************************************************
*/

#include "bsp_pd.h"
#include "data.h"

uint8_t g_ucaCsDetectionFlag[MODULE_NUM+1] = {0};
uint8_t g_ucaCsForwordCnt[MODULE_NUM+1] = {0};
/*
*********************************************************************************************************
*	函 数 名: bsp_PD_GPIO_Init 
*	功能说明: PD GPIO初始化
*	形    参: 无
*	返 回 值:  无
*********************************************************************************************************
*/
void bsp_PD_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
  //使能时钟
	RCC_AHB1PeriphClockCmd ( MS3_CLK|
                           MS1_CLK|
                           MS2_CLK|
                           MS5_CLK|
                           MS7_CLK|
                           CS4_CLK|
						   CS9_CS10_CLK, ENABLE);
    
    //下拉输入 检测高电平
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Pin = MS1_PIN;
    GPIO_Init(MS1_PORT, &GPIO_InitStructure);

    //MS2
    GPIO_InitStructure.GPIO_Pin = MS2_PIN;
    GPIO_Init(MS2_PORT, &GPIO_InitStructure);

    //MS3
    GPIO_InitStructure.GPIO_Pin = MS3_PIN;
    GPIO_Init(MS3_PORT, &GPIO_InitStructure);

    //MS4
    GPIO_InitStructure.GPIO_Pin = MS4_PIN;
    GPIO_Init(MS4_PORT, &GPIO_InitStructure);

    //MS5
    GPIO_InitStructure.GPIO_Pin = MS5_PIN;
    GPIO_Init(MS5_PORT, &GPIO_InitStructure);

    //MS6
    GPIO_InitStructure.GPIO_Pin = MS6_PIN;
    GPIO_Init(MS6_PORT, &GPIO_InitStructure);

    //MS7
    GPIO_InitStructure.GPIO_Pin = MS7_PIN;
    GPIO_Init(MS7_PORT, &GPIO_InitStructure);

    //MS8
    GPIO_InitStructure.GPIO_Pin = MS8_PIN;
    GPIO_Init(MS8_PORT, &GPIO_InitStructure);

    //CS1
    GPIO_InitStructure.GPIO_Pin = CS1_PIN;
    GPIO_Init(CS1_PORT, &GPIO_InitStructure);

    //CS2
    GPIO_InitStructure.GPIO_Pin = CS2_PIN;
    GPIO_Init(CS2_PORT, &GPIO_InitStructure);

    //CS3
    GPIO_InitStructure.GPIO_Pin = CS3_PIN;
    GPIO_Init(CS3_PORT, &GPIO_InitStructure);

    //CS4
    GPIO_InitStructure.GPIO_Pin = CS4_PIN;
    GPIO_Init(CS4_PORT, &GPIO_InitStructure);

    //CS5
    GPIO_InitStructure.GPIO_Pin = CS5_PIN;
    GPIO_Init(CS5_PORT, &GPIO_InitStructure);

    //CS6
    GPIO_InitStructure.GPIO_Pin = CS6_PIN;
    GPIO_Init(CS6_PORT, &GPIO_InitStructure);

    //CS7
    GPIO_InitStructure.GPIO_Pin = CS7_PIN;
    GPIO_Init(CS7_PORT, &GPIO_InitStructure);

    //CS8
    GPIO_InitStructure.GPIO_Pin = CS8_PIN;
    GPIO_Init(CS8_PORT, &GPIO_InitStructure);

    //CS9&CS10
    GPIO_InitStructure.GPIO_Pin = CS9_CS10_PIN;
    GPIO_Init(CS9_CS10_PORT, &GPIO_InitStructure);
}
