#ifndef __DATA_H
#define __DATA_H

#include "stm32f4xx.h"
#include "bsp_publicIO.h"
#include "bsp_led.h"

//定义模块总数
#define MODULE_NUM          8

#define BROADCAST_ADDRESS   0xff
#define MASTER_ID			      0xfe
#define MODULE_ID_1			    0x01
#define MODULE_ID_2			    0x02
#define MODULE_ID_3			    0x04
#define MODULE_ID_4			    0x08
#define MODULE_ID_5			    0x10
#define MODULE_ID_6			    0x20
#define MODULE_ID_7			    0x40
#define MODULE_ID_8			    0x80

extern uint32_t g_aSubcontrolID[MODULE_NUM];

#define TOP_STATUS_NORMAL()        TOP_YELLOW_ON();\
                                   TOP_GREEN_OFF();\
                                   TOP_RED_OFF();

#define TOP_STATUS_LASER()        TOP_GREEN_ON();\
                                  TOP_YELLOW_OFF();\
                                  TOP_RED_OFF();

#define TOP_STATUS_ALARM()        TOP_RED_ON();\
                                  TOP_YELLOW_OFF();\
                                  TOP_GREEN_OFF();

#define USER_CONTROL		0xAA
#define DEFAULT_CONTROL 	0xBB

#define User_Red_Open		0xCC
#define User_Red_Close		0xDD

extern uint32_t g_ulGuideUserControl;

/*
******************************************************************************
*     定义主控板数据结构体
******************************************************************************
*/
typedef struct
{
    uint16_t ID;//主模块ID
    uint16_t Mode;//模式 内外控
    uint16_t Power;//功率
    uint16_t ControlTemp;//控制板温度
    uint16_t ControlHumi;//控制板湿度
    uint16_t HeshuqiModeTemp;//合束器模块温度
    uint16_t HeshuqiModeHumi;//合束器模块湿度
    uint16_t TestSwitch;//内控switch
    uint16_t WaterFlow;//水流量
    uint16_t FlashWaterFlow;//水流量报警点
    uint16_t FlashHeshuqiTemp;//合束器温度报警点
    uint16_t FlashHeshuqiWaterTemp;//合束器水冷板温度报警点
    uint16_t FlashQBHWaterFlow;//QBH水流量报警点
    uint16_t FlashRedUserState;//红光用户控制状态
    uint16_t FlashModeSelection;//模块选择
    uint16_t FlashHeshuqiRedCurrent;//红光电流
    uint16_t RongdianheTemp;//熔点盒温度
    uint16_t HeshuqiTemp;//合束器温度
    uint16_t HeshuqiWaterTemp;//合束器水冷板温度
    uint16_t AlarmLowBit;//报警
    uint16_t QBHWaterFlow;//QBH水流量
}MasterData_T;
extern MasterData_T tMasterData;

/*
******************************************************************************
*     定义子模块帧命令
******************************************************************************
*/
#define CMD_MASTER_LEN			CMD_MASTER_NULL

typedef enum {
	CMD_MASTER_ID = 0,    				//ID
	CMD_MASTER_MODE,					//模式
	CMD_MASTER_POWER,					//功率
	CMD_MASTER_CONTROL_TEMP,			//控制板温度
	CMD_MASTER_CONTROL_HUMI,			//控制板湿度
	CMD_MASTER_HESHUQI_MODULE_TEMP,		//合束器模块温度
	CMD_MASTER_HESHUQI_MODULE_HUMI,		//合束器模块湿度
	CMD_MASTER_TEST_SWITCH,				//测试开关
	CMD_MASTER_WATER_FLOW,				//水流量
	CMD_MASTER_FLASH_WATER_FLOW,		//水流量报警值
	CMD_MASTER_MODULE_SELECTION,		//模块选择
	CMD_MASTER_RONGDIANHE_TEMP,			//熔点盒温度
	CMD_MASTER_HESHUQI_TEMP,			//合束器温度
	CMD_MASTER_HESHUQI_WATER_TEMP,		//合束器水冷板温度
	CMD_MASTER_NULL,
}MASTER_FRAME_COMMAND_E;

/*
******************************************************************************
*     定义模块数据结构体
******************************************************************************
*/
typedef struct
{
    uint16_t ID;//子模块ID
    uint16_t ElectricalTemp;//电模块温度
    uint16_t ElectricalHumi;//电模块湿度
    uint16_t ElectricalWaterTemp;//电模块水冷板
    uint16_t OpticalWaterTemp;//光模块水冷板
    uint16_t I1;
    uint16_t I2;
    uint16_t I3;
    uint16_t I4;
    uint16_t I5;
    uint16_t I6;
    uint16_t I7;
    uint16_t I8;
    uint16_t I9;
    uint16_t SubControlVersion;//子控制板版本号
    uint16_t OverCurrent;//过流
    uint16_t LowVoltage;//低压
    uint16_t FullPowerAdjustment;//满功率微调
    uint16_t RedPhotocurrent;//红光电流
    uint16_t ForwardReferenceVoltage;//前向光参考电压
    uint16_t OverElectricalWaterTemp;//过电模块水冷板
    uint16_t OverOpticalWaterTemp;//过光模块水冷板
    uint16_t BiasReferenceVoltage;//偏置参考电压
    uint16_t GuideState;//指示光状态
    uint16_t DriverVersion;//驱动板版本号
    uint16_t SubControlPower;//子控制板功率
    uint16_t Voltage_1;//电压值1
    uint16_t Voltage_2;//电压值2
    uint16_t Voltage_3;//电压值3
    uint16_t SubControlAlarm;//子模块报警值
}ModuleData_T;
extern ModuleData_T taModuleData[MODULE_NUM];
/*
******************************************************************************
*     定义子模块帧命令
******************************************************************************
*/
typedef enum {
    CMD_MODULE_ID = 1,    //ID
    CMD_MODULE_ELECTRIAL_TEMP,//电模块温度
    CMD_MODULE_ELECTRIAL_HUMI,//电模块湿度
    CMD_MODULE_ELECTRIAL_WATER_TEMP,//电模块水冷板温度
    CMD_MODULE_OPTICAL_WATER_TEMP,//光模块水冷板温度
    CMD_MODULE_FULL_POWER_ADJUSTMENT,//满功率微调
    CMD_MODULE_RED_CURRENT,//红光电流
    CMD_MODULE_FORWARD_VOLTAGE,//前向光参考电压
    CMD_MODULE_OVER_ELECTRIAL_WATER_TEMP,
    CMD_MODULE_OVER_OPTICAL_WATER_TEMP,
    CMD_MODULE_POWER,
}MODULE_FRAME_COMMAND_E;

enum Flash_Index{
    Index_CheckData = 0,
    Index_FlashWaterFlow,
    Index_FlashModeSelection,
    Index_FlashHeshuqiTemp,
    Index_FlashHeshuqiWaterTemp,
    Index_FlashQBHWaterFlow,
    Index_FlashRedUserState,
    Index_FlashHeshuqiRedCurrent,
    INDEX_FLASH_NUM,
};
extern uint16_t g_ulaBufFlash[INDEX_FLASH_NUM];

extern uint32_t g_ulTopGuideLEDStateCnt;
extern uint32_t g_ulaModuleCommunicationCnt[MODULE_NUM];

extern uint16_t TEMP_LD[60];

void Scan_Mode(void);
void EE_Flash_Set_Data(void);
void Alarm_Close_Laser(void);
uint8_t MS_In(uint8_t _index);
uint8_t CS_In(uint8_t _index);
void Guide_LED_Status_Control(void);


#endif
