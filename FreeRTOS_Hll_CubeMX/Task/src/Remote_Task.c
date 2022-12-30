#include "Remote_Task.h"

#include "led.h"
#include "usart_ll.h"
#include "imu.h"  
#include "tim_ll.h"

extern osSemaphoreId_t Remote_SemaHandle; 		/* ��ֵ�ź��� */ 
extern osThreadId_t CHASSISHandle;						/* ����֪ͨ��� */

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
		if(xSemaphoreTake(Remote_SemaHandle,portMAX_DELAY) == pdTRUE) /* ��ȡ�ź��� */
		{
			mpu_get_data();
			imu_ahrs_update();
			imu_attitude_update();
			imu_compensation(1);
			printf("yaw %f,pitch %f,roll %f\r\n",imu_com.yaw,imu_com.pit,imu_com.rol);
			LED_FLOW_ON(2);	

			xTaskNotify(CHASSISHandle, 5, eSetValueWithOverwrite);	 /* ��������֪ͨ */		
		}
		osDelay(5);
  }
  /* USER CODE END Remote_Task */
}





