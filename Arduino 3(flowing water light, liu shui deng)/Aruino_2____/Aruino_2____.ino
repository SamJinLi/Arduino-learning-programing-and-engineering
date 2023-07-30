void setup() {
  // put your setup code here, to run once:
  for (int i = 2; i <= 11; i++)
  {
    if (i == 7) {
      i = 9;
    }
    pinMode(i, OUTPUT);
  }
//  pinMode(2, OUTPUT); 继电器
//  pinMode(3, OUTPUT); 蜂鸣器
//  pinMode(4, OUTPUT); 单色LED
//  pinMode(5, OUTPUT);
//  pinMode(6, OUTPUT); 双色LED
//  pinMode(9, OUTPUT);
//  pinMode(10, OUTPUT);
//  pinMode(11, OUTPUT); LED RGB
//  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i = 2; i <= 11; i++)
  {
    if (i == 7) {
      i = 9;
    }
    //如果i等于7，那么就将i设置为9，然后执行以下程序。
    digitalWrite(i, 1);
    delay(1000);
  }
  for (int i = 11; i >= 2; i--)
  {
    if (i!=7 && i!= 8)//如果i不等于7和8，那么就执行一下程序。
    {
      digitalWrite(i, 0);
      delay(1000);
    }
  }
  //  digitalWrite(2, 1);
  //  delay(1000);
  //  digitalWrite(3, 1);
  //  delay(1000);
  //  digitalWrite(4, 1);
  //  delay(1000);
  //  digitalWrite(5, 1);
  //  delay(1000);
  //  digitalWrite(6, 1);
  //  delay(1000);
  //  digitalWrite(9, 1);
  //  delay(1000);
  //  digitalWrite(10, 1);
  //  delay(1000);
  //  digitalWrite(11, 1);
  //  delay(1000);
  //  digitalWrite(13, 1);
  //  delay(1000);
  //  digitalWrite(2, 0);
  //  delay(1000);
  //  digitalWrite(3, 0);
  //  delay(1000);
  //  digitalWrite(4, 0);
  //  delay(1000);
  //  digitalWrite(5, 0);
  //  delay(1000);
  //  digitalWrite(6, 0);
  //  delay(1000);
  //  digitalWrite(9, 0);
  //  delay(1000);
  //  digitalWrite(10, 0);
  //  delay(1000);
  //  digitalWrite(11, 0);
  //  delay(1000);
  //  digitalWrite(13, 0);
  //  delay(1000);
}
