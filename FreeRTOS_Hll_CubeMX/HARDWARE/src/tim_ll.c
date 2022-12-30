/*
 * @Description: 
 * @LastEditTime: 2022-12-27 18:06:09
 */
#include "tim_ll.h"
#include "cmsis_os.h"
#include "semphr.h"
#include "led.h"
/**
 * @description: 初始化更新tim7
 * @return {*}
 * @LastEditTime: Do not edit
 */
void tim7_base_init(void)
{
	LL_TIM_EnableIT_UPDATE(TIM7); 
	LL_TIM_EnableCounter(TIM7); 
}


extern osSemaphoreId_t Remote_SemaHandle;  		/* 二值信号量 */

/**
 * @description: tim7回调函数
 * @return {*}
 * @LastEditTime: Do not edit
 */
void TIM7_IRQHandler_CallBack(void)
{
	BaseType_t xHigherPriorityTaskWoken;
	if(LL_TIM_IsActiveFlag_UPDATE(TIM7))
	{
		LL_TIM_ClearFlag_UPDATE(TIM7);
		LED_FLOW_ON(3);
		/* 发送二值信号量：ISR级 */
		xSemaphoreGiveFromISR(Remote_SemaHandle, &xHigherPriorityTaskWoken);
		portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
		
	}
}


