/*
 * @Description: 
 * @LastEditTime: 2022-12-28 11:38:36
 */
#include "base_init.h"

void Init_Hardware(void)
{
	Led_Init();
	Power_Ctrl_Init();

	/*SPI5复用引脚*/
	LL_GPIO_SetAFPin_0_7(GPIOF, LL_GPIO_PIN_7, LL_GPIO_AF_5); 
	LL_GPIO_SetAFPin_8_15(GPIOF, LL_GPIO_PIN_8, LL_GPIO_AF_5);
	LL_GPIO_SetAFPin_8_15(GPIOF, LL_GPIO_PIN_9, LL_GPIO_AF_5);
	LL_SPI_Enable(SPI5);/* 第一步就是配置!!! */
	
	printf("Welcome to HLL\r\n");
}




