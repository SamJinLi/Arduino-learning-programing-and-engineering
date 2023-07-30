#include<Ticker.h>

const int m1p1 = 12, m1p2 = 13, m2p1 = 15, m2p2 = 2;
const int m1rx1 = 36, m1rx2 = 39, m2rx1 = 34, m2rx2 = 35;

#define PI 3.1415926
int flag =0;
/***************** 定时中断参数 *****************/
Ticker timer1;  // 中断函数
int interrupt_time = 100; // 中断时间
int timer_flag = 0;             //定时器标志；

/***************** 编码器引脚及参数 *****************/
uint8_t R1 = 36, R2 = 39;  // 右轮编码器引脚
uint8_t L1 = 34, L2 = 35;  // 左轮编码器引脚   左右轮区分是以锂电池为底部区分
volatile long Rcounter1 = 0, Rcounter2 = 0; // 右轮脉冲计数  该变量用于存储编码器的值，所以用类型修饰符volatile；
volatile long Lcounter1 = 0, Lcounter2 = 0; // 左轮脉冲计数  该变量用于存储编码器的值，所以用类型修饰符volatile；


// void right_counter_encoder1(); // 右轮1 计数脉冲数
// void right_counter_encoder2(); // 右轮2 计数脉冲数
// void left_counter_encoder1();  // 左轮1 计数脉冲数
// void left_counter_encoder2();  // 左轮2 计数脉冲数

void setup()

{

  pinMode(m1p1, OUTPUT);
  pinMode(m1p2, OUTPUT);
  pinMode(m2p1, OUTPUT);
  pinMode(m2p2, OUTPUT);
  pinMode(m1rx1, INPUT);
  pinMode(m1rx2, INPUT);
  pinMode(m2rx1, INPUT);
  pinMode(m2rx2, INPUT);
  analogWrite(m1p1, 255);
  analogWrite(m1p2, 0);
  analogWrite(m2p1, 0);
  analogWrite(m2p2,255);
  Serial.begin(115200);

  /***************** 编码器初始化 *****************/
  pinMode(L1, INPUT);    pinMode(L2, INPUT);
  pinMode(R1, INPUT);    pinMode(R2, INPUT);
  attachInterrupt(R1, right_counter_encoder1, RISING);//设置编码器R1相位上升沿中断
  attachInterrupt(R2, right_counter_encoder2, RISING);//设置编码器R2相位上升沿中断
  attachInterrupt(L1, left_counter_encoder1, RISING);//设置编码器L1相位上升沿中断
  attachInterrupt(L2, left_counter_encoder2, RISING);//设置编码器L2相位上升沿中断

  /***************** 定时中断 *****************/
  timer1.attach_ms(interrupt_time, timerIsr);  // 打开定时器中断
  interrupts();                      //打开外部中断

}
void loop()
{
if (timer_flag == 1)
  { //判断是否发生定时器中断，这里是 interrupt_time ms发生一次
    timer_flag = 0;     //清除标记
  }
}

//定时器中断处理函数,其功能主要为了输出编码器得到的数据
void timerIsr() {
  timer_flag = 1; //定时时间达到标志
  readEncoder();   // 编码器
}

//编码器输出  mm
void readEncoder() {
  float L = Lcounter1 / 7 / 238 * 2 * PI; // 编码器旋转的角度（rad）编码器旋转一圈7个脉冲，减速比为238，
  float R = Rcounter1 / 7 /30 * 2 * PI; // 右轮
  Serial.print("左轮旋转的角度为： "); Serial.print(L); Serial.print("右轮旋转的角度为：  ");  Serial.println(R);
  
//数值清零，重新计数
  Rcounter1 = 0; Rcounter2 = 0;
  Lcounter1 = 0; Lcounter2 = 0;
}



// 编码器计数
void right_counter_encoder1() { //右轮计数
  Rcounter1++;
}
void right_counter_encoder2() {
  Rcounter2++;
}
void left_counter_encoder1() { // //左轮计数
  Lcounter1++;
}
void left_counter_encoder2() {
  Rcounter2++;
}
