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
 ALIENTEK 精英STM32F103开发板 FreeRTOS实验2-1
 FreeRTOS移植实验-库函数版本
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/


#define AAA 0

u8 showplace = 1;//1表示主界面；2表示数据采集界面；3表示数据传输界面




extern int data[2000];


//任务优先级
#define START_TASK_PRIO		1
//任务堆栈大小	
#define START_STK_SIZE 		128  
//任务句柄
TaskHandle_t StartTask_Handler;
//任务函数
void start_task(void *pvParameters);

//任务优先级
#define LED0_TASK_PRIO		2
//任务堆栈大小	
#define LED0_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED0Task_Handler;
//任务函数
void led0_task(void *pvParameters);

//任务优先级
#define LED1_TASK_PRIO		3
//任务堆栈大小	
#define LED1_STK_SIZE 		50  
//任务句柄
TaskHandle_t LED1Task_Handler;
//任务函数
void led1_task(void *pvParameters);


//按键消息队列的数量
#define TOUCHMSG_Q_NUM    1  		//触屏消息队列的数量  

QueueHandle_t Touch_Queue;   		//触屏消息队列句柄



#if !(AAA)

int main(void)
{
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);//设置系统中断优先级分组4	 
	delay_init();	    				//延时函数初始化	  
	uart_init(115200);					//初始化串口
	LED_Init();	//初始化LED
	font_init();
  LCD_Init();
	TP_Init();
	SHOW_Init();
	Adc_Init();
	control_init();
	 
	//创建开始任务
    xTaskCreate((TaskFunction_t )start_task,            //任务函数
                (const char*    )"start_task",          //任务名称
                (uint16_t       )START_STK_SIZE,        //任务堆栈大小
                (void*          )NULL,                  //传递给任务函数的参数
                (UBaseType_t    )START_TASK_PRIO,       //任务优先级
                (TaskHandle_t*  )&StartTask_Handler);   //任务句柄              
    vTaskStartScheduler();          //开启任务调度
}

#endif




//开始任务任务函数
void start_task(void *pvParameters)
{
    taskENTER_CRITICAL();           //进入临界区
	
		Touch_Queue = xQueueCreate(TOUCHMSG_Q_NUM,sizeof(u8));
	  if(Touch_Queue!=NULL)   	//消息队列Key_Queue创建成功
	
    //创建LED0任务
    xTaskCreate((TaskFunction_t )led0_task,     	
                (const char*    )"led0_task",   	
                (uint16_t       )LED0_STK_SIZE, 
                (void*          )NULL,				
                (UBaseType_t    )LED0_TASK_PRIO,	
                (TaskHandle_t*  )&LED0Task_Handler);   
    //创建LED1任务
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
										
    vTaskDelete(StartTask_Handler); //删除开始任务
    taskEXIT_CRITICAL();            //退出临界区
}

//LED0任务函数 
void led0_task(void *pvParameters)
{
    while(1)
    {
        LED0=~LED0;
        vTaskDelay(500);
    }
}   

//LED1任务函数
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
	u16 vp=2000,vn=1000;//设置方波输出电压高低初始值为2v和1v

	while(1)
	{
		TP_Scan(0);//扫描屏幕坐标.返回值:当前触屏状态:0,触屏无触摸;1,触屏有触摸;并记录触摸点坐标x和y。
	  TOUCH_key_v(&vp,&vn);//触摸处闪烁一下并处理触摸结果
	  vTaskDelay(100);
		
	}
}


#if  AAA

int main(void)
 {	 
 	u8 x=0;
	u8 v1=0xff;
	u8 v2=0;
	u8 lcd_id[12];			//存放LCD ID字符串
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 	//串口初始化为115200
 	LED_Init();			     //LED端口初始化
	LCD_Init();
	
	control_init();
	 
	AIR_GPIO = 0; 
	POINT_COLOR=RED;
	sprintf((char*)lcd_id,"LCD ID:%04X",lcddev.id);//将LCD ID打印到lcd_id数组。 
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
 		LCD_ShowString(30,110,200,16,16,lcd_id);		//显示LCD ID	      					 
		LCD_ShowString(30,130,200,12,12,"2015/1/14");	      					 
	    x++;
		if(x==12)x=0;
		LED0=!LED0;				   		 
		delay_ms(1000);	
		delay_ms(1000);
	} 
}
 
#endif

