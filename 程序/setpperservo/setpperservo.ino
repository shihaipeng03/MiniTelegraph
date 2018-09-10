//淘宝『有名称的店铺』https://somebodys.taobao.com/
//更新日期 2018/08/29
//Mini Telegraph 电报机 调试舵机位置程序


#include <Servo.h>        //舵机库
#include <Stepper_28BYJ_48.h> //更新的步进电机库文件

    
#define SERVO_PIN 2   //写字舵机接口号 
#define DOT_PIN   3   //打点舵机接口号


#define DOT_UP_DELAY 120    //打点等待时间  
#define DOT_DOWN_DELAY 50   //打点等待时间   //要符合舵机动作幅度的时间，延时过小会导致无法打到

#define DOT_UP 1700     //打点舵机提升到的位置，让笔刚好贴到纸上，不能抬的幅度过高
#define DOT_DOWN 1900   //下降后的位置
//    
#define SERVO_MIN 110   //写字舵机的字顶部位置
#define SERVO_MAX 140   //底部位置    

#define SERVO_STEPS 15  //字符的高度，数值越小，字符越大，建议范围（10~30）
#define LINE_TAB 9     //字符的基线位置，过大会超出纸的高度

#define SERVO_STEP (SERVO_MAX - SERVO_MIN) / SERVO_STEPS

#define SERVO_DELAY SERVO_STEP*5

#define STEPPER_STEP 600 / SERVO_STEPS


Servo servo;              //写字舵机
Servo servo_dot;          //打点舵机
Stepper_28BYJ_48 stepper(4,5,6,7);  // 步进电机   in1~4端口 4 5 6 7

void setup() {

  servo_dot.attach(DOT_PIN);
  servo_dot.write(DOT_DOWN);
 
  servo.attach(SERVO_PIN);
  
  pinMode(DOT_PIN, OUTPUT);


}







void loop() 
{
//调试打点舵机的位置
servo.write((SERVO_MIN + SERVO_MAX) / 2);
 for (int i=0;i<5;i++)
 {
  servo_dot.write(DOT_UP);
  delay(DOT_UP_DELAY);  
  servo_dot.write(DOT_DOWN);
  delay(DOT_DOWN_DELAY);
  delay(300);
 }
  delay(1500);

  //调试写字舵机的位置
  for (int i=0;i<5;i++)
 {
  servo.write(SERVO_MIN);
  delay(800);  
  servo.write(SERVO_MAX);
  delay(800);
 }

  stepper.step(500); //连续很长的纸
  delay(100);
}


