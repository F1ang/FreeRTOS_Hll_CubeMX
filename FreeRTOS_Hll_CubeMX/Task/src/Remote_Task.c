#include "Remote_Task.h"

#include "led.h"
#include "usart_ll.h"
#include "imu.h"  
#include "tim_ll.h"

extern osSemaphoreId_t Remote_SemaHandle; 		/* 二值信号量 */ 
extern osThreadId_t CHASSISHandle;						/* 任务通知句柄 */

void Remote_Task(void *argument)
{
  /* USER CODE BEGIN Remote_Task */
	mpu_device_init();
	init_quaternion();
	mpu_offset_call();
	tim7_base_init();
  /* Infinite loop */
  for(;;)
  {
		if(xSemaphoreTake(Remote_SemaHandle,portMAX_DELAY) == pdTRUE) /* 获取信号量 */
		{
			mpu_get_data();
			imu_ahrs_update();
			imu_attitude_update();
			imu_compensation(1);
			printf("yaw %f,pitch %f,roll %f\r\n",imu_com.yaw,imu_com.pit,imu_com.rol);
			LED_FLOW_ON(2);	

			xTaskNotify(CHASSISHandle, 5, eSetValueWithOverwrite);	 /* 发送任务通知 */		
		}
		osDelay(5);
  }
  /* USER CODE END Remote_Task */
}





