#include "Chassis_Task.h"

#include "led.h"

extern osThreadId_t CHASSISHandle;						/* 任务通知句柄 */

void Chassis_Take(void *argument)
{
  /* USER CODE BEGIN Chassis_Tak */
	uint32_t pulNotificationValue;
  /* Infinite loop */
  for(;;)
  {
		if (xTaskNotifyWait(0, 0xffffffff, &pulNotificationValue, portMAX_DELAY) == pdTRUE){  
			LED_GREEN_TOGGLE;
			LED_FLOW_ON(pulNotificationValue); //任务通知值为5
		}
		osDelay(1); 
  }
  /* USER CODE END Chassis_Tak */
}




