# include <MsTimer2.h>; 
# define date_pin 4
# define SRCLK 2
# define RCLK 3
extern int number;
char Number [10] {
  0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90
  //数字一到九按照顺序的十六进制数字。
};

void led4_initial (){
   pinMode(date_pin, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
  MsTimer2::set (2,Nub_display);// 2ms period, 每五毫秒执行一次定时中断
  MsTimer2::start (); // 开始在后台运行，括号只是一个填空的，向 “Void” 一样
  }
void LED_OUT(unsigned char X) {
  //设置每一位
  for (int i = 0; i < 8 ; i++) {
    if (X & 0x80) digitalWrite(date_pin, 1);//0x80 = 0b10000000,测如果该值为1
    else digitalWrite(date_pin, 0);
    digitalWrite(SRCLK, 0);
    digitalWrite(SRCLK, 1); // 转一下齿轮到下一个卡槽
    X <<= 1;//X=X<<1, 左移数据（X）的值
  }
}

void LED_display (unsigned char pin, unsigned char count) {
  LED_OUT(count);
  LED_OUT(1 << (pin-1))  ;
  digitalWrite(RCLK, 0);
  digitalWrite(RCLK, 1);
}
void Nub_display () {
  char qian, bai, shi, ge;
  qian = number / 1000;
  bai = number% 1000 / 100;
  shi = number % 100 / 10;
  ge = number % 10;
  LED_display (4, Number [qian]);
  LED_display (3, Number [bai]);
  LED_display (2, Number [shi]);
  LED_display (1, Number [ge]);
}
