#ifndef __MOTOR_H
#define __MOTOR_H
#include "sys.h"

#define Motor_CLK 		RCC_APB2Periph_GPIOB
#define Motor_GPIO 		GPIOB
#define AA 				GPIO_Pin_12
#define BB 				GPIO_Pin_13
#define CC 				GPIO_Pin_14
#define DD 				GPIO_Pin_15
#define  Speed  6    //(1-20) //1最快 20最慢  速度快，扭矩小

void Motor_Init(void);
void Delay_xms(u8 x);
void Motorcw(void);
void Motorcw_setp(u8 s);
void Motorccw(void);
void MotorStop(void);

#endif
