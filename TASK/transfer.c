#include "transfer.h"
#include "usart.h"	
#include "lcd.h"
#include "text.h"
#include "stmflash.h"

void transfer()
{
	u16 data = 0;
	u16 temp = 0;
	u16 t = 0;
	u32 addr= 0X08070024;
	u16 x = 0;
		STMFLASH_Read(0X08070020,&temp,1);
		if(temp == 0xAA)
		{
			
			for(t=0;t<300;t++)
			{
				STMFLASH_Read(addr,&data,1);
				if(x == 10)
					x=0;
				LCD_ShowxNum(32*x++,t/10*20,data&0xff,4,16,0);
				USART_SendData(USART1, data>>8);//向串口1发送数据高8位
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
				USART_SendData(USART1, data);//向串口1发送数据低8位
				while(USART_GetFlagStatus(USART1,USART_FLAG_TC)!=SET);//等待发送结束
				LCD_ShowxNum(30,250,t/3,3,16,0);
				addr +=2;
			}
		}
		else{
		Show_Str(30,50,100,30,"没有数据",16,1);
		
		}
		
}


		

