#ifndef __TASK_T_
#define __TASK_T_
#include "sys.h"
#include "FreeRTOS.h"
#include "task.h"




#define AIR_GPIO PAout(4)// PA4

void	air_pump_control(u8 sta);
void	voltage_control(u8 addr,u8 v);
void  data_collection(void);
void control_init(void);

#define AIR_PUMP_ON air_pump_control(1)
			
#define	HIGH_VOLTAGE_CONTROL_ON voltage_control(0x90,255)
		
#define SQUARE_WAVE_AND_DATA_COLLECTION data_collection()

#define	HIGH_VOLTAGE_CONTROL_OFF voltage_control(0x90,0)

#define AIR_PUMP_OFF air_pump_control(0) 





#endif
