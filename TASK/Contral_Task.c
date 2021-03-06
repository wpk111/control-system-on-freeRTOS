#include "contral_task.h"
#include "myiicg.h"
#include "FreeRTOS.h"
#include "task.h"
#include "myiicg.h"
#include "delay.h"
#include "adc.h"
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
/*

控制任务：

1、启动气体微泵
2、启动高压控制
3、启动方波信号并进行数据采集
4、结束方波信号
5、结束高压控制
6、停止气体微泵


*/



 
 
extern QueueHandle_t Touch_Queue;  


void control_task(void *pvParameters)
{
	u8 flag;
    while(1)
    { vTaskDelay(1000);
			if(xQueueReceive(Touch_Queue,&flag,portMAX_DELAY))
			{
				LCD_Clear(WHITE);
				Show_Str(270,450,200,24,"返回",24,1);
				AIR_PUMP_ON;
				
				vTaskDelay(500);
				HIGH_VOLTAGE_CONTROL_ON;
				voltage_control(0x90,255);
				Show_Str(70,20,200,24,"P3输出5V",24,0);
				
				vTaskDelay(500);
				SQUARE_WAVE_AND_DATA_COLLECTION;
				
			
				vTaskDelay(500);
				HIGH_VOLTAGE_CONTROL_OFF;
				Show_Str(70,20,200,24,"P3输出0V",24,0);
				
				vTaskDelay(500);
				AIR_PUMP_OFF; 
				//LCD_Clear(RED);
				//LCD_ShowxNum(30,250,data[90],1,16,0);
			}
			
    }
} 




//初始化控制引脚，包括控制气体微泵的GPIO（PA4），控制高压和方波的IIC(PG6,PG7)
void control_init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOG, ENABLE);	 //使能PB,PE端口时钟
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LED0-->PB.5 端口配置
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_4);


	/*IIC   init*/
	//GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOG, ENABLE );	//使能GPIOB时钟
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_SetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 输出高
}



//控制气泵
void	air_pump_control(u8 sta)  //传递不同的参数0或1实现气泵开关，
{
	
	AIR_GPIO = sta;
	
}





void	voltage_control(u8 addr,u8 v)  //电压或方波控制函数，根据地地址address的值设置不同的PCF8591，实现不同的电压控制
{
		IIC_StartG();  
		IIC_Send_ByteG(addr);	    //发送地址
		IIC_Wait_AckG();
		IIC_Send_ByteG(0X40);//发送DA控制指令
	  IIC_Wait_AckG();	   
    IIC_Send_ByteG(v);   //发送DA值
	  IIC_Wait_AckG(); 	 										  		    	   
    IIC_StopG();//
	  delay_ms(10);
}

void data_collection()
{	
	int i =0;
	int j=0;
	u8 v=255;
	u16 temp = 0XAA;
	u16 data[300] = {0};
	while(i<300)
	{
		
		Show_Str(70,50,200,24,"P2输出5V",24,0);

	  for(j=0;j<50;j++,i++)
		{
			voltage_control(0x92,v-=5);
		  data[i] = Get_Adc(1);			
		
			LCD_Fast_DrawPoint(i,data[i]*33/4096+200,RED);
			LCD_ShowxNum(4*j,100,(data[i]),4,16,0);		
		}				
		Show_Str(70,50,200,24,"P2输出1V",24,0);
	  for(j=0;j<50;j++,i++)
		{
		  voltage_control(0x92,v+=5);
			data[i] = Get_Adc(1);
			LCD_Fast_DrawPoint(i,data[i]*33/4096+200,RED);
			LCD_ShowxNum(4*j,130,data[i],4,16,0);					
		}
	}
	STMFLASH_Write(0X08070020,&temp,1);//置标志位，说明已经有采集到的数据存�
	STMFLASH_Write(0X08070024,data,300);//把采集的数据保存到flash上，以备查看和上传到PC机
	
}


