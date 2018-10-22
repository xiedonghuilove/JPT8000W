#ifndef __BSP_PUBLICIO_
#define __BSP_PUBLICIO_

#include "stm32f4xx.h"
#include "bsp_sys.h"

//蜂鸣器    PE4
#define Buzzer_PIN                  GPIO_Pin_4
#define Buzzer_GPIO_PORT            GPIOE
#define Buzzer_GPIO_CLK             RCC_AHB1Periph_GPIOE
#define Buzzer_L()				          PEout(4)=0
#define Buzzer_H()				          PEout(4)=1
#define BUZZER_Out()			          PEout(4)

//面板指示灯  运行 C_RUN_LED  PC9
#define RUN_LED_PIN                 GPIO_Pin_9
#define RUN_LED_GPIO_PORT           GPIOC
#define RUN_LED_GPIO_CLK            RCC_AHB1Periph_GPIOC
#define RUN_LED_RED()               PCout(9)=1
#define RUN_LED_GREEN()             PCout(9)=0

//面板指示灯  报警 C_RUN_LED  PA8
#define ERR_LED_PIN                 GPIO_Pin_8
#define ERR_LED_GPIO_PORT           GPIOA
#define ERR_LED_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define ERR_LED_RED()               PAout(8)=1
#define ERR_LED_GREEN()             PAout(8)=0

//三色灯  顶部红色 PG7
#define TOP_RED_PIN                 GPIO_Pin_7
#define TOP_RED_GPIO_PORT           GPIOG
#define TOP_RED_GPIO_CLK            RCC_AHB1Periph_GPIOG
#define TOP_RED_ON()               PGout(7)=1
#define TOP_RED_OFF()              PGout(7)=0

//三色灯  顶部黄色 PG6
#define TOP_YELLOW_PIN                 GPIO_Pin_6
#define TOP_YELLOW_GPIO_PORT           GPIOG
#define TOP_YELLOW_GPIO_CLK            RCC_AHB1Periph_GPIOG
#define TOP_YELLOW_ON()                PGout(6)=1
#define TOP_YELLOW_OFF()               PGout(6)=0

//三色灯  顶部绿色 PG5
#define TOP_GREEN_PIN                 GPIO_Pin_5
#define TOP_GREEN_GPIO_PORT           GPIOG
#define TOP_GREEN_GPIO_CLK            RCC_AHB1Periph_GPIOG
#define TOP_GREEN_ON()                PGout(5)=1
#define TOP_GREEN_OFF()               PGout(5)=0

//STAT灯  PG8
#define PANEL_STAR_PIN                 GPIO_Pin_8
#define PANEL_STAR_GPIO_PORT           GPIOG
#define PANEL_STAR_GPIO_CLK            RCC_AHB1Periph_GPIOG
#define PANEL_STAR_ON()                PGout(8)=1
#define PANEL_STAR_OFF()               PGout(8)=0
#define PANEL_STAR_Out()               PGout(8)

//继电器 Realy1  PA9   预留
#define RELAY_1_PIN                 GPIO_Pin_9
#define RELAY_1_GPIO_PORT           GPIOA
#define RELAY_1_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define RELAY_1_ON()                PAout(9)=1
#define RELAY_1_OFF()               PAout(9)=0

//继电器 Realy2  PA10  预留
#define RELAY_2_PIN                 GPIO_Pin_10
#define RELAY_2_GPIO_PORT           GPIOA
#define RELAY_2_GPIO_CLK            RCC_AHB1Periph_GPIOA
#define RELAY_2_ON()                PAout(10)=1
#define RELAY_2_OFF()               PAout(10)=0

//继电器 Realy3  PH12   预留
#define RELAY_3_PIN                 GPIO_Pin_12
#define RELAY_3_GPIO_PORT           GPIOH
#define RELAY_3_GPIO_CLK            RCC_AHB1Periph_GPIOH
#define RELAY_3_ON()                PHout(12)=1
#define RELAY_3_OFF()               PHout(12)=0

