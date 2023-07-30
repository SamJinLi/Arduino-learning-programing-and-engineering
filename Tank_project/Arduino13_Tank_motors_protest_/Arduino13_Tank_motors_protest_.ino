//蓝牙模块端口：RXD是11, TXD是10。LED RGB端口：R是5,G是6,B是9。
# include <MsTimer2.h>
#include <SoftwareSerial.h>
#include <String.h>
int q = 0, h = 0, R = 0, G = 0, B = 0, printtest = 0, R2 = 0, G2 = 0, B2 = 0, forwardleft = 3, forwardright = 9, backwardleft = 5, backwardright = 6;
SoftwareSerial mySerial(11, 10); // RX, TX

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  tank_direction_initial ();
  pinMode(forwardleft, OUTPUT);
  pinMode(forwardright, OUTPUT);
  pinMode(backwardleft, OUTPUT);
  pinMode(backwardright, OUTPUT);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }


  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(9600);
  mySerial.println("Hello, world?");
}

void tank_direction_initial () {
  MsTimer2::set (90, backward_or_forward);
  MsTimer2::start();//设置定时中断以确认是前进还是后退
  }
  String date = "";
void backward_or_forward(){
 if (date == "后退"){
  h = 1;
  q = 0;
  }
 else if (date == "前进"){
  h = 0;
  q = 1;
  } 
}
Serial.println(date);
}
String temp = "";

void loop() { // run over and over
  
  if (mySerial.available())
  {
    while (mySerial.available())
    {
      date += char(mySerial.read()); // a+=1   a=a+1
      delay(2);   //等待缓存数据
    }
    
    if (q == 1){
Serial.println("forward");
/*
      temp = date.substring(0, 1);
      if (temp == "R")
      {
        String temp1 = "";
        temp1 = date.substring(4, 7);
        R = temp1.toInt();//字符类型转化为数字类型
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
        analogWrite(forwardleft, R2);
        analogWrite(backwardleft, 0);
        analogWrite(forwardright, G2);
        analogWrite(backwardright, 0);
      }*/
    }

    


    else if (h == 1) {
      Serial.println("backwards");
     /* if (temp == "R")
      {
        String temp1 = "";
        temp1 = date.substring(5, 8);
        R = temp1.toInt();//字符类型转化为数字类型
        Serial.print("R:");
        Serial.println(R);
        String temp2 = "";
        temp2 = date.substring(12, 15);
        G = temp2.toInt();//字符类型转化为数字类型
        Serial.print("G:");
        Serial.println(G);
        String temp3 = "";
        temp3 = date.substring(19, 24);
        B = temp3.toInt();//字符类型转化为数字类型
        Serial.print("B:");
        Serial.println(B);
        R2 = R;
        G2 = G;
        B2 = B;
        analogWrite(forwardleft, 0);
        analogWrite(backwardleft, R2);
        analogWrite(forwardright, 0);
        analogWrite(backwardright, G2);
      }*
    }*/
  }
}

}
