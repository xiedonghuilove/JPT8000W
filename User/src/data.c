/**
  ******************************************************************************
  * @file    date.c
  * @author  JPT
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   数据定义与数据处理
  ******************************************************************************
  */
#include "data.h"
#include "bsp_eeprom_24xx.h"
#include "bsp_usart.h"
#include "stdio.h"
#include "bsp_publicIO.h"
#include "bsp_pd.h"
#include "bsp_os.h"

//结构体变量定义 8字节对齐 后面需要访问
MasterData_T tMasterData;

ModuleData_T taModuleData[MODULE_NUM];
uint32_t g_aSubcontrolID[MODULE_NUM] = {1,2,3,4,5,6,7,8};//子控板ID

uint16_t g_ulaBufFlash[INDEX_FLASH_NUM] = {0};
uint32_t g_ulTopGuideLEDStateCnt = 0;

uint32_t g_ulaModuleCommunicationCnt[MODULE_NUM] = {0};

uint32_t g_ulGuideUserControl = 0;
//热敏电阻电压值  对应 温度 表
uint16_t TEMP_LD[60]=
{
    1481, 1520,  1558, 1597, 1635, 1673, 1712, 1750, 1788, 1826,\
    1864, 1901,  1938, 1975, 2011, 2048, 2083, 2119, 2154, 2188,\
    2222, 2256,  2289, 2322, 2354, 2411, 2417, 2448, 2478, 2508,\
    2537, 2565,  2593, 2621, 2648, 2674, 2700, 2725, 2750, 2774,\
    2798, 2821,  2844, 2866, 2887, 2909, 2929, 2949, 2969, 2988,\
    3007
};

/*
*********************************************************************************************************
*	函 数 名: Scan_Mode
*	功能说明: 扫描模式
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void Scan_Mode(void)
{
  uint8_t i,ucRobotCnt=0,ucTestCnt=0,ucRS232Cnt=0;
  for (i = 0; i < 10; i++)
  {
    delay_ms(100);
    if(INTERLOCKB_In()==1)//互锁信号短接
    {
      if(MODE_In()==1)//RS232模式
      {
        ucRS232Cnt++;
      }
      else//测试模式
      {
        ucTestCnt++;
      }
    }
    else//互锁信号断开  正常模式
    {
      if(MODE_In()==1)//外控模式
      {
        ucRobotCnt++;
      }
      else//测试模式
      {
        ucTestCnt++;
      }
    }
    LED3_Out() = !LED3_Out();
  }
  if(ucRS232Cnt>1)
  {
    tMasterData.Mode=2;    //RS232
    LASER_MODE_OUTSIDE();
  }
  else if(ucRobotCnt>1)
  {
    tMasterData.Mode=1;    //外控
    LASER_MODE_OUTSIDE();
  }
  else if(ucTestCnt>8)
  {
    tMasterData.Mode=0;   //测试
    LASER_MODE_INSIDE();
  }
}
/*
*********************************************************************************************************
*	函 数 名: EE_Flash_Set_Data
*	功能说明: 开机读取一次eeprom数据
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void EE_Flash_Set_Data(void)
{
  int i = 0;
  //读取flash中的数据
  ee_ReadBytes((uint8_t*)g_ulaBufFlash, 0, sizeof(g_ulaBufFlash));
  for(i=0;i<INDEX_FLASH_NUM;i++)
  {
      printf(" %04X", g_ulaBufFlash[i]);
  }
  //校验正确
  if(g_ulaBufFlash[Index_CheckData] == 0x55AA)
  {
    //将flash中的数据赋值给结构体变量
    tMasterData.FlashWaterFlow = g_ulaBufFlash[Index_FlashWaterFlow];
    tMasterData.FlashModeSelection = g_ulaBufFlash[Index_FlashModeSelection];
    tMasterData.FlashHeshuqiTemp = g_ulaBufFlash[Index_FlashHeshuqiTemp];
    tMasterData.FlashHeshuqiWaterTemp = g_ulaBufFlash[Index_FlashHeshuqiWaterTemp];
    tMasterData.FlashQBHWaterFlow = g_ulaBufFlash[Index_FlashQBHWaterFlow];
    tMasterData.FlashRedUserState = g_ulaBufFlash[Index_FlashRedUserState];
    tMasterData.FlashHeshuqiRedCurrent = g_ulaBufFlash[Index_FlashHeshuqiRedCurrent];
  }
  else
  {
    tMasterData.FlashWaterFlow = 4500;//整机水流量报警点
    tMasterData.FlashModeSelection = 0xFF;//默认开启8个
    tMasterData.FlashHeshuqiTemp = 45;
    tMasterData.FlashHeshuqiWaterTemp = 45;
    tMasterData.FlashQBHWaterFlow = 4500;//QBH水流量报警点
    tMasterData.FlashRedUserState = 0xBB;//默认状态
    tMasterData.FlashHeshuqiRedCurrent = 90;//合束器红光电流
  }

}

/*
*********************************************************************************************************
*	函 数 名: Alarm_Close_Laser
*	功能说明: 报警关闭激光
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void Alarm_Close_Laser(void)
{
  RUN_LED_RED();
  ERR_LED_RED();
  LED1_Out() = 1;
  STOP_CTRL_OFF();
  PANEL_STAR_OFF();//START灯灭
  tMasterData.TestSwitch = 0;//报警后，上位机的时候不走。
  WARNING_OUT_ON();//报警输出
  TOP_STATUS_ALARM();//顶部报警状态
  INSIDE_EN_OFF();//内控使能关闭
}
/*
*********************************************************************************************************
*	函 数 名: CS_In
*	功能说明: CS判断
*	形    参:  _index
*	返 回 值:  0 / 1
*********************************************************************************************************
*/
uint8_t CS_In(uint8_t _index)
{
  switch (_index)
  {
      case 0: if(CS1_In() == PD_FORWARD_ON)return 1;
      break;
      case 1: if(CS2_In() == PD_FORWARD_ON)return 1;
      break;
      case 2: if(CS3_In() == PD_FORWARD_ON)return 1;
      break;
      case 3: if(CS4_In() == PD_FORWARD_ON)return 1;
      break;
      case 4: if(CS5_In() == PD_FORWARD_ON)return 1;
      break;
      case 5: if(CS6_In() == PD_FORWARD_ON)return 1;
      break;
      case 6: if(CS7_In() == PD_FORWARD_ON)return 1;
      break;
      case 7: if(CS8_In() == PD_FORWARD_ON)return 1;
      break;
      case 8: if(CS9_CS10_In() == PD_FORWARD_ON)return 1;
      break;
      default: break;
  }
  return 0;
} 