//继电器 Realy4  PD8   预留
#define RELAY_4_PIN                 GPIO_Pin_8
#define RELAY_4_GPIO_PORT           GPIOD
#define RELAY_4_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define RELAY_4_ON()                PDout(8)=1
#define RELAY_4_OFF()               PDout(8)=0

//继电器 Realy5  PD12   预留
#define RELAY_5_PIN                 GPIO_Pin_12
#define RELAY_5_GPIO_PORT           GPIOD
#define RELAY_5_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define RELAY_5_ON()                PDout(12)=1
#define RELAY_5_OFF()               PDout(12)=0

//继电器 Realy6  PD11   预留
#define RELAY_6_PIN                 GPIO_Pin_11
#define RELAY_6_GPIO_PORT           GPIOD
#define RELAY_6_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define RELAY_6_ON()                PDout(11)=1
#define RELAY_6_OFF()               PDout(11)=0

//继电器 Realy7  PD10   预留
#define RELAY_7_PIN                 GPIO_Pin_10
#define RELAY_7_GPIO_PORT           GPIOD
#define RELAY_7_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define RELAY_7_ON()                PDout(10)=1
#define RELAY_7_OFF()               PDout(10)=0

//继电器 Realy8  PD9   预留
#define RELAY_8_PIN                 GPIO_Pin_9
#define RELAY_8_GPIO_PORT           GPIOD
#define RELAY_8_GPIO_CLK            RCC_AHB1Periph_GPIOD
#define RELAY_8_ON()                PDout(9)=1
#define RELAY_8_OFF()               PDout(9)=0

//漏水检测    PH13
#define LEAK_WATER_PIN              GPIO_Pin_13
#define LEAK_WATER_PORT             GPIOH
#define LEAK_WATER_CLK              RCC_AHB1Periph_GPIOH
#define LEAK_WATER_In()             PHin(13)

// 水冷机故障 PH6
#define INTERLOCKA_PIN              GPIO_Pin_6
#define INTERLOCKA_PORT             GPIOH
#define INTERLOCKA_CLK              RCC_AHB1Periph_GPIOH
#define INTERLOCKA_In()             PHin(6)

//START  正常不触发 为0 触发为 1
#define START_PIN              GPIO_Pin_7
#define START_PORT             GPIOH
#define START_CLK              RCC_AHB1Periph_GPIOH
#define START_In()             PHin(7)

//QBH2  检测低电平     PH8
#define QBH2_PIN              GPIO_Pin_8
#define QBH2_PORT             GPIOH
#define QBH2_CLK              RCC_AHB1Periph_GPIOH
#define QBH2_In()             PHin(8)

//STOP  检测高电平  下拉
#define STOP_PIN              GPIO_Pin_9
#define STOP_PORT             GPIOH
#define STOP_CLK              RCC_AHB1Periph_GPIOH
#define STOP_In()             PHin(9)

//QBH3  检测低电平     PH10
#define QBH3_PIN              GPIO_Pin_10
#define QBH3_PORT             GPIOH
#define QBH3_CLK              RCC_AHB1Periph_GPIOH
#define QBH3_In()             PHin(10)

//QBH1  检测低电平     PH11
#define QBH1_PIN              GPIO_Pin_11
#define QBH1_PORT             GPIOH
#define QBH1_CLK              RCC_AHB1Periph_GPIOH
#define QBH1_In()             PHin(11)

//外部红光控制信号检测  PG0  检测高电平
#define RED_CTR_PIN              GPIO_Pin_0
#define RED_CTR_PORT             GPIOG
#define RED_CTR_CLK              RCC_AHB1Periph_GPIOG
#define RED_CTR_In()             PGin(0)

