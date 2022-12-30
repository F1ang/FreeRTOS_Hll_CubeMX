/*
 * @Description: 
 
 * @Author: YiPei_Fang
 * @Date: 2022-09-26 00:24:27
 */
#include "led.h"
#include "cmsis_os.h"

void Led_Flow_On(void)
{
	LED_GREEN_ON;
	//LL_mDelay(30);/* 莫名的BUG，推荐延时用HAL_Delay() */
	HAL_Delay(30);
	LED_RED_ON;
	HAL_Delay(30);
	for(int i=0; i<8; i++)
	{
		LED_FLOW_ON(i);
		HAL_Delay(30);
	}
}

void Led_Flow_Off(void)
{
	for(uint8_t i=0; i<8; i++)
	{
		LED_FLOW_OFF(i);
		HAL_Delay(30);
	}
	LED_RED_OFF;
	HAL_Delay(30);
	LED_GREEN_OFF;
	HAL_Delay(30);
}
void Led_Init(void)
{
	LED_GREEN_OFF;
	LED_RED_OFF;
	LED_FLOW_OFF_PIN(LL_GPIO_PIN_1 |LL_GPIO_PIN_2 | LL_GPIO_PIN_3 | LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_6 | LL_GPIO_PIN_7 | LL_GPIO_PIN_8);
	Led_Flow_On();
	Led_Flow_Off();
}


