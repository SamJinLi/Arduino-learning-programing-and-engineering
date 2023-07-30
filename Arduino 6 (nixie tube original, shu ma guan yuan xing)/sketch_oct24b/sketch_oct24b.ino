#define date_pin  7
#define SRCLK     13
#define RCLK      8
void setup() {
  // put your setup code here, to run once:
  pinMode(date_pin, OUTPUT);
  pinMode(SRCLK, OUTPUT);
  pinMode(RCLK, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  LED_Display(4,0xf9);
  LED_Display(3,0b11000000);
  LED_Display(2,0b10110000);
  LED_Display(1,0b10011001);
}

void LED_OUT(unsigned char X) //设置每一位
{
  for (int i = 0; i < 8; i++)
  {
    if (X & 0x80)digitalWrite(date_pin, 1);
    else      digitalWrite(date_pin, 0);
    digitalWrite(SRCLK,0);
    digitalWrite(SRCLK,1);
    X<<=1;    //  X=X<<1
  }
}

void LED_Display(unsigned char addr,unsigned char count)
{
  LED_OUT(count);

  //片选
  LED_OUT(1<<(addr-1));//选第几个数码管工作
  digitalWrite(RCLK,0);
  digitalWrite(RCLK,1);
  }
