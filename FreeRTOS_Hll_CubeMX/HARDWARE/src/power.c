#include "power.h"


//24V���
void Power_Ctrl_Init(void)
{
	POWER1_CTRL_ON;
	POWER2_CTRL_ON;
	POWER3_CTRL_ON;
	POWER4_CTRL_ON;
}
