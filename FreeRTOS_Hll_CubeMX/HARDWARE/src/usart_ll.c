/**
 * @description: 
 * @return {*}
 * @LastEditTime: Do not edit
 */
#include "usart_ll.h"

/* 重定向printf-usart3 */
int fputc(int ch, FILE *f)											
{
	LL_USART_TransmitData8(USART3,ch);
	while(!LL_USART_IsActiveFlag_TC(USART3))
	{
	}
	return ch;
}

