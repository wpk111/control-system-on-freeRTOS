#ifndef __SCAN__
#define __SCAN__

//�������ȼ�
#define SCAN_TASK_PRIO		2
//�����ջ��С	
#define SCAN_STK_SIZE 		200  
//������
TaskHandle_t SCANTask_Handler;
//������
void scan_task(void *pvParameters);




#endif
