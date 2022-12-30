/*
 * @Description: 
 * @LastEditTime: 2022-12-28 10:43:51
 */
#ifndef _LED_HLL_H_
#define _LED_HLL_H_

#include "main.h"
#include "gpio.h"
/*LED*/
#define LED_RED_ON     		LL_GPIO_ResetOutputPin(GPIOE, LL_GPIO_PIN_11)
#define LED_RED_OFF    		LL_GPIO_SetOutputPin(GPIOE, LL_GPIO_PIN_11)
#define LED_RED_TOGGLE 		LL_GPIO_TogglePin(GPIOE,LL_GPIO_PIN_11)

#define LED_GREEN_ON    	LL_GPIO_ResetOutputPin(GPIOF, LL_GPIO_PIN_14)
#define LED_GREEN_OFF    	LL_GPIO_SetOutputPin(GPIOF, LL_GPIO_PIN_14)
#define LED_GREEN_TOGGLE 	LL_GPIO_TogglePin(GPIOF,LL_GPIO_PIN_14)

#define LED_FLOW_ON(led_num) 	    LL_GPIO_ResetOutputPin(GPIOG, (LL_GPIO_PIN_8 >> led_num))
#define LED_FLOW_OFF(led_num) 	    LL_GPIO_SetOutputPin(GPIOG, (LL_GPIO_PIN_8 >> led_num))
#define LED_FLOW_TOGGLE(led_num)    LL_GPIO_TogglePin(GPIOG,(LL_GPIO_PIN_8 >> led_num))

#define LED_FLOW_ON_PIN(GPIO_Pin) 		LL_GPIO_ResetOutputPin(GPIOG, GPIO_Pin)
#define LED_FLOW_OFF_PIN(GPIO_Pin) 		LL_GPIO_SetOutputPin(GPIOG, GPIO_Pin)
#define LED_FLOW_TOGGLE_PIN(GPIO_Pin)   LL_GPIO_TogglePin(GPIOG,GPIO_Pin)

void Led_Init(void);
void Led_Flow_On(void);
void Led_Flow_Off(void);

#endif



