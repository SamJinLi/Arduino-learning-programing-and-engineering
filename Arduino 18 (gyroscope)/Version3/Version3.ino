  /*!
   @file getSixaxis.ino
   @brief 用来收集传感器三轴加速度值，并计算Roll及Pitch角度信息
   @copyright   Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
   @license     The MIT License (MIT)
   @author      PengKaixing(kaixing.peng@dfrobot.com)
   @version  V2.0.0
   @date  2022-02-22
   @url https://github.com/DFRobot/DFRobot_ADXL345
*/
#include "DFRobot_ADXL345.h"

#define cs_pin 10

DFRobot_ADXL345_I2C ADXL345(&Wire, 0x53);
/**
   注意在使用leonardo作为主控的时候，需要连接ICSP引脚
*/
// DFRobot_ADXL345_SPI ADXL345(&SPI, cs_pin);

void setup()
{
  Serial.begin(9600);
  ADXL345.begin();
  ADXL345.powerOn();
}
float Filter_Value, Filter_Value_Y, Filter_Value_Z;
int accval[3];
float FILTER_A = 0.4;
float FILTER_B;
float Value, Value_Y, Value_Z;
void loop()
{
  String date; 
  if (Serial.available()){
    while (Serial.available() > 0) {
      date += char (Serial.read());
      delay(2);
      }
      FILTER_A = date.toInt();
      FILTER_A = FILTER_A /100;
    }
  //Serial.println("Triaxial acceleration value is :");
  //Serial.print("x:");
  Filter_Value = Filter();       // 获得滤波器输出值
  Filter_Value_Y = Filter_Y();
  Filter_Value_Z = Filter_Z();
  if (Filter_Value > 500 || Filter_Value < -500){
    
    }
  else if (Filter_Value_Y > 500 || Filter_Value_Y < -500){
    
    }
  else if (Filter_Value_Z > 500 || Filter_Value_Z < -500){
    
    }
  else {
    /*Serial.print(Filter_Value); // 串口输出
    Serial.print(",");
    Serial.print(Filter_Value_Y);
    Serial.print(",");
    Serial.print(Filter_Value_Z);
    Serial.print(",");
    */
    //Serial.print(Get_AD());
   // Serial.print(",");
    //Serial.print(Get_AD_Y());
   // Serial.print(",");
    Serial.println(Get_AD_Z());
    }
  delay(2);
 /*if (Filter_Value > 175) {
    delay(1500);
    Filter_Value = Filter();
    if (Filter_Value < 100) {
     // Serial.println("上一页");
    }
  }
  if (Filter_Value < -175) {
    delay(1500);
    Filter_Value = Filter();
    if (Filter_Value > -100) {
     // Serial.println("下一页");
    }
  }*/
  //Serial.println(Filter_Value);
  //ADXL345.RPCalculate(accval);
  //Serial.print("Roll:"); Serial.println( ADXL345.RP.roll );
  //Serial.print("Pitch:"); Serial.println( ADXL345.RP.pitch );
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
