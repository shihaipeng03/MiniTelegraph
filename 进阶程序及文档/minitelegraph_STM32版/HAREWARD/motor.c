#include "motor.h"
#include "delay.h"

uint16_t phasecw[4] ={0x8000,0x4000,0x2000,0x1000};// D-C-B-A   
uint16_t phaseccw[4]={0x1000,0x2000,0x4000,0x8000};// A-B-C-D

void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(Motor_CLK,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = AA | BB | CC | DD ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(Motor_GPIO,&GPIO_InitStructure);
	GPIO_ResetBits(Motor_GPIO,AA | BB | CC | DD );
}

void Delay_xms(u8 x)
{
	u8 i,j;
	for(i=0;i<x;i++)
		for(j=0;j<112;j++);
}

void Motorcw(void)  
{  
	GPIO_ResetBits(Motor_GPIO,AA); 
	GPIO_ResetBits(Motor_GPIO,BB);
	GPIO_ResetBits(Motor_GPIO,CC);
	GPIO_SetBits(Motor_GPIO,DD);
    delay_ms(Speed); 
	GPIO_ResetBits(Motor_GPIO,AA); 
	GPIO_ResetBits(Motor_GPIO,BB);
	GPIO_SetBits(Motor_GPIO,CC);
	GPIO_ResetBits(Motor_GPIO,DD);
    delay_ms(Speed); 
	GPIO_ResetBits(Motor_GPIO,AA); 
	GPIO_SetBits(Motor_GPIO,BB);
	GPIO_ResetBits(Motor_GPIO,CC);
	GPIO_ResetBits(Motor_GPIO,DD);
    delay_ms(Speed); 
	GPIO_SetBits(Motor_GPIO,AA); 
	GPIO_ResetBits(Motor_GPIO,BB);
	GPIO_ResetBits(Motor_GPIO,CC);
	GPIO_ResetBits(Motor_GPIO,DD);
    delay_ms(Speed); 
}

void Motorcw_setp(u8 s)
{
	u8 i;
	for(i=0;i<s;i++)
		Motorcw();
}

void Motorccw(void)  
{  
    uint8_t i;  
    for(i=0;i<4;i++)  
    {  
        GPIO_Write(Motor_GPIO,phaseccw[i]);  
        delay_ms(Speed);  
    }  
} 

void MotorStop(void)  
{  
    GPIO_Write(Motor_GPIO,0x0000);  
}



