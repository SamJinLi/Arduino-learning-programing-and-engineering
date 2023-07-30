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

signed int accval[3];

int date_Old1 = 0, date_Old2 = 0;

void setup()
{
  Serial.begin(9600);
  ADXL345.begin();
  ADXL345.powerOn();
}

void loop()
{
  ADXL345.readAccel(accval);
  //Serial.println("Triaxial acceleration value is :");
  //Serial.print("x:");

  if (accval[0] > 10000) {
    accval[0] -= 65536;
  }
  int finished = (accval[0] + date_Old1 + date_Old2) / 3;
  date_Old2 = date_Old1;
  date_Old2 = accval[0];

  //  *Serial.println(finished);
  if (finished > 175) {
    delay(1500);
    ADXL345.readAccel(accval);
    if (accval[0] > 10000){
      accval[0] -= 65536;
      }
    finished = (accval[0] + date_Old1 + date_Old2) / 3;
    date_Old2 = date_Old1;
    date_Old2 = accval[0];
    if (finished < 100) {
      Serial.println("上一页");
    }
  }
  if (finished < -175) {
    delay(1500);
    ADXL345.readAccel(accval);
    if (accval[0] > 10000){
      accval[0] -= 65536;
    }
    finished = (accval[0]+date_Old1+date_Old2)/3;
    date_Old2=date_Old1;
    date_Old2=accval[0];
    if (finished > -100) {
      Serial.println("下一页");
    }
  }
  //Serial.print(" y:");Serial.print(accval[1]);
  //Serial.print(" z:");Serial.println(accval[2]);
  //ADXL345.RPCalculate(accval);
  //Serial.print("Roll:"); Serial.println( ADXL345.RP.roll );
  //Serial.print("Pitch:"); Serial.println( ADXL345.RP.pitch );
  delay(20);
}
