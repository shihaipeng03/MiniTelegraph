//淘宝『有名称的店铺』https://somebodys.taobao.com/
//更新日期 2018/09/07
//Mini Telegraph 电报机 写字程序
//本程序对应商品 https://item.taobao.com/item.htm?spm=a1z10.1-c.w4004-1266050943.20.5eaf3c6eT6FuhE&id=576633734176

//本程序实时更新的最新版本 https://create.arduino.cc/editor/wjd76/68303116-0495-4580-b125-6a339187df4f/preview
//强烈推荐使用web版编译器查看

#include <SoftwareSerial.h>  //蓝牙串口库 没有蓝牙模块也可以通过PC串口传输文字 

#include <Servo.h>        //舵机库
#include <Stepper_28BYJ_48.h> //更新的步进电机库文件
#include "chars.h"         //点阵字符库 
   
#define SERVO_PIN 2   //写字舵机接口号 
#define DOT_PIN   3   //打点舵机接口号


#define DOT_UP_DELAY 120    //打点等待时间  
#define DOT_DOWN_DELAY 50   //打点等待时间   //要符合舵机动作幅度的时间，延时过小会导致无法打到

#define DOT_UP 1100     //打点舵机提升到的位置，让笔刚好贴到纸上，不能抬的幅度过高
#define DOT_DOWN 1200   //下降后的位置
//    
#define SERVO_MIN 25   //写字舵机的字顶部位置
#define SERVO_MAX 65   //底部位置    

#define SERVO_STEPS 16  //字符的高度，数值越小，字符越大，建议范围（10~30）
#define LINE_TAB 9     //字符的基线位置，过大会超出纸的高度

#define SERVO_STEP (SERVO_MAX - SERVO_MIN) / SERVO_STEPS

#define SERVO_DELAY SERVO_STEP*5

#define STEPPER_STEP 600 / SERVO_STEPS


Servo servo_arm;          //写字舵机
Servo servo_dot;          //打点舵机
Stepper_28BYJ_48 stepper(4,5,6,7);  // 步进电机   in1~4端口 4 5 6 7

SoftwareSerial Bluetooth(0, 1);   // 初始化蓝牙 [RX, TX]    不同型号主板 端口号不同，uno 是 0,1  10,11 都可以

void setup() {
  Bluetooth.begin(9600);
  Serial.begin(9600); //调试代码
  servo_dot.attach(DOT_PIN);
  servo_dot.write(DOT_DOWN);
 
  servo_arm.attach(SERVO_PIN);
  servo_arm.write((SERVO_MIN + SERVO_MAX) / 2);
  
  pinMode(DOT_PIN, OUTPUT);
 
  printString("Test OK!");   //测试写一些内容
}



int pos = 0;

void dot(int m)  //打一个点
{
  if(m) {
    pos = 1;
    servo_dot.write(DOT_UP);
    delay(DOT_UP_DELAY);
  } else {
    pos = 0;
    servo_dot.write(DOT_DOWN);
    delay(DOT_DOWN_DELAY);
  }
}


void printLine(int b) //画一连续的线
{ 
  servo_arm.write(SERVO_MAX - (LINE_TAB-2)*SERVO_STEP);
  if(b != 0)
  {
    servo_arm.write(SERVO_MAX - (LINE_TAB-2)*SERVO_STEP);
    delay(SERVO_DELAY*20);
    for (int j = 0; b != 0; j++)
    {
      dot(b&1);
      b >>= 1;
      servo_arm.write(SERVO_MAX - (LINE_TAB+j)*SERVO_STEP);
      delay(SERVO_DELAY);
      }    
    dot(0);
  }

  stepper.step(7); //走纸幅度 可调节，数值越大，字符越扁
}



void printChar(char c)  //打印某一字符
{
int n = 0;
  Serial.println(c);
  for (int i = 0; i < 8; i++)
  {
    if(chars[c][i] != 0)
    {
      printLine(chars[c][i]);
      n++;
    }
    else stepper.step(7); //走纸幅度 可调节，数值越大，字符越扁
  }
}


void printString(char* str)   //打印一字符串  
{
  while(*str != '\0') {
    printChar(*str);
    str+=1;
  }
}

int n;
void loop() 
{
  Serial.println(Serial.available());   //等待串口状态，串口一次传递最多64个字符
  if(Bluetooth.available() > 0) {
    servo_dot.attach(DOT_PIN);
    servo_dot.write(DOT_DOWN);
    servo_arm.attach(SERVO_PIN);
    while(Bluetooth.available() > 2) {    //写串口接收到的字，最后2位的换行符不写。
      n =  Bluetooth.read();
      if(n >= 2) printChar((char)n);
    }
  }
  servo_dot.detach();
  servo_arm.detach(); //放松舵机，避免持续受力过热
  Bluetooth.write(1);
  delay(1500);
}
