#include "tim.h"
#include "bsp_encoder.h"

#include <stdio.h>
#include <string.h>

extern TIM_HandleTypeDef htim4;

void BSP_ENCODER_init(void)
{
	HAL_TIM_Encoder_Start(&htim4, TIM_CHANNEL_ALL);
}


#define FILTER_WINDOW_SIZE 5
int32_t encoder_filter_moving_average(int32_t new_value) {
    static int32_t buffer[FILTER_WINDOW_SIZE] = {0};
    static uint8_t index = 0;
    static int32_t sum = 0;
    
    sum -= buffer[index];
    buffer[index] = new_value;
    sum += new_value;
    
    index = (index + 1) % FILTER_WINDOW_SIZE;
    return sum / FILTER_WINDOW_SIZE;
}
#define MEDIAN_FILTER_SIZE 3
uint16_t encoder_filter_median(uint16_t new_value) {
    static uint16_t buffer[MEDIAN_FILTER_SIZE];
    static uint8_t index = 0;
    
    buffer[index] = new_value;
    index = (index + 1) % MEDIAN_FILTER_SIZE;
    
    // 实现简易排序找中值
    uint16_t temp[MEDIAN_FILTER_SIZE];
    memcpy(temp, buffer, sizeof(temp));
    for(int i=0; i<MEDIAN_FILTER_SIZE-1; i++) {
        for(int j=i+1; j<MEDIAN_FILTER_SIZE; j++) {
            if(temp[i] > temp[j]) {
                uint16_t swap = temp[i];
                temp[i] = temp[j];
                temp[j] = swap;
            }
        }
    }
    return temp[MEDIAN_FILTER_SIZE/2];
}
void BSP_ENCODER_setCounter(uint32_t cnt)
{
    __HAL_TIM_SET_COUNTER(&htim4,cnt);
}
unsigned short BSP_getENCODER(void)
{
   uint16_t median_filtered;     
   median_filtered = encoder_filter_median(__HAL_TIM_GET_COUNTER(&htim4));
   return encoder_filter_moving_average(median_filtered);
}

uint16_t BSP_ENCODER_getSteps(uint16_t prev, uint16_t curr) 
{
    uint16_t delta = (curr > prev) ? (curr - prev) : (prev - curr);
    return (delta <= 32767) ? delta : (65535 - delta);
}

