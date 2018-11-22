/*
******************************************************************************
* @file    bsp_warning.c
* @author  JPT
* @version V1.0
* @date    2018-xx-xx
* @brief   报警配置
******************************************************************************
*/
#include "bsp_warning.h"
#include "bsp_os.h"

/*
*********************************************************************************************************
*	函 数 名: bsp_Warning_GPIO_Init
*	功能说明: bsp 报警 GPIO初始化
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void bsp_Warning_GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;

    //使能时钟
	RCC_AHB1PeriphClockCmd ( MODULE_WARNING1_CLK|
                           MODULE_WARNING3_CLK|
                           MODULE_WARNING4_CLK|
                           MODULE_WARNING5_CLK, ENABLE);
    
    //下拉输入 检测高电平
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_Pin = MODULE_WARNING1_PIN;
    GPIO_Init(MODULE_WARNING1_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MODULE_WARNING2_PIN;
    GPIO_Init(MODULE_WARNING2_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MODULE_WARNING3_PIN;
    GPIO_Init(MODULE_WARNING3_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MODULE_WARNING4_PIN;
    GPIO_Init(MODULE_WARNING4_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MODULE_WARNING5_PIN;
    GPIO_Init(MODULE_WARNING5_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MODULE_WARNING6_PIN;
    GPIO_Init(MODULE_WARNING6_PORT, &GPIO_InitStructure);
    
    GPIO_InitStructure.GPIO_Pin = MODULE_WARNING7_PIN;
    GPIO_Init(MODULE_WARNING7_PORT, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = MODULE_WARNING8_PIN;
    GPIO_Init(MODULE_WARNING8_PORT, &GPIO_InitStructure);

}

/*
*********************************************************************************************************
*	函 数 名: Moudle_Warning_In
*	功能说明: 模块索引输入，检测模块是否报警
*	形    参:  index
*	返 回 值:  0/1
*********************************************************************************************************
*/
uint8_t Moudle_Warning_In(uint8_t _index)
{
    switch (_index)
    {
        case 0: if(MODULE_WARNING1_In() == MODEL_WARNING_ON)return 1;
        break;
        case 1: if(MODULE_WARNING2_In() == MODEL_WARNING_ON)return 1;
        break;
        case 2: if(MODULE_WARNING3_In() == MODEL_WARNING_ON)return 1;
        break;
        case 3: if(MODULE_WARNING4_In() == MODEL_WARNING_ON)return 1;
        break;
        case 4: if(MODULE_WARNING5_In() == MODEL_WARNING_ON)return 1;
        break;
        case 5: if(MODULE_WARNING6_In() == MODEL_WARNING_ON)return 1;
        break;
        case 6: if(MODULE_WARNING7_In() == MODEL_WARNING_ON)return 1;
        break;
        case 7: if(MODULE_WARNING8_In() == MODEL_WARNING_ON)return 1;
        break;
        default: break;
    }
    return 0;
}
