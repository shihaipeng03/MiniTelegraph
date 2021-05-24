/**********************************************
**author£º obito
**github£º https://github.com/obito0
***********************************************/
#include "stm32f10x.h"
#include "led.h"
#include "delay.h"
#include "usart.h"
#include "motor.h"
#include "pwm_output.h"

 int main(void)
 {	 
	 u8 i;
	 LED_Init();
	 delay_init();
	 uart_init(9600);
	 Motor_Init();
	 TIM4_PWM_Init();
	 Motorcw_setp(20);
	 for(i=0;i<19;i++)
	 {
		printChinese(i);
		Motorcw_setp(10);
	 }
	 printString("..........................................................");
//	 printString("I wish you all a happy New Year and all the best!");
//	 Motorcw_setp(200);
//	 Motorcw_setp(200);
//	 Motorcw_setp(200);
//	 Motorcw_setp(200);
//	 Motorcw_setp(200);
//	 Motorcw_setp(200);
	 while(1)
	 {
		  //Motorcw_setp(100);
	 }
 }
 

