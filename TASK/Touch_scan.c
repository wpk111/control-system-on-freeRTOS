#include "touch.h"
#include "Touch_scan.h"
#include "delay.h"







void scan_task(void *pvParameters)
{
	u16 vp=2000,vn=1000;
	
	while(1)
	{
		TP_Scan(0);//ɨ����Ļ����.����ֵ:��ǰ����״̬:0,�����޴���;1,�����д���;����¼����������x��y��
	  TOUCH_key_v(&vp,&vn);//��������˸һ�²����������
	  vTaskDelay(500);
	}
	return 0;
}




