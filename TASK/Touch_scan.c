#include "touch.h"
#include "Touch_scan.h"
#include "delay.h"







void scan_task(void *pvParameters)
{
	u16 vp=2000,vn=1000;
	
	while(1)
	{
		TP_Scan(0);//扫描屏幕坐标.返回值:当前触屏状态:0,触屏无触摸;1,触屏有触摸;并记录触摸点坐标x和y。
	  TOUCH_key_v(&vp,&vn);//触摸处闪烁一下并处理触摸结果
	  vTaskDelay(500);
	}
	return 0;
}