//模式检测    0 --> 内控    1 --> 外控  检测高电平
#define MODE_PIN              GPIO_Pin_1
#define MODE_PORT             GPIOG
#define MODE_CLK              RCC_AHB1Periph_GPIOG
#define MODE_In()             PGin(1)
#define MODE_INSIDE           0
#define MODE_OUTSIDE          1

//REMOTE   PE11    远程控制信号检测高电平
#define REMOTE_PIN              GPIO_Pin_11
#define REMOTE_PORT             GPIOE
#define REMOTE_CLK              RCC_AHB1Periph_GPIOE
#define REMOTE_In()             PEin(11)

//INTERLOCKB   PE12    检测高电平
#define INTERLOCKB_PIN              GPIO_Pin_12
#define INTERLOCKB_PORT             GPIOE
#define INTERLOCKB_CLK              RCC_AHB1Periph_GPIOE
#define INTERLOCKB_In()             PEin(12)

//报警输出  推挽输出 PF15
#define WARNING_PIN              GPIO_Pin_15
#define WARNING_PORT             GPIOF
#define WARNING_CLK              RCC_AHB1Periph_GPIOF
#define WARNING_OUT_ON()         PFout(15)=1
#define WARNING_OUT_OFF()        PFout(15)=0

//24V外控使能  检测高电平
#define OUTSIDE_EN_PIN              GPIO_Pin_7
#define OUTSIDE_EN_PORT             GPIOE
#define OUTSIDE_EN_CLK              RCC_AHB1Periph_GPIOE

//PWM
#define PWM_PIN              GPIO_Pin_10
#define PWM_PORT             GPIOE
#define PWM_CLK              RCC_AHB1Periph_GPIOE

//PWM&EN  PE8  下拉输入
#define PWM_EN_PIN              GPIO_Pin_8
#define PWM_EN_PORT             GPIOE
#define PWM_EN_CLK              RCC_AHB1Periph_GPIOE
#define PWM_EN_In()             PEin(8)

//STOP_CTRL   PH5  推挽输出
#define STOP_CTRL_PIN              GPIO_Pin_5
#define STOP_CTRL_PORT             GPIOH
#define STOP_CTRL_CLK              RCC_AHB1Periph_GPIOH
#define STOP_CTRL_ON()             PHout(5)=1
#define STOP_CTRL_OFF()            PHout(5)=0

//INSIDE_EN   内控使能  推挽输出
#define INSIDE_EN_PIN              GPIO_Pin_3
#define INSIDE_EN_PORT             GPIOH
#define INSIDE_EN_CLK              RCC_AHB1Periph_GPIOH
#define INSIDE_EN_ON()             PHout(3)=1
#define INSIDE_EN_OFF()            PHout(3)=0

//INSIDE PWM 内控PWM   PH2  推挽输出
#define INSIDE_PWM_PIN             GPIO_Pin_2
#define INSIDE_PWM_PORT            GPIOH
#define INSIDE_PWM_CLK             RCC_AHB1Periph_GPIOH
#define INSIDE_PWM_ON()            PHout(2)=1
#define INSIDE_PWM_OFF()           PHout(2)=0

//LSAER_MODE  激光模式控制   PH4    推挽输出
#define LASER_MODE_PIN                GPIO_Pin_4
#define LASER_MODE_PORT               GPIOH
#define LASER_MODE_CLK                RCC_AHB1Periph_GPIOH
#define LASER_MODE_INSIDE()           PHout(4)=1
#define LASER_MODE_OUTSIDE()          PHout(4)=0

//引导激光(红光)   PE1
#define GUIDE_LASER_PIN              GPIO_Pin_1
#define GUIDE_LASER_PORT             GPIOE
#define GUIDE_LASER_CLK              RCC_AHB1Periph_GPIOE
#define GUIDE_LASER_ON()             PEout(1)=1
#define GUIDE_LASER_OFF()            PEout(1)=0


void PublicIO_Init(void);

#endif
