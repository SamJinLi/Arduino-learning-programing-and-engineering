String temp = "";
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(4,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0 )
  {
    while (Serial.available() > 0)
    {
      temp += char(Serial.read()); // a+=1   a=a+1
      delay(2);   //等待缓存数据
    }
    Serial.println(temp);
    if (temp == "on")
    {
      digitalWrite(4,1);
      Serial.print("开");
      }
    if (temp == "off")
    {
      digitalWrite(4,0);
      Serial.print("关");
      }
    temp = ""; // 清空数据
  }

  delay(100);
}
