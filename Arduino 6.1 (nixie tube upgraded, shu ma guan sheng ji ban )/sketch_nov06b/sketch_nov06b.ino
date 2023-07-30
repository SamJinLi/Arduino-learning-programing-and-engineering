#define date_pin  7
#define SRCLK     13
#define RCLK     8
char Number [10] {
  0xC0, 0xF9, 0xA4, 0xB0, 0x99, 0x92, 0x82, 0xF8, 0x80, 0x90
  //数字一到九按照顺序的十六进制数字。
};
void setup() {
  // put your setup code here, to run once:
  pinMode(date_pin, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  Nub_display (5678);
}
void Nub_display (int date) {
  //用于显示 date 是一个四位的数据
  char qian, bai, shi, ge; //1234
  qian = date / 1000;
  bai = date  % 1000 / 100;
  shi = date  % 100 / 10;
  ge = date  % 10;
  LED_Display(4, Number[qian]);
  LED_Display(3, Number[bai]);
  LED_Display(2, Number[shi]);
  LED_Display(1, Number[ge]);
}
void LED_OUT(unsigned char X) //设置每一位
{
  for (int i = 0; i < 8; i++)
  {
    if (X & 0x80)digitalWrite(date_pin, 1);
    else      digitalWrite(date_pin, 0);
    digitalWrite(SRCLK, 0);
    digitalWrite(SRCLK, 1);
    X <<= 1;  //  X=X<<1
  }
}

void LED_Display(unsigned char addr, unsigned char count)
{
  LED_OUT(count);

  //片选
  LED_OUT(1 << (addr - 1)); //选第几个数码管工作
  digitalWrite(RCLK, 0);
  digitalWrite(RCLK, 1);
}
