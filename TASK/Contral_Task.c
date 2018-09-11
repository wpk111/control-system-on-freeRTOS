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

��������

1����������΢��
2��������ѹ����
3�����������źŲ��������ݲɼ�
4�����������ź�
5��������ѹ����
6��ֹͣ����΢��


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
				Show_Str(270,450,200,24,"����",24,1);
				AIR_PUMP_ON;
				
				vTaskDelay(500);
				HIGH_VOLTAGE_CONTROL_ON;
				voltage_control(0x90,255);
				Show_Str(70,20,200,24,"P3���5V",24,0);
				
				vTaskDelay(500);
				SQUARE_WAVE_AND_DATA_COLLECTION;
				
			
				vTaskDelay(500);
				HIGH_VOLTAGE_CONTROL_OFF;
				Show_Str(70,20,200,24,"P3���0V",24,0);
				
				vTaskDelay(500);
				AIR_PUMP_OFF; 
				//LCD_Clear(RED);
				//LCD_ShowxNum(30,250,data[90],1,16,0);
			}
			
    }
} 




//��ʼ���������ţ�������������΢�õ�GPIO��PA4�������Ƹ�ѹ�ͷ�����IIC(PG6,PG7)
void control_init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOG, ENABLE);	 //ʹ��PB,PE�˿�ʱ��
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LED0-->PB.5 �˿�����
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //�������
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO���ٶ�Ϊ50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //�����趨������ʼ��GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_4);


	/*IIC   init*/
	//GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOG, ENABLE );	//ʹ��GPIOBʱ��
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_SetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 �����
}



//��������
void	air_pump_control(u8 sta)  //���ݲ�ͬ�Ĳ���0��1ʵ�����ÿ��أ�
{
	
	AIR_GPIO = sta;
	
}





void	voltage_control(u8 addr,u8 v)  //��ѹ�򷽲����ƺ��������ݵص�ַaddress��ֵ���ò�ͬ��PCF8591��ʵ�ֲ�ͬ�ĵ�ѹ����
{
		IIC_StartG();  
		IIC_Send_ByteG(addr);	    //���͵�ַ
		IIC_Wait_AckG();
		IIC_Send_ByteG(0X40);//����DA����ָ��
	  IIC_Wait_AckG();	   
    IIC_Send_ByteG(v);   //����DAֵ
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
		
		Show_Str(70,50,200,24,"P2���5V",24,0);

	  for(j=0;j<50;j++,i++)
		{
			voltage_control(0x92,v-=5);
		  data[i] = Get_Adc(1);			
		
			LCD_Fast_DrawPoint(i,data[i]*33/4096+200,RED);
			LCD_ShowxNum(4*j,100,(data[i]),4,16,0);		
		}				
		Show_Str(70,50,200,24,"P2���1V",24,0);
	  for(j=0;j<50;j++,i++)
		{
		  voltage_control(0x92,v+=5);
			data[i] = Get_Adc(1);
			LCD_Fast_DrawPoint(i,data[i]*33/4096+200,RED);
			LCD_ShowxNum(4*j,130,data[i],4,16,0);					
		}
	}
	STMFLASH_Write(0X08070020,&temp,1);//�ñ�־λ��˵���Ѿ��вɼ��������ݴ�
	STMFLASH_Write(0X08070024,data,300);//�Ѳɼ������ݱ��浽flash�ϣ��Ա��鿴���ϴ���PC��
	
}


