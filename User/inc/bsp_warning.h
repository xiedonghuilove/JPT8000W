#ifndef __BSP_WARNING_H
#define __BSP_WARNING_H

#include "stm32f4xx.h"
#include "bsp_sys.h"

/*
  C_Warning1 --> PF13
  C_Warning2 --> PF12
  C_Warning3 --> PC0 
  C_Warning4 --> PI10
  C_Warning5 --> PE6
  C_Warning6 --> PE5
  C_Warning7 --> PI7
  C_Warning8 --> PI6
*/

//PF13
#define MODULE_WARNING1_PIN              GPIO_Pin_13
#define MODULE_WARNING1_PORT             GPIOF
#define MODULE_WARNING1_CLK              RCC_AHB1Periph_GPIOF
#define MODULE_WARNING1_In()             PFin(13)

//PF12
#define MODULE_WARNING2_PIN              GPIO_Pin_12
#define MODULE_WARNING2_PORT             GPIOF
#define MODULE_WARNING2_CLK              RCC_AHB1Periph_GPIOF
#define MODULE_WARNING2_In()             PFin(12)

//PC0
#define MODULE_WARNING3_PIN              GPIO_Pin_0
#define MODULE_WARNING3_PORT             GPIOC
#define MODULE_WARNING3_CLK              RCC_AHB1Periph_GPIOC
#define MODULE_WARNING3_In()             PCin(0)

//PI10
#define MODULE_WARNING4_PIN              GPIO_Pin_10
#define MODULE_WARNING4_PORT             GPIOI
#define MODULE_WARNING4_CLK              RCC_AHB1Periph_GPIOI
#define MODULE_WARNING4_In()             PIin(10)

//PE6
#define MODULE_WARNING5_PIN              GPIO_Pin_6
#define MODULE_WARNING5_PORT             GPIOE
#define MODULE_WARNING5_CLK              RCC_AHB1Periph_GPIOE
#define MODULE_WARNING5_In()             PEin(6)

//PE5
#define MODULE_WARNING6_PIN              GPIO_Pin_5
#define MODULE_WARNING6_PORT             GPIOE
#define MODULE_WARNING6_CLK              RCC_AHB1Periph_GPIOE
#define MODULE_WARNING6_In()             PEin(5)

//PI7
#define MODULE_WARNING7_PIN              GPIO_Pin_7
#define MODULE_WARNING7_PORT             GPIOI
#define MODULE_WARNING7_CLK              RCC_AHB1Periph_GPIOI
#define MODULE_WARNING7_In()             PIin(7)

//PI6
#define MODULE_WARNING8_PIN              GPIO_Pin_6
#define MODULE_WARNING8_PORT             GPIOI
#define MODULE_WARNING8_CLK              RCC_AHB1Periph_GPIOI
#define MODULE_WARNING8_In()             PIin(6)

#define MODEL_WARNING_ON		1
#define MODEL_WARNING_OFF		0

//主控板报警
#define ERROR_MASTER_CS1_ALARM                  (1<<0)
#define ERROR_MASTER_CS2_ALARM                  (1<<1)
#define ERROR_MASTER_CS3_ALARM                  (1<<2)
#define ERROR_MASTER_CS4_ALARM                  (1<<3)
#define ERROR_MASTER_CS5_ALARM                  (1<<4)
#define ERROR_MASTER_CS6_ALARM                  (1<<5)
#define ERROR_MASTER_CS7_ALARM                  (1<<6)
#define ERROR_MASTER_CS8_ALARM                  (1<<7)
#define ERROR_MASTER_CS9_CS10_ALARM             (1<<8)
#define ERROR_MASTER_HESHUQI_WATER_TEMP         (1<<9)
#define ERROR_MASTER_INTERLOCKA                 (1<<10)
#define ERROR_MASTER_QBH						(1<<11)
#define ERROR_MASTER_E_STOP                     (1<<12)
#define ERROR_MASTER_WATER						(1<<13)
#define ERROR_MASTER_QBHWATER					(1<<14)
#define ERROR_MASTER_SUBCONTROL                 (1<<15)


//模块报警
#define ERROR_SUB_PD_FORWORD							      (1<<4)
#define ERROR_SUB_LOW_V									        (1<<6)
#define ERROR_SUB_OVER_I							  	      (1<<7)
#define ERROR_SUB_DIAN_WATER							      (1<<8)
#define ERROR_SUB_PD_FORWORD_FAULT						  (1<<9)
#define ERROR_SUB_GUAN_WATER							      (1<<10)
#define ERROR_SUB_COMMUNICATION             		(1<<11)

void bsp_Warning_GPIO_Init(void);
uint8_t Moudle_Warning_In(uint8_t _index);
#endif
