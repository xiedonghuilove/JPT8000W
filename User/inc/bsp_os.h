#ifndef __BSP_OS_H__
#define __BSP_OS_H__

#include "stm32f4xx.h"
#include "stdio.h"
#include "bsp_sys.h"

//任务结构
typedef struct tagTaskStruct_T
{
  uint8_t isRun;    //表示任务是否运行
  uint16_t TimerSlice;   //分配给任务的时间片
  uint16_t SliceNumber; //时间片个数，在TimeSlice为0时，将其赋值给TimerSlice重新计数。
  void (*TaskPointer)(void);  //任务函数指针
}TaskStruct_T;

// 任务清单
typedef enum
{
	TAST_DISP_CLOCK,            // 显示时钟
	TAST_KEY_SAN,             // 按键扫描
	TASK_DISP_WS,             // 工作状态显示
	// 这里添加你的任务。。。。
	TASKS_MAX                // 总的可供分配的定时任务数目
} TASK_LIST_E;

//数组大小
#define ARRAYSIZE(a) (sizeof(a) / sizeof((a)[0]))

#define USER_START_ON			 (1<<0)
#define USER_START_OFF			~(1<<0)
#define USER_LASER_ON			 (1<<2)//4
#define USER_LASER_OFF			~(1<<2)

#define ERROR_INTERLOCKA			(1<<12)
#define ERROR_QBH					(1<<13)
#define ERROR_E_STOP				(1<<14)
#define ERROR_WATER					(1<<15)

void Task_Process(void);
void TaskSysClk_Init(u16 period, u16 prescaler);


#endif //__OS_H__
