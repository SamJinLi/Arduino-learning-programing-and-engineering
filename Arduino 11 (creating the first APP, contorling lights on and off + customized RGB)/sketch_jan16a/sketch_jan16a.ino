#include <SoftwareSerial.h>
#include <String.h>
String temp = "";
int R = 0, G = 0, B = 0, printtest = 0;
SoftwareSerial mySerial(10, 11); // RX, TX

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
  String date = "";

void loop() { // run over and over
  if (mySerial.available())
  {
    while (mySerial.available())
    {
      date += char(mySerial.read()); // a+=1   a=a+1
      delay(2);   //等待缓存数据
    }
    temp = date.substring(0, 1);
    Serial.println(date);
    if (temp == "R")
    {
      temp = date.substring(4,7);    
      R = temp.toInt();//字符类型转化为数字类型
      analogWrite(5,R);
      
    }
  }
  delay(20);
}
    /*if (dest == "G")
    {
      dest = temp.substring(4,7);
      G = dest.toInt();//字符类型转化为数字类型
      analogWrite(6,G);//PWM output
      Serial.println('G', G);
      dest = "";
      delay(2);
      }
    if (dest == "B")
    {
      dest = temp.substring(4,7);
      B = dest.toInt();//字符类型转化为数字类型
      analogWrite(9,B);//PWM output
      Serial.println('B', B);
      dest = "";
      delay(2);
      }
    if (dest == "on")
    {
      digitalWrite(4,1);
      Serial.println("LED, 开");
      dest = "";
      }
    if (dest == "off")
    {
      digitalWrite(4,0);
      Serial.println("LED, 关");
      dest = "";
      }
    temp = ""; 
  }
  delay(20);
  mySerial.print(printtest);
}*/
