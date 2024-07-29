#include <Arduino.h>
/*
// TODO: Test
*/
#include "BluetoothSerial.h"
#include <ESP32Encoder.h>
#include <Ticker.h>

#include "DFRobot_ADXL345.h"

DFRobot_ADXL345_I2C ADXL345(&Wire, 0x53);

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

bool accelOutput = false;

const int m1p1 = 15, m1p2 = 2, m1rx1 = 36, m1rx2 = 39, m2p1 = 14, m2p2 = 27, m2rx1 = 34, m2rx2 = 35;
int motorspeed = 0; 

// Accelerometer initialization
float Filter_Value, Filter_Value_Y, Filter_Value_Z;
int accval[3];
float FILTER_A = 0.4;
float FILTER_B;
float Value, Value_Y, Value_Z;

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
  pinMode(m1rx1, INPUT);
  pinMode(m1rx2, INPUT);
  Serial.begin(115200);

  ADXL345.begin();
  ADXL345.powerOn();

  analogWrite(m1p1,0);
  analogWrite(m1p2,255);
}

void loop() {
  analogWrite(m1p1, 255);
  analogWrite(m2p1, 255);
//   // put your main code here, to run repeatedly:
//   String data = "";
//   String date; 
//   // if (Serial.available()){
//   //   while (Serial.available() > 0) {
//   //     date += char (Serial.read());
//   //     delay(2);
//   //     }
//   //     FILTER_A = date.toInt();
//   //     FILTER_A = FILTER_A /100;
//   //   }
//    if (Serial.available()) {
//     SerialBT.write(Serial.read());
//   }
//   if (SerialBT.available()) {
//      while (SerialBT.available())
//      {
//        data += char(SerialBT.read()); // a+=1   a=a+1
//        delay(2);   //等待缓存数据
//      }
//      FILTER_A = data.toInt();
//     FILTER_A = FILTER_A /100;
//     Serial.println(data);
//   }
//   delay(20);
//   input = Filter_Value; 
//   setPoint = 0;
//   output = computePID(input);
//   if (output > 0){
//     analogWrite(m1p1, output); //2
//     analogWrite(m1p2, 0); //15
//   }
//   else if (output < 0){
//     analogWrite(m1p1, 0);
//     analogWrite(m1p2,abs(output));
//   }
//   else{
//     analogWrite(m1p1, 0);
//     analogWrite(m1p2, 0);
//   }

//   if (Filter_Value > 500 || Filter_Value < -500){
//     accelOutput = false;
//     }
//   else if (Filter_Value_Y > 500 || Filter_Value_Y < -500){
//     accelOutput = false;
//     }
//   else if (Filter_Value_Z > 500 || Filter_Value_Z < -500){
//     accelOutput = false;
//     }
//   else {
//     accelOutput = true;
//     }
//   delay(2);
//  /*if (Filter_Value > 175) {
//     delay(1500);
//     Filter_Value = Filter();
//     if (Filter_Value < 100) {
//      // Serial.println("上一页");
//     }
//   }
//   if (Filter_Value < -175) {
//     delay(1500);
//     Filter_Value = Filter();
//     if (Filter_Value > -100) {
//      // Serial.println("下一页");
//     }
//   }*/
//   //Serial.println(Filter_Value);
//   //ADXL345.RPCalculate(accval);
//   //Serial.print("Roll:"); Serial.println( ADXL345.RP.roll );
//   //Serial.print("Pitch:"); Serial.println( ADXL345.RP.pitch );
  
//   // analogWrite(m1p1, 255);
//   // analogWrite(m1p2, 0);
//   analogWrite(m2p1, 0);
//   analogWrite(m2p2, 0);

//   delay(5);
}

void outPutDatas()
{
  if(accelOutput)
  {
    Serial.print("x ");
    Serial.print(Filter_Value); // 串口输出
    Serial.print(",");
    Serial.print("y ");
    Serial.print(Filter_Value_Y);
    Serial.print(",");
    Serial.print("z ");
    Serial.print(Filter_Value_Z);
    Serial.print(",");
    
    //Serial.print(Get_AD());
   // Serial.print(",");
    //Serial.print(Get_AD_Y());
   // Serial.print(",");
    Serial.println(String(Get_AD_Z()) + " ");
  }

  Serial.println("Encoder 1 val: " + String(encoder1val) + " Encoder 2 val: " + String(encoder2val) + " Output: " + output);
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

float Get_AD() {
  ADXL345.readAccel(accval);
  if (accval[0] > 10000) {
    accval[0] -= 65536;
  }
  return accval[0];
}
// 一阶滞后滤波法
float Filter() {
  float NewValue;
  NewValue = Get_AD();
  Value = ((float)NewValue * FILTER_A + (1.0 - FILTER_A) * (float)Value);
  return Value;
} 


float Get_AD_Y() {
  ADXL345.readAccel(accval);
  if (accval[1] > 10000) {
    accval[1] -= 65536;
  }
  return accval[1];
}
// 一阶滞后滤波法
float Filter_Y() {
  float NewValue;
  NewValue = Get_AD_Y();
  Value_Y = ((float)NewValue * FILTER_A + (1.0 - FILTER_A) * (float)Value_Y);
  return Value_Y;
} 


float Get_AD_Z() {
  ADXL345.readAccel(accval);
  if (accval[2] > 10000) {
    accval[2] -= 65536;
  }
  return accval[2];
}
// 一阶滞后滤波法
float Filter_Z() {
  float NewValue;
  NewValue = Get_AD_Z();
  Value_Z = ((float)NewValue * FILTER_A + (1.0 - FILTER_A) * (float)Value_Z);
  return Value_Z;
} 
