#include "kalman.h"


/* Ð±ÆÂÂË²¨Æ÷:Output_value£¬²½³¤£¬Remote_value */
void Ramp_fun(float *Output_value, int Pace, float Remote_value)
{
	if(Remote_value>*Output_value)
	{
		*Output_value += Pace;
		if(*Output_value>Remote_value)
		{
			*Output_value=Remote_value;
		}
	}
	else if(Remote_value<*Output_value)
	{
		*Output_value -= Pace;
		if(*Output_value<Remote_value)
		{
			*Output_value=Remote_value;
		}
	}
	else *Output_value=Remote_value;
}



