#include <Arduino.h>
/*
// TODO: Test
*/
#include "BluetoothSerial.h"
#include <ESP32Encoder.h>
#include <Ticker.h>

//PID constants
double kp = 0.1;
double ki = 0;
double kd = 0;
 
unsigned long currentTime, previousTime;
double elapsedTime;
double error;
double lastError;
double input, output, setPoint;
double cumError, rateError;

const int m1p1 = 12, m1p2 = 13, m1rx1 = 36, m1rx2 = 39, m2p1 = 2, m2p2 = 15, m2rx1 = 34, m2rx2 = 35;
int motorspeed = 0; 

ESP32Encoder encoder;
ESP32Encoder encoder2;
float encoder1val, encoder2val;
Ticker encoderUpdater;

String device_name = "ESP32BT";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial SerialBT;

void setup() {
  // put your setup code here, to run once:

  //Encoder part starts here

  // Enable the weak pull down resistors

  //ESP32Encoder::useInternalWeakPullResistors=DOWN;
  // Enable the weak pull up resistors
  ESP32Encoder::useInternalWeakPullResistors = puType::up;

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

  //Bluetooth setup begins here
  SerialBT.begin(device_name); //Bluetooth device name
  Serial.printf("The device with name \"%s\" is started.\nNow you can pair it with Bluetooth!\n", device_name.c_str());
  //   Bluetooth setup ends here
  pinMode(m1p1, OUTPUT);
  pinMode(m1p2, OUTPUT);
  pinMode(m2p1, OUTPUT);
  pinMode(m2p2, OUTPUT);
  pinMode(m1rx1, INPUT);
  pinMode(m1rx2, INPUT);
  Serial.begin(115200);

  // analogWrite(m1p1,255);
  // analogWrite(m1p2,0);


}

void loop() {
  // put your main code here, to run repeatedly:
  // String data = "";
  //  if (Serial.available()) {
  //   SerialBT.write(Serial.read());
  // }
  // if (SerialBT.available()) {
  //    while (SerialBT.available())
  //    {
  //      data += char(SerialBT.read()); // a+=1   a=a+1
  //      delay(2);   //等待缓存数据
  //    }
  //   Serial.println(data);
  // }
  // delay(20);
  // input = encoder1val; 
  // setPoint = -1555;
  // output = computePID(input);
  // delay(100);
  //     analogWrite(m2p1, 255);
  // analogWrite(m2p2, 0);
  // if (output > 0){
  //   analogWrite(m1p1, output); //2
  //   analogWrite(m1p2, 0); //15
  // }
  // else if (output < 0){
  //   analogWrite(m1p1, 0);
  //   analogWrite(m1p2,abs(output));
  // }
  // else{
  //   analogWrite(m1p1, 0);
  //   analogWrite(m1p2, 0);
  // }
  analogWrite(m1p1, 255);
  analogWrite(m2p1, 255);
  
  // analogWrite(m1p1, 255);
  // analogWrite(m1p2, 0);
  // analogWrite(m2p1, 0);
  // analogWrite(m2p2, 0);
  Serial.println("Encoder 1 val: " + String(encoder1val) + " Encoder 2 val: " + String(encoder2val) + " Output: " + output);
  delay(5);
}

void updateEncoder () {
  encoder1val = encoder.getCount();
  encoder2val = encoder2.getCount();

  encoder.setCount(0);
  encoder2.setCount(0);
//  PID 控制要在中断里！
  
}

double computePID(double inp){     
        currentTime = millis();                //get current time
        elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation
        
        error = setPoint - inp;                                // determine error
        cumError += error * elapsedTime;                // compute integral     
        rateError = (error - lastError)/elapsedTime;   // compute derivative
 
        double out = kp*error + ki*cumError + kd*rateError;                //PID output               
 
        lastError = error;                                //remember current error
        previousTime = currentTime;                        //remember current time
 
        return out;                                        //have function return the PID output
}
