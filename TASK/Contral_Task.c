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

¿ØÖÆÈÎÎñ£º

1¡¢Æô¶¯ÆøÌåÎ¢±Ã
2¡¢Æô¶¯¸ßÑ¹¿ØÖÆ
3¡¢Æô¶¯·½²¨ĞÅºÅ²¢½øĞĞÊı¾İ²É¼¯
4¡¢½áÊø·½²¨ĞÅºÅ
5¡¢½áÊø¸ßÑ¹¿ØÖÆ
6¡¢Í£Ö¹ÆøÌåÎ¢±Ã


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
				Show_Str(270,450,200,24,"·µ»Ø",24,1);
				AIR_PUMP_ON;
				
				vTaskDelay(500);
				HIGH_VOLTAGE_CONTROL_ON;
				voltage_control(0x90,255);
				Show_Str(70,20,200,24,"P3Êä³ö5V",24,0);
				
				vTaskDelay(500);
				SQUARE_WAVE_AND_DATA_COLLECTION;
				
			
				vTaskDelay(500);
				HIGH_VOLTAGE_CONTROL_OFF;
				Show_Str(70,20,200,24,"P3Êä³ö0V",24,0);
				
				vTaskDelay(500);
				AIR_PUMP_OFF; 
				//LCD_Clear(RED);
				//LCD_ShowxNum(30,250,data[90],1,16,0);
			}
			
    }
} 




//³õÊ¼»¯¿ØÖÆÒı½Å£¬°üÀ¨¿ØÖÆÆøÌåÎ¢±ÃµÄGPIO£¨PA4£©£¬¿ØÖÆ¸ßÑ¹ºÍ·½²¨µÄIIC(PG6,PG7)
void control_init(void)
{
 GPIO_InitTypeDef  GPIO_InitStructure;
 	
 RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOG, ENABLE);	 //Ê¹ÄÜPB,PE¶Ë¿ÚÊ±ÖÓ
	
 GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;				 //LED0-->PB.5 ¶Ë¿ÚÅäÖÃ
 GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //ÍÆÍìÊä³ö
 GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO¿ÚËÙ¶ÈÎª50MHz
 GPIO_Init(GPIOA, &GPIO_InitStructure);					 //¸ù¾İÉè¶¨²ÎÊı³õÊ¼»¯GPIOB.5
 GPIO_SetBits(GPIOB,GPIO_Pin_4);


	/*IIC   init*/
	//GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOG, ENABLE );	//Ê¹ÄÜGPIOBÊ±ÖÓ
	   
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP ;   //ÍÆÍìÊä³ö
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOG, &GPIO_InitStructure);
	GPIO_SetBits(GPIOG,GPIO_Pin_6|GPIO_Pin_7); 	//PB6,PB7 Êä³ö¸ß
}



//¿ØÖÆÆø±Ã
void	air_pump_control(u8 sta)  //´«µİ²»Í¬µÄ²ÎÊı0»ò1ÊµÏÖÆø±Ã¿ª¹Ø£¬
{
	
	AIR_GPIO = sta;
	
}





void	voltage_control(u8 addr,u8 v)  //µçÑ¹»ò·½²¨¿ØÖÆº¯Êı£¬¸ù¾İµØµØÖ·addressµÄÖµÉèÖÃ²»Í¬µÄPCF8591£¬ÊµÏÖ²»Í¬µÄµçÑ¹¿ØÖÆ
{
		IIC_StartG();  
		IIC_Send_ByteG(addr);	    //·¢ËÍµØÖ·
		IIC_Wait_AckG();
		IIC_Send_ByteG(0X40);//·¢ËÍDA¿ØÖÆÖ¸Áî
	  IIC_Wait_AckG();	   
    IIC_Send_ByteG(v);   //·¢ËÍDAÖµ
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
		
		Show_Str(70,50,200,24,"P2Êä³ö5V",24,0);

	  for(j=0;j<50;j++,i++)
		{
			voltage_control(0x92,v-=5);
		  data[i] = Get_Adc(1);			
		
			LCD_Fast_DrawPoint(i,data[i]*33/4096+200,RED);
			LCD_ShowxNum(4*j,100,(data[i]),4,16,0);		
		}				
		Show_Str(70,50,200,24,"P2Êä³ö1V",24,0);
	  for(j=0;j<50;j++,i++)
		{
		  voltage_control(0x92,v+=5);
			data[i] = Get_Adc(1);
			LCD_Fast_DrawPoint(i,data[i]*33/4096+200,RED);
			LCD_ShowxNum(4*j,130,data[i],4,16,0);					
		}
	}
	STMFLASH_Write(0X08070020,&temp,1);//ÖÃ±êÖ¾Î»£¬ËµÃ÷ÒÑ¾­ÓĞ²É¼¯µ½µÄÊı¾İ´æ´
	STMFLASH_Write(0X08070024,data,300);//°Ñ²É¼¯µÄÊı¾İ±£´æµ½flashÉÏ£¬ÒÔ±¸²é¿´ºÍÉÏ´«µ½PC»ú
	
}


