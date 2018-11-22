#ifndef __BSP_PD_H
#define __BSP_PD_H

#include "stm32f4xx.h"
#include "bsp_sys.h"
#include "data.h"

/*
    CS1-8 条件检测
    {
    C_PDB1 --> PB0
    C_PDB2 --> PF11
    C_PDB3 --> PA4
    C_PDB4 --> PA3
    C_PDB5 --> PC3
    C_PDB6 --> PA0 
    C_PDB7 --> PI5
    C_PDB8 --> PI4
    }
*/
//MS1  PB0
#define MS1_PIN              GPIO_Pin_0
#define MS1_PORT             GPIOB
#define MS1_CLK              RCC_AHB1Periph_GPIOB
#define MS1_In()             PBin(0)

//MS2  PF11
#define MS2_PIN              GPIO_Pin_11
#define MS2_PORT             GPIOF
#define MS2_CLK              RCC_AHB1Periph_GPIOF
#define MS2_In()             PFin(11)

//MS3   PA4
#define MS3_PIN              GPIO_Pin_4
#define MS3_PORT             GPIOA
#define MS3_CLK              RCC_AHB1Periph_GPIOA
#define MS3_In()             PAin(4)

//MS4   PA3
#define MS4_PIN              GPIO_Pin_3
#define MS4_PORT             GPIOA
#define MS4_CLK              RCC_AHB1Periph_GPIOA
#define MS4_In()             PAin(3)

//MS5   PC3
#define MS5_PIN              GPIO_Pin_3
#define MS5_PORT             GPIOC
#define MS5_CLK              RCC_AHB1Periph_GPIOC
#define MS5_In()             PCin(3)

//MS6   PA0
#define MS6_PIN              GPIO_Pin_0
#define MS6_PORT             GPIOA
#define MS6_CLK              RCC_AHB1Periph_GPIOA
#define MS6_In()             PAin(0)

//MS7   PI5
#define MS7_PIN              GPIO_Pin_5
#define MS7_PORT             GPIOI
#define MS7_CLK              RCC_AHB1Periph_GPIOI
#define MS7_In()             PIin(5)

//MS8   PI4
#define MS8_PIN              GPIO_Pin_4
#define MS8_PORT             GPIOI
#define MS8_CLK              RCC_AHB1Periph_GPIOI
#define MS8_In()             PIin(4)

/*
    PD1 --> PB5
    PD2 --> PB4 
    PD3 --> PB3
    PD4 --> PG15
    PD5 --> PG12 
    PD6 --> PG11
    PD7 --> PG10
    PD8 --> PG9
    PD9&PD10 --> PD7
*/

//CS1  PB5
#define CS1_PIN              GPIO_Pin_5
#define CS1_PORT             GPIOB
#define CS1_CLK              RCC_AHB1Periph_GPIOB
#define CS1_In()             PBin(5)

//CS2  PB4
#define CS2_PIN              GPIO_Pin_4
#define CS2_PORT             GPIOB
#define CS2_CLK              RCC_AHB1Periph_GPIOB
#define CS2_In()             PBin(4)

//CS3  PB3
#define CS3_PIN              GPIO_Pin_3
#define CS3_PORT             GPIOB
#define CS3_CLK              RCC_AHB1Periph_GPIOB
#define CS3_In()             PBin(3)

//CS4  PG15
#define CS4_PIN              GPIO_Pin_15
#define CS4_PORT             GPIOG
#define CS4_CLK              RCC_AHB1Periph_GPIOG
#define CS4_In()             PGin(15)

//CS5  PG12
#define CS5_PIN              GPIO_Pin_12
#define CS5_PORT             GPIOG
#define CS5_CLK              RCC_AHB1Periph_GPIOG
#define CS5_In()             PGin(12)

//CS6  PG11
#define CS6_PIN              GPIO_Pin_11
#define CS6_PORT             GPIOG
#define CS6_CLK              RCC_AHB1Periph_GPIOG
#define CS6_In()             PGin(11)

//CS7  PG10
#define CS7_PIN              GPIO_Pin_10
#define CS7_PORT             GPIOG
#define CS7_CLK              RCC_AHB1Periph_GPIOG
#define CS7_In()             PGin(10)

//CS8  PG9
#define CS8_PIN              GPIO_Pin_9
#define CS8_PORT             GPIOG
#define CS8_CLK              RCC_AHB1Periph_GPIOG
#define CS8_In()             PGin(9)

//CS9&CS10  PD7
#define CS9_CS10_PIN              GPIO_Pin_7
#define CS9_CS10_PORT             GPIOD
#define CS9_CS10_CLK              RCC_AHB1Periph_GPIOD
#define CS9_CS10_In()             PDin(7)

//条件状态
#define PD_CONDITION_ON				0			//有光
#define PD_CONDITION_OFF			1			//没光

//结果检测
#define PD_FORWARD_ON			1			//有光
#define PD_FORWARD_OFF			0			//没光


//  #define MS_In(a)    if(a==0){if(MS1_In()==PD_CONDITION_ON)a=1;}else{a=0;}
// #define MS_In(a)    switch (a)\
//                    {\
//                        case 0: if(MS1_In()==PD_CONDITION_ON);return 1;\
//                                else return 0;\
//                                break;\
//                    }

// #define CS_In(a)                    switch (a)\
//                                     {\
//                                         case 0: CS1_In();break;\
//                                         case 1: CS2_In();break;\
//                                         case 2: CS3_In();break;\
//                                         case 3: CS4_In();break;\
//                                         case 4: CS5_In();break;\
//                                         case 5: CS6_In();break;\
//                                         case 6: CS7_In();break;\
//                                         case 7: CS8_In();break;\
//                                         default:break;\
//                                     }
// #define CS_In(a)    if (a==0)\
// 					CS1_In() == PD_CONDITION_ON;\
// 					else if(a==1)\
// 					CS2_In()\
//                     else if(a==2)\
// 					CS3_In()\
//                     else if(a==3)\
// 					CS4_In()\
//                     else if(a==4)\
// 					CS5_In()\
//                     else if(a==5)\
// 					CS6_In()\
//                     else if(a==6)\
// 					CS7_In()\
//                     else if(a==7)\
// 					CS8_In()

extern uint8_t g_ucaCsDetectionFlag[MODULE_NUM+1];//检测标志
extern uint8_t g_ucaCsForwordCnt[MODULE_NUM+1];//报警计数

void bsp_PD_GPIO_Init(void);
#endif

