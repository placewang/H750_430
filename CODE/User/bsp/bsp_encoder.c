#include "tim.h"
#include "bsp_encoder.h"

#include <stdio.h>
#include <string.h>

extern TIM_HandleTypeDef htim4;

void BSP_ENCODER_init(void)
{
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}

unsigned short BSP_ENCODER_getCounter(void)
{
	return __HAL_TIM_GET_COUNTER(&htim4);
}
void BSP_ENCODER_setCounter(uint32_t cnt)
{
    __HAL_TIM_SET_COUNTER(&htim4,cnt);
}