/*
*********************************************************************************************************
*	函 数 名: MS_In
*	功能说明: MS判断
*	形    参:  _index
*	返 回 值:  0 / 1
*********************************************************************************************************
*/
uint8_t MS_In(uint8_t _index)
{
  switch (_index)
  {
      case 0: if(MS1_In() == PD_CONDITION_ON)return 1;
      break;
      case 1: if(MS2_In() == PD_CONDITION_ON)return 1;
      break;
      case 2: if(MS3_In() == PD_CONDITION_ON)return 1;
      break;
      case 3: if(MS4_In() == PD_CONDITION_ON)return 1;
      break;
      case 4: if(MS5_In() == PD_CONDITION_ON)return 1;
      break;
      case 5: if(MS6_In() == PD_CONDITION_ON)return 1;
      break;
      case 6: if(MS7_In() == PD_CONDITION_ON)return 1;
      break;
      case 7: if(MS8_In() == PD_CONDITION_ON)return 1;
      break;
      default: break;
  }
  return 0;
} 

/*
*********************************************************************************************************
*	函 数 名: Guide_LED_Status_Control
*	功能说明: 指示灯用户控制状态
*	形    参:  无
*	返 回 值:  无
*********************************************************************************************************
*/
void Guide_LED_Status_Control(void)
{
    //未报警状态
    if(!tMasterData.AlarmLowBit)
    {
        if(!tMasterData.Mode)//内控
        {
            if(!tMasterData.TestSwitch)//不出光
            {
                RUN_LED_RED();//运行灯红色
                TOP_STATUS_NORMAL();//顶部指示灯正常颜色
                // GUIDE_LASER_ON();//上电默认开指示光
                if((g_ulGuideUserControl == User_Red_Close)&&(tMasterData.FlashRedUserState == USER_CONTROL))
                {
                    GUIDE_LASER_OFF();
                }
                else
                {
                    GUIDE_LASER_ON();
                }
            }
            else//内控出光状态
            {
                if((tMasterData.Power>=10)&&(tMasterData.TestSwitch == 1)&&(g_ulUserFlag & USER_LASER_ON)&&(tMasterData.FlashModeSelection>0))
                {
                    RUN_LED_GREEN();//运行灯绿色
                    TOP_STATUS_LASER();//顶部指示灯为出光状态
                    GUIDE_LASER_OFF();//关闭红光
                }
            }
        }
        else//外控
        {
            if(g_ulUserFlag & USER_LASER_ON)//START开启
            {
                if(g_ulTopGuideLEDStateCnt > 4000)//1.2S没光
                {
                    g_ulTopGuideLEDStateCnt = 5000;
                    RUN_LED_RED();//运行灯红色
                    TOP_STATUS_NORMAL();//顶部指示灯正常颜色
                    // GUIDE_LASER_ON();	//上电默认开指示光
                    if((RED_CTR_In() == 1)&&(tMasterData.FlashRedUserState == USER_CONTROL))
                    {
                        GUIDE_LASER_OFF();
                    }
                    else
                    {
                        GUIDE_LASER_ON();
                    }

                }
                else if (tMasterData.FlashModeSelection>0)
                {
                    RUN_LED_GREEN();//运行灯绿色
                    TOP_STATUS_LASER();//顶部指示灯为出光状态
                    GUIDE_LASER_OFF();//关闭红光
                }
            }
            else  //START关闭
            {
                RUN_LED_RED();//运行灯红色
                TOP_STATUS_NORMAL();//顶部指示灯正常颜色
                // GUIDE_LASER_ON();	//上电默认开指示光
                if((RED_CTR_In() == 1)&&(tMasterData.FlashRedUserState == USER_CONTROL))
                {
                    GUIDE_LASER_OFF();
                }
                else
                {
                    GUIDE_LASER_ON();
                }
            }
        }
    }
    else//报警状态 可以进行红光控制
    {
        if(((tMasterData.Mode == 0)&&(tMasterData.FlashRedUserState == USER_CONTROL)&&(g_ulGuideUserControl == User_Red_Close))\
        ||((tMasterData.Mode>0)&&(tMasterData.FlashRedUserState == USER_CONTROL)&&(RED_CTR_In() == 1)))
        {
            GUIDE_LASER_OFF();
        }
        else
        {
            GUIDE_LASER_ON();
        }
    }
}
