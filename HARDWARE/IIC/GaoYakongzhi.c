#include "GaoYakongzhi.h"
#include "lcd.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "myiic.h"
#include "touch.h"
#include "w25qxx.h"
#include "text.h"
#include "show.h"
#include "led.h"
#include "myiicg.h"

void gaoyakongzhi(u8 *dav,u8 *dan){
	IIC_InitG();
	
	LCD_Fill(0,0,319,479,WHITE);
	
	Show_Str(10,270,200,24,"���ø�ѹ��ѹ��+ ",24,1);
	Show_Str(225,270,200,24,"-",24,1);
	Show_Str(270,450,200,24,"����",24,1);
	LCD_Draw_Circle(184,282,10);
	LCD_Draw_Circle(230,282,10);
	
	Show_Str(30,200,200,24,"��ѹ��  V",24,1);	
	LCD_ShowNum(88,200,(u32)(*dav),2,24);
	Show_Str(96,200,200,24,".",24,1);	
	
	
	while(1){
	
	tp_dev.scan(0);//������ɨ��

	
	 
	
     if(tp_dev.x[0]<210&&tp_dev.x[0]>170&&tp_dev.y[0]<290&&tp_dev.y[0]>270)//KEY_UP����
		 {
			 LCD_Fill(170,270,200,290,WHITE);
			 delay_ms(100);
			 Show_Str(179,270,200,24,"+",24,1);
			 LCD_Draw_Circle(184,282,10);
		
			
			(*dav)++;
			 if(*dav>51){
				 *dav=0;
			 }
				
			*dan=(*dav)*5;
			
		  IIC_StartG();  
	
		  IIC_Send_ByteG(0X90);	    //���͵�ַ
		  IIC_Wait_AckG();
		  IIC_Send_ByteG(0X40);//����DA����ָ��
 	 
	    IIC_Wait_AckG();	   
      IIC_Send_ByteG(*dan);   //����DAֵ
	    IIC_Wait_AckG(); 	 										  		    	   
      IIC_StopG();//
	    delay_ms(10);
			
			
		  LCD_ShowNum(88,200,(u32)((*dav)-3),2,24);
    	Show_Str(96,200,200,24,".",24,1);	
	 	}
    if(tp_dev.x[0]<270&&tp_dev.x[0]>220&&tp_dev.y[0]<290&&tp_dev.y[0]>270)//KEY1����
		{
			 LCD_Fill(220,270,240,290,WHITE);
			 delay_ms(100);
			 Show_Str(225,270,200,24,"-",24,1);
			 LCD_Draw_Circle(230,282,10);
 		
					
			(*dav)--;
			if(*dav==1){
				(*dav)=51;
			}
			*dan=(*dav)*5;
			
			IIC_StartG();  
	
	   	IIC_Send_ByteG(0X90);	    //���͵�ַ
	  	IIC_Wait_AckG();
	  	IIC_Send_ByteG(0X40);//����AD����ָ��
 	 
    	IIC_Wait_AckG();	   
      IIC_Send_ByteG(*dan);   //����ADת������
	    IIC_Wait_AckG(); 	 										  		   		    	   
      IIC_StopG();
	    delay_ms(10);
			
			
		
			LCD_ShowNum(88,200,(u32)((*dav)-3),2,24);
    	Show_Str(96,200,200,24,".",24,1);	
			
		}
		delay_ms(10);  
	  LED0=!LED0;//��ʾϵͳ��������	
	
		
		if(tp_dev.x[0]<319&&tp_dev.x[0]>250&&tp_dev.y[0]<479&&tp_dev.y[0]>440)
			break;
	}
	
	
	LCD_Fill(0,0,319,479,WHITE);
	Show_Str(10,40,200,24,"���ø�ѹģ��",24,1);
	Show_Str(10,70,200,24,"ʵʱ��ʾ�ɼ����ĵ�ѹ����Ӧ�ĵ���",24,1);
	Show_Str(202,70,200,24,"��ѹ����Ӧ�ĵ���",24,1);
	Show_Str(10,100,200,24,"�ĵ���",24,1);
	Show_Str(10,130,200,24,"����",24,1);
	}






