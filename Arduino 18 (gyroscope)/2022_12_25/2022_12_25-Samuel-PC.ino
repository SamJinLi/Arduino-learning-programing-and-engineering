/*

*/
//Source Link: https://www.geek-workshop.com/thread-681-1-1.html

//ADXL345
#include "DFRobot_ADXL345.h"
#include <Wire.h>

#define cs_pin 10

#define Acc 0x1D
#define Gyr 0x69
#define Mag 0x1E
#define Gry_offset -13    // 陀螺仪偏移量
#define Gyr_Gain 0.07     // 满量程2000dps时灵敏度(dps/digital)
#define pi 3.14159
long timer = 0;           // 采样时间

//陀螺仪值
DFRobot_ADXL345_I2C ADXL345(&Wire, 0x53);
int accval[3];

void setup() {
  sensor_init();                                // 配置传感器
  Serial.begin(9600);                           // 开启串口以便监视数据
  ADXL345.begin();
  ADXL345.powerOn();
  delay(1000);
}
void loop() {
  long o_timer = timer;                         // 上一次采样时间(ms)
  timer = millis();                            // 当前时间(ms)
  float ACdt = (timer - o_timer) / 1000.0;     // 微分时间(s)
  float angleAX = gDat(0); // x value
  float angleAY = gDat(1); // y value
  float angleAZ = gDat(2); //z value
  Serial.print(ErJieSuanFa(angleAX, ACdt, 0));
  Serial.print(",");
  Serial.print(ErJieSuanFa(angleAY, ACdt, 1));
  Serial.print(",");
  Serial.println(ErJieSuanFa(angleAZ, ACdt, 2));
  delay(5);
}
// 二阶互补算法
float ErJieSuanFa (float AngleA, float dt, int num) {
  static float X1[3], X2[3], UsedToBex1[3], Com2_angle[3];
  static int K = 3;
  float omega =  Gyr_Gain * (AngleA +  Gry_offset);
  X1[num] = (AngleA - Com2_angle[num]) * K * K;
  UsedToBex1[num] = UsedToBex1[num] + X1[num] * dt;
  X2[num] = UsedToBex1[num] + 2 * K * (AngleA - Com2_angle[num]) + omega;
  Com2_angle[num] = Com2_angle[num] + X2[num] * dt;
  return Com2_angle[num];
}

/*函数作用：陀螺仪数据获取函数
 * 入口参数：device 获取xyz轴，0是x，1是y，2是z
 * 返回值：无
 */
int gDat(int device)
{
  ADXL345.readAccel(accval);
  if (accval[device] > 10000)
  {
    accval[device] -= 65536;
  }
  return accval[device];
}
void sensor_init() {                         // 配置九轴姿态传感器
  writeRegister(Acc, 0x2D, 0b00001000);    // 测量模式
  // 配置ADXL345
  writeRegister(Gyr, 0x20, 0b00001111);    // 设置睡眠模式、x, y, z轴使能
  writeRegister(Gyr, 0x21, 0b00000000);    // 选择高通滤波模式和高通截止频率
  writeRegister(Gyr, 0x22, 0b00000000);    // 设置中断模式
  writeRegister(Gyr, 0x23, 0b00110000);    // 设置量程(2000dps)、自检状态、SPI模式
  writeRegister(Gyr, 0x24, 0b00000000);    // FIFO & 高通滤波
  // 配置L3G4200D(2000 deg/sec)
  writeRegister(Mag, 0x02, 0x00);          // 连续测量
  // 配置HMC5883L
}

/*函数作用：陀螺仪传感器初始化
 * 入口参数：无
 * 返回值：无
 */
void writeRegister(int device, byte address, byte val) {    // 写寄存器
  Wire.beginTransmission(device);
  Wire.write(address);
  Wire.write(val);
  Wire.endTransmission();
}
