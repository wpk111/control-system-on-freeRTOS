#ifndef __SCAN__
#define __SCAN__

//任务优先级
#define SCAN_TASK_PRIO		2
//任务堆栈大小	
#define SCAN_STK_SIZE 		200  
//任务句柄
TaskHandle_t SCANTask_Handler;
//任务函数
void scan_task(void *pvParameters);




#endif
