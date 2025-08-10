#ifndef _BSP_ENCODER_H_
#define _BSP_ENCODER_H_

void BSP_ENCODER_init(void);
unsigned short BSP_getENCODER(void);
void BSP_ENCODER_setCounter(unsigned int);
unsigned short BSP_ENCODER_getSteps(unsigned short prev, unsigned short curr);
#endif 
