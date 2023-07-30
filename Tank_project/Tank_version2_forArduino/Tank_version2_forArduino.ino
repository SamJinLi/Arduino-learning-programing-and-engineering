//蓝牙模块端口：RXD是11, TXD是10。
#include <SoftwareSerial.h>
#include <String.h>
String lefttrack = "", righttrack = "", test = "", test2 = "";
int R = 0, L = 0, R2 = 0, L2 = 0, forwardleft = 3, forwardright = 9, backwardleft = 5, backwardright = 6;
SoftwareSerial mySerial(11, 10); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  pinMode(forwardleft, OUTPUT);
  pinMode(forwardright, OUTPUT);
  pinMode(backwardleft, OUTPUT);
  pinMode(backwardright, OUTPUT);
  pinMode(13,OUTPUT); //13号引脚是Arduino Uno的内置LED灯，在这个程序里用来报错。
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}

void loop() { // run over and over
  String data = "";
  if (mySerial.available())
  {
    while (mySerial.available())
    {
      data += char(mySerial.read()); // a+=1   a=a+1
      delay(2);   //等待缓存数据
    }
    test = data.substring(0, 2);
    if (test == "AA") {
      test2 = data.substring(18, 20);
      if (test2 == "FF") {
        digitalWrite(13,1);
        lefttrack = data.substring(4,5);//检测左履带是前进还是后退
        righttrack = data.substring(12, 13);//检测右履带是前进还是后退

        if (lefttrack == "1") {//坦克左履带前进
          String temp1 = "";
          temp1 = data.substring(7, 10);
          L = temp1.toInt();//字符类型转化为数字类型
          Serial.print("L forward:");
          Serial.println(L);
          L2 = L;
          analogWrite(forwardleft, L2);
          analogWrite(backwardleft, 0);
        }
        else if (lefttrack == "0") {//坦克左履带后退
          String temp1 = "";
          temp1 = data.substring(7, 10);
          L = temp1.toInt();//字符类型转化为数字类型
          Serial.print("L backward:");
          Serial.println(L);
          L2 = L;
          analogWrite(forwardleft, 0);
          analogWrite(backwardleft, L2);
        }
        if (righttrack == "1") {//坦克右履带前进
          String temp1 = "";
          temp1 = data.substring(15, 18);
          R = temp1.toInt();//字符类型转化为数字类型
          Serial.print("R forward:");
          Serial.println(R);
          R2 = R;
          analogWrite(forwardright, R2);
          analogWrite(backwardright, 0);
        }
        else if (righttrack == "0") {//坦克右履带后退
          String temp1 = "";
          temp1 = data.substring(15, 18);
          R = temp1.toInt();//字符类型转化为数字类型
          Serial.print("R backward:");
          Serial.println(R);
          R2 = R;
          analogWrite(forwardright, 0);
          analogWrite(backwardright, R2);
        }
      }
      else {
        Serial.println("Data wasn't recived successfully");
        digitalWrite(13,0);
        }
    }
  }
}
