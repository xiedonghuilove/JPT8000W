#ifndef __DATA_H
#define __DATA_H

#include "stm32f4xx.h"
#include "bsp_publicIO.h"
#include "bsp_led.h"

//定义模块总数
#define MODULE_NUM          8

#define MASTER_ID			0xfe
#define MODULE_ID_1			0x01
#define MODULE_ID_2			0x02
#define MODULE_ID_3			0x04
#define MODULE_ID_4			0x08
#define MODULE_ID_5			0x10
#define MODULE_ID_6			0x20
#define MODULE_ID_7			0x40
#define MODULE_ID_8			0x80
extern uint32_t g_aSubcontrolID[MODULE_NUM];
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
  uint16_t ModeSelection;//模块选择
  uint16_t RongdianheTemp;//熔点盒温度
  uint16_t HeshuqiTemp;//合束器温度
  uint16_t HeshuqiWaterTemp;//合束器水冷板温度
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
  uint8_t ID;//子模块ID
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
  uint16_t Version;//版本号
  uint16_t OverVoltage;//过压
  uint16_t LowVoltage;//低压
  uint8_t FullPowerAdjustment;//满功率微调
  uint16_t RedPhotocurrent;//红光电流
  uint16_t ForwardReferenceVoltage;//前向光参考电压
  uint8_t OverElectricalWaterTemp;//过电模块水冷板
  uint8_t OverOpticalWaterTemp;//光模块水冷板
  uint16_t BiasReferenceVoltage;//偏置参考电压
  uint8_t GuideState;//指示光状态
  uint16_t CommunicationCount;//通信计数
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




void Scan_Mode(void);

#endif
