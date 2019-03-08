#include "stm32f10x.h"
#include "led.h"

void LED_Init(void)
{
	GPIO_InitTypeDef GPIO_Initstrue;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);
	
	GPIO_Initstrue.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_Initstrue.GPIO_Pin = GPIO_Pin_13;
	GPIO_Initstrue.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_Initstrue);
	
	GPIO_SetBits(GPIOC,GPIO_Pin_13);
}




