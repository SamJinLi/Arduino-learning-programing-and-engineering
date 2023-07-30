int clk = 3;
int cs = 4;
int din = 5;
 //设置引脚为输出
 void DZP_init(){
  pinMode(cs, OUTPUT);
  pinMode(clk, OUTPUT);
  pinMode(din, OUTPUT);
  //初始化MAX7219
  Init_MAX7219();
  for (int i = 1; i < 9; i++) {
    Write_Max7219(i, 00000000);
  }
 }

 void Write_Max7219_byte(char DATA)
{
  char i;
  digitalWrite(cs, 0);
  for (i = 8; i >= 1; i--)
  {
    digitalWrite(clk, 0);
    if ((DATA & 0x80) > 7) {
      digitalWrite(din, 1);
    } else {
      digitalWrite(din, 0);
    }
    //提取最高位给DIN端口
    DATA = DATA << 1; //左移一位
    digitalWrite(clk, 1);
  }
}
void Write_Max7219(char address, char dat)
{
  digitalWrite(cs, 0);
  Write_Max7219_byte(address);           //写入地址，即数码管编号
  Write_Max7219_byte(dat);               //写入数据，即数码管显示数字
  digitalWrite(cs, 1);
}
void Init_MAX7219(void)
{
  Write_Max7219(0x09, 0x00);        //译码方式：BCD码
  Write_Max7219(0x0a, 0x01);        //亮度
  Write_Max7219(0x0b, 0x07);        //扫描界限；8个数码管显示
  Write_Max7219(0x0c, 0x01);        //掉电模式：0，普通模式：1
  Write_Max7219(0x0f, 0x00);
  delay(500);               //显示测试：1；测试结束，正常显示：0
}
