#include "lcd.h"
#include "w25qxx.h"
#include "showrtad.h"
#include "touch.h"
#include "show.h"
#include "set.h"
#include "GaoYakongzhi.h"
#include "showrtad.h"
#include "text.h"




void set(u8 *dav,u8 *dan){

	LCD_Fill(0,0,319,479,WHITE);
	Show_Str(10,40,200,24,"���ø�ѹģ��",24,1);
	Show_Str(10,70,200,24,"ʵʱ��ʾ�ɼ����ĵ�ѹ����Ӧ�ĵ���",24,1);
	Show_Str(202,70,200,24,"��ѹ����Ӧ�ĵ���",24,1);
	Show_Str(10,100,200,24,"�ĵ���",24,1);
	Show_Str(10,130,200,24,"����",24,1);
	while(1){
		tp_dev.scan(0);//������ɨ��	
		if(tp_dev.y[0]<70&&tp_dev.y[0]>20){
			gaoyakongzhi((u8 *)&dav,(u8 *)&dan);
			
		}
		if(tp_dev.y[0]<130&&tp_dev.y[0]>70){
			showrtad();
			
		}
		if(tp_dev.y[0]<160&&tp_dev.y[0]>130){
			break;
		}
		
	}
	SHOW_Init();

}




