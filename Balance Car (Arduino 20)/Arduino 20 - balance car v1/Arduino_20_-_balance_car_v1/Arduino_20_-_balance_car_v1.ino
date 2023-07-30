/*
// TODO: Test
*/
#include <ESP32Encoder.h>
#include <Ticker.h>

const int m1p1 = 12, m1p2 = 13, m1rx1 = 36, m1rx2 = 39, m2p1 = 2, m2p2 = 15, m2rx1 = 34, m2rx2 = 35;
int motorspeed = 0; 

ESP32Encoder encoder;
ESP32Encoder encoder2;
float encoder1val, encoder2val;
Ticker encoderUpdater;

void setup() {
  // put your setup code here, to run once:

  //Encoder part starts here

  // Enable the weak pull down resistors

  //ESP32Encoder::useInternalWeakPullResistors=DOWN;
  // Enable the weak pull up resistors
  ESP32Encoder::useInternalWeakPullResistors = UP;

  // use pin 19 and 18 for the first encoder
  encoder.attachHalfQuad(36, 39);
  // use pin 17 and 16 for the second encoder
  encoder2.attachHalfQuad(34, 35);

  // set starting count value after attaching
  encoder.setCount(0);

  // clear the encoder's raw count and set the tracked count to zero
  encoder2.clearCount();
  Serial.println("Encoder Start = " + String((int32_t)encoder.getCount()));

  //Encoder part end here

  //Initialize encoder updater
  encoderUpdater.attach_ms(100, updateEncoder);

  pinMode(m1p1, OUTPUT);
  pinMode(m1p2, OUTPUT);
  pinMode(m1rx1, INPUT);
  pinMode(m1rx2, INPUT);
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  String data = "";
//  蓝牙通信在loop里！
  //   if (Serial.available())
  //  {
  //    while (Serial.available())
  //    {
  //      data += char(Serial.read()); // a+=1   a=a+1
  //      delay(2);   //等待缓存数据
  //    }
  //      motorspeed = data.toInt();
  //  }
  //  if (motorspeed.substring(0,1) != -) {//坦克左履带前进
  //          String temp1 = "";
  //          analogWrite(m1p1, motorspeed);
  //          analogWrite(m1p2, 0);
  //        }
  //        else if (motorspeed.substring(0,1) == -) {//坦克左履带后退
  //          analogWrite(m1p1, 0);
  //          analogWrite(m1p2, motorspeed);
  //        }
  Serial.println(String(encoder1val) + String(encoder2val));
  analogWrite(m1p1, 0);
  analogWrite(m1p2, 255);
  analogWrite(m2p1, 0);
  analogWrite(m2p2, 255);
  delay(5);
}

void updateEncoder () {
  encoder1val = encoder.getCount();
  encoder2val = encoder2.getCount();

  encoder.setCount(0);
  encoder2.setCount(0);
//  PID 控制要在中断里！
  
}
