#include "sys.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "FreeRTOS.h"
#include "task.h"
#include "lcd.h"
#include "show.h"
#include "w25qxx.h"
#include "touch.h"
#include "Touch_scan.h"
//#include "contral_task.h"
#include "queue.h"
#include "adc.h"
#include "text.h"
#include "w25qxx.h" 
/************************************************
 ALIENTEK ��ӢSTM32F103������ FreeRTOSʵ��2-1
 FreeRTOS��ֲʵ��-�⺯���汾
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/


#define AAA 0

u8 showplace = 1;//1��ʾ�����棻2��ʾ���ݲɼ����棻3��ʾ���ݴ������




extern int data[2000];


//�������ȼ�
#define START_TASK_PRIO		1
//�����ջ��С	
#define START_STK_SIZE 		128  
//������
TaskHandle_t StartTask_Handler;
//������
void start_task(void *pvParameters);

//�������ȼ�
#define LED0_TASK_PRIO		2
//�����ջ��С	
#define LED0_STK_SIZE 		50  
//������
TaskHandle_t LED0Task_Handler;
//������
void led0_task(void *pvParameters);

//�������ȼ�
#define LED1_TASK_PRIO		3
//�����ջ��С	
#define LED1_STK_SIZE 		50  
//������
TaskHandle_t LED1Task_Handler;
//������
void led1_task(void *pvParameters);


//������Ϣ���е�����
#define TOUCHMSG_Q_NUM    1  		//������Ϣ���е�����  

QueueHandle_t Touch_Queue;   		//������Ϣ���о��



#if !(AAA)

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//����ϵͳ�ж����ȼ�����4	 
	delay_init();	    				//��ʱ������ʼ��	  
	uart_init(115200);					//��ʼ������
	LED_Init();	//��ʼ��LED
	font_init();
  LCD_Init();
	TP_Init();
	SHOW_Init();
	Adc_Init();
	control_init();
	 
	//������ʼ����
    xTaskCreate((TaskFunction_t )start_task,            //������
                (const char*    )"start_task",          //��������
                (uint16_t       )START_STK_SIZE,        //�����ջ��С
                (void*          )NULL,                  //���ݸ��������Ĳ���
                (UBaseType_t    )START_TASK_PRIO,       //�������ȼ�
                (TaskHandle_t*  )&StartTask_Handler);   //������              
    vTaskStartScheduler();          //�����������
}

#endif




//��ʼ����������
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //�����ٽ���
	
		Touch_Queue = xQueueCreate(TOUCHMSG_Q_NUM,sizeof(u8));
	  if(Touch_Queue!=NULL)   	//��Ϣ����Key_Queue�����ɹ�
	
    //����LED0����
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //����LED1����
    xTaskCreate((TaskFunction_t )led1_task,     
                (const char*    )"led1_task",   
                (uint16_t       )LED1_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )LED1_TASK_PRIO,
                (TaskHandle_t*  )&LED1Task_Handler); 
		xTaskCreate((TaskFunction_t )scan_task,     
                (const char*    )"scan_task",   
                (uint16_t       )SCAN_STK_SIZE, 
                (void*          )NULL,
                (UBaseType_t    )SCAN_TASK_PRIO,
                (TaskHandle_t*  )&SCANTask_Handler); 	
										
    vTaskDelete(StartTask_Handler); //ɾ����ʼ����
    taskEXIT_CRITICAL();            //�˳��ٽ���
}

//LED0������ 
void led0_task(void *pvParameters)
{
    while(1)
    {
        LED0=~LED0;
        vTaskDelay(500);
    }
}   

//LED1������
void led1_task(void *pvParameters)
{
    while(1)
    {
        LED1=0;
        vTaskDelay(200);
        LED1=1;
        vTaskDelay(800);
    }
}


void scan_task(void *pvParameters)
{
	u16 vp=2000,vn=1000;//���÷��������ѹ�ߵͳ�ʼֵΪ2v��1v

	while(1)
	{
		TP_Scan(0);//ɨ����Ļ����.����ֵ:��ǰ����״̬:0,�����޴���;1,�����д���;����¼����������x��y��
	  TOUCH_key_v(&vp,&vn);//��������˸һ�²����������
	  vTaskDelay(100);
		
	}
}


#if  AAA

int main(void)
 {	 
 	u8 x=0;
	u8 v1=0xff;
	u8 v2=0;
	u8 lcd_id[12];			//���LCD ID�ַ���
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 	//���ڳ�ʼ��Ϊ115200
 	LED_Init();			     //LED�˿ڳ�ʼ��
	LCD_Init();
	
	control_init();
	 
	AIR_GPIO = 0; 
	POINT_COLOR=RED;
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//��LCD ID��ӡ��lcd_id���顣 
  	while(1) 
	{		 
		
		
		voltage_control(0x90,v1-=2);
		
		voltage_control(0x92,v2+=3);
		
		AIR_GPIO = !AIR_GPIO;
		
		switch(x)
		{
			case 0:LCD_Clear(WHITE);break;
			case 1:LCD_Clear(BLACK);break;
			case 2:LCD_Clear(BLUE);break;
			case 3:LCD_Clear(RED);break;
			case 4:LCD_Clear(MAGENTA);break;
			case 5:LCD_Clear(GREEN);break;
			case 6:LCD_Clear(CYAN);break; 
			case 7:LCD_Clear(YELLOW);break;
			case 8:LCD_Clear(BRRED);break;
			case 9:LCD_Clear(GRAY);break;
			case 10:LCD_Clear(LGRAY);break;
			case 11:LCD_Clear(BROWN);break;
		}
		POINT_COLOR=RED;	  
		LCD_ShowString(30,40,210,24,24,"Elite STM32F1 ^_^"); 
		LCD_ShowString(30,70,200,16,16,"TFTLCD TEST");
		LCD_ShowString(30,90,200,16,16,"ATOM@ALIENTEK");
 		LCD_ShowString(30,110,200,16,16,lcd_id);		//��ʾLCD ID	      					 
		LCD_ShowString(30,130,200,12,12,"2015/1/14");	      					 
	    x++;
		if(x==12)x=0;
		LED0=!LED0;				   		 
		delay_ms(1000);	
		delay_ms(1000);
	} 
}
 
#endif

