#include "lcd.h"
#include "text.h"
#include "touch.h" 
#include "lcd.h"
#include "delay.h"
#include "stdlib.h"
#include "math.h"
#include "24cxx.h" 
#include "text.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "stmflash.h"
#include "task.h"
#include "contral_task.h"
#include "transfer.h"


#define CONTROL_TASK_PRIO		3
//任务堆栈大小	
#define CONTROL_STK_SIZE 		240  
//任务句柄
TaskHandle_t ControlTask_Handler;
//任务函数
void control_task(void *pvParameters);


extern QueueHandle_t Touch_Queue; 
extern u8 showplace;

SHOW_Init()
{
	LCD_Fill(0,0,319,479,WHITE);
  POINT_COLOR=BLUE;
	Show_Str(70,20,200,24,"气溶胶检测系统",24,0);
 	POINT_COLOR=LIGHTGREEN;		 //设置字体为
	
	
	LCD_Fill(0,440,50,480,BROWN);
	LCD_Fill(270,440,380,480, BROWN);
	Show_Str(0,450,200,24,"开始",24,1);
	Show_Str(270,450,200,24,"保存",24,1);
	Show_Str(200-10,450,200,24,"传输",24,1);
	Show_Str(80,450,200,24,"设置",24,1);
	
	LCD_Fill(70,270,100,290,BROWN);
	LCD_Fill(110,270,140,290,BROWN);
	Show_Str(0,270,200,24,"高压： +  - ",24,1);
	LCD_Fill(220,270,260,290, BROWN);
	LCD_Fill(270,270,310,290, BROWN);
	Show_Str(160,270,200,24,"低压：+   - ",24,1);
	
	
	
	Show_Str(30,190,200,16,"粒径:  ",16,1);	 
  Show_Str(130,190,200,16,"浓度:  ",16,1);	 
  LCD_ShowString(39,250,200,16,16,".  V  .  V");
  Show_Str(30,230,200,16,"输出方波高低电压",16,1);

	//Show_Str(30,400,200,16,"可采样2k次，采样周期20us",16,1);	
  //Show_Str(30,360,200,16,"温度:  ℃",16,1);	 
 	//Show_Str(110,360,200,16,"湿度:  %",16,1);


}


void TOUCH_key_v(u16 *vp,u16 * vn)//把触摸点闪烁一下，并处理触摸结果
{
		u8 flag =0;
	 BaseType_t err;

   if(PEN == 0){
		 switch(showplace)
		 {
			 case 1:
						 { 
							 if(tp_dev.x[0]>280 && tp_dev.y[0]>440 && !(tp_dev.x[0]==0xffff)){
								
								LCD_Fill(270,440,320,480,WHITE);
								delay_ms(500);
							
								LCD_Fill(270,440,320,480,LBBLUE);
								Show_Str(270,450,200,24,"保存",24,1);
								showplace = 1;
								
								}
							
							if(tp_dev.x[0]<40 && tp_dev.y[0]>440)
								{
									LCD_Fill(0,440,50,480,WHITE);
									delay_ms(200);
									LCD_Fill(0,440,50,480,LBBLUE);
									Show_Str(0,450,200,24,"开始",24,1);
									flag = 1;
									err=xQueueSend(Touch_Queue,&flag,10);//发送消息队列
									showplace = 2;
									xTaskCreate((TaskFunction_t )control_task,     
															(const char*    )"control_task",   
															(uint16_t       )CONTROL_STK_SIZE, 
															(void*          )NULL,
															(UBaseType_t    )CONTROL_TASK_PRIO,
															(TaskHandle_t*  )&ControlTask_Handler);
									
								}
								
								else if(tp_dev.x[0]>190&&tp_dev.x[0]<270&&tp_dev.y[0]>440){//传输数据
									
										LCD_Clear(WHITE);
										transfer();
										showplace = 3;
									
								}
								
							else if(tp_dev.x[0]<100&&tp_dev.x[0]>60&&tp_dev.y[0]<290&&tp_dev.y[0]>270)
								{
										LCD_Fill(70,270,100,290,WHITE);
										delay_ms(200);
										LCD_Fill(70,270,100,290,LBBLUE);
										Show_Str(0,270,200,24,"       +   ",24,1);
										*vp=*vp+100;
										LCD_ShowxNum(30,250,*vp/1000,1,16,0);
										LCD_ShowxNum(30+12,250,*vp/10-(*vp/1000)*100,2,16,0);
								 }
							else if(tp_dev.x[0]<140&&tp_dev.x[0]>100&&tp_dev.y[0]<290&&tp_dev.y[0]>270)										
								{  LCD_Fill(110,270,140,290,WHITE);
									 delay_ms(200);
									 LCD_Fill(110,270,140,290,LBBLUE);
									 Show_Str(0,270,200,24,"          -   ",24,1);
									 *vp=*vp-100;
									 LCD_ShowxNum(30,250,*vp/1000,1,16,0);
									 LCD_ShowxNum(30+12,250,*vp/10-(*vp/1000)*100,2,16,0);	 
								}
							else if(tp_dev.x[0]<270&&tp_dev.x[0]>220&&tp_dev.y[0]<290&&tp_dev.y[0]>270)
								{
									LCD_Fill(220,270,260,290,WHITE);
									delay_ms(200);
									LCD_Fill(220,270,260,290,LBBLUE);
									Show_Str(235,270,200,24,"+",24,1);
									*vn=*vn+100;
									LCD_ShowxNum(80,250,*vn/1000,1,16,0);
									LCD_ShowxNum(90,250,*vn/10-(*vn/1000)*100,2,16,0);
								}
							else if(tp_dev.x[0]<320&&tp_dev.x[0]>270&&tp_dev.y[0]<290&&tp_dev.y[0]>270)
								{  LCD_Fill(270,270,315,290,WHITE);
									 delay_ms(200);
									 LCD_Fill(270,270,315,290,LBBLUE);
									 Show_Str(290,270,200,24,"-",24,1);	
									 *vn=*vn-100;
									 LCD_ShowxNum(80,250,*vn/1000,1,16,0);
									 LCD_ShowxNum(90,250,*vn/10-(*vn/1000)*100,2,16,0);
								}
							else showplace = 1;	
							
							}
						 break;
			 case 2:
				 if(tp_dev.x[0]>270&&tp_dev.y[0]>440){
					 vTaskDelete(ControlTask_Handler); //
					 SHOW_Init();
					 showplace = 1;
				 }
					break;
			 case 3:	
					if(tp_dev.x[0]>270&&tp_dev.y[0]>440){
					 SHOW_Init();
					 showplace = 1;
				 }
					break;					
					
	   }
	 }
}

