//蓝牙模块端口：RXD是11, TXD是10。LED RGB端口：R是5,G是6,B是9。
#include <SoftwareSerial.h>
#include <String.h>
int R = 0, G = 0, B = 0, printtest = 0, R2 = 0, G2 = 0, B2 = 0;
SoftwareSerial mySerial(11, 10); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  pinMode(4, OUTPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}

String temp = "";

void loop() { // run over and over
  String date = "";
  if (mySerial.available())
  {
    while (mySerial.available())
    {
      date += char(mySerial.read()); // a+=1   a=a+1
      delay(2);   //等待缓存数据
    }
    temp = date.substring(0, 1);
    if (temp == "R")
    {
      temp = date.substring(4, 7);
      R = temp.toInt();//字符类型转化为数字类型
      Serial.print("R:");
      Serial.println(R);
      String temp2 = "";
      temp2 = date.substring(11, 14);
      G = temp2.toInt();//字符类型转化为数字类型
      Serial.print("G:");
      Serial.println(G);
      String temp3 = "";
      temp3 = date.substring(18, 23);
      B = temp3.toInt();//字符类型转化为数字类型
      Serial.print("B:");
      Serial.println(B);
      R2 = R;
      G2 = G;
      B2 = B;
      analogWrite(5,R2);
      analogWrite(6,G2);
      analogWrite(9,B2);
    }
  }
}
