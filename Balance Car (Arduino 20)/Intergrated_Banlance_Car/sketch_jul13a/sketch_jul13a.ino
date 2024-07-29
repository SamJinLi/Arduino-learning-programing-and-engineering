//FRONT OF THE BANLANCE CAR IS OPPOSITE OF ESP32
#include <Arduino.h>

#include "BluetoothSerial.h"
#include <ESP32Encoder.h>
#include <Ticker.h>
#include <algorithm>

#include "DFRobot_ADXL345.h"

#include <Thread.h>
#include <ThreadController.h>

//Threads Stuff
ThreadController controll = ThreadController();

//thread for updating PID,
//encoder values & gyro values
Thread updater = Thread();
//thread for turning the motors
Thread motorOutput = Thread();

//ADXL345 Accelerometer Variables
DFRobot_ADXL345_I2C ADXL345(&Wire, 0x53);

// float Filter_Value, Filter_Value_Y, Filter_Value_Z;
int acelValues[3], oldAcelValues[3], filteredValues[3];
float FILTER_A = 0.4;
float FILTER_B;
// float Value, Value_Y, Value_Z;
// PID Constants
double kp = 0.1;
double ki = 0;
double kd = 0;

unsigned long currentTime, previousTime;
double elapsedTime;
double error;
double lastError;
double input, output, setPoint = 0.0;
double cumError, rateError;

int leftMotorOutput, rightMotorOutput;

//Motor constants
const int leftP1 = 12, leftP2 = 13, leftRx1 = 36, leftRx2 = 39; //motor 1 constants, p is power, rx is encoders
const int rightP1 = 2, rightP2 = 15, rightRx1 = 34, rightRx2 = 35; //motor 2 constants

ESP32Encoder leftEncoder;
ESP32Encoder rightEncoder;

float leftEncoderVal, rightEncoderVal;
Ticker threadInterrupts;

//Bluetooth stuff
String device_name = "BanlanceCar32";

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif

BluetoothSerial serialBT;
void timerCallback()
{
  controll.run();
}

void waitSerial()
{
  String stuff = "";
  while (!Serial.available());
	delay(10);
  if(Serial.available())
  {
    while (Serial.available())
    {
      stuff += char(Serial.read());
      delay(2);
    }
    String firstVal = stuff.substring(0,1);
    if(firstVal.equals("p"))
    {
      kp = stuff.substring(1).toDouble();
    }
    if(firstVal.equals("i"))
    {
      ki = stuff.substring(1).toDouble();
    }
    if(firstVal.equals("d"))
    {
      kd = stuff.substring(1).toDouble();
    }	
    Serial.println(stuff);  
  }
}

void waitBluetoothSerial()
{
  String bluetoothData = "";
  if(serialBT.available())
  {
    while(serialBT.available())
    {
      bluetoothData += char(serialBT.read());
      delay(2);
    }

    String firstVal = bluetoothData.substring(0,1);
    if(firstVal.equals("p"))
    {
      kp = bluetoothData.substring(1).toDouble();
    }
    if(firstVal.equals("i"))
    {
      ki = bluetoothData.substring(1).toDouble();
    }
    if(firstVal.equals("d"))
    {
      kd = bluetoothData.substring(1).toDouble();
    }	
    Serial.println(bluetoothData);
  }
}

//Motor functions
void rotateMotors()
{
  rotateLeftMotor(leftMotorOutput);
  rotateRightMotor(rightMotorOutput);
}

void rotateRightMotor(int right)
{
  if(right < 0)
  {
    analogWrite(rightP1, 0);
    analogWrite(rightP2, right);
    // return;
  }
  else if (right > 0)
  {
    analogWrite(rightP1, right);
    analogWrite(rightP2, 0);
  }
  else
  {
    analogWrite(rightP1, 0);
    analogWrite(rightP2, 0);
  }
}

void rotateLeftMotor(int left)
{
  if(left < 0)
  {
    analogWrite(leftP1, 0);
    analogWrite(leftP2, left);
    // return;
  }
  else if (left > 0)
  {
    analogWrite(leftP1, left);
    analogWrite(leftP2, 0);
  }
  else
  {
    analogWrite(leftP1, 0);
    analogWrite(leftP2, 0);
  }
}
//Accelerometer functions

void updateAllAccelAxis()
{
  ADXL345.readAccel(acelValues);
  std::for_each(acelValues, acelValues+3, accelBounder);
  // if (acelValues[0] > 10000) {
  //   acelValues[0] -= 65536;
  // }
  // if (acelValues[1] > 10000) {
  //   acelValues[1] -= 65536;
  // }
  // if (acelValues[1] > 10000) {
  //   acelValues[1] -= 65536;
  // }
}
void accelBounder(float accelValue)
{
  if(accelValue > 10000)
  {
    accelValue -= 65536;
  }
}

void filterAllAccelAxis()
{
  for(int i = 0; i < ((sizeof(acelValues))/ (sizeof(acelValues[0]))); i ++)
  {
    filteredValues[i] = ((float)acelValues[i] * FILTER_A + (1.0 - FILTER_A) * (float)filteredValues[i]);
  }
}

double calculatePID(int currentInput)
{
        currentTime = millis();                //get current time
        elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation
        
        error = setPoint - currentInput;                                // determine error
        cumError += error * elapsedTime;                // compute integral     
        rateError = (error - lastError)/elapsedTime;   // compute derivative
 
        double out = kp*error + ki*cumError + kd*rateError;                //PID output               
 
        lastError = error;                                //remember current error
        previousTime = currentTime;                        //remember current time

        return out;                                        //have function return the PID output
}

// float getRawAccelX() {
//   ADXL345.readAccel(acelValues);
//   if (acelValues[0] > 10000) {
//     acelValues[0] -= 65536;
//   }
//   return acelValues[0];
// }
// // 一阶滞后滤波法
// float Filter_X() {
//   float NewValue;
//   NewValue = getRawAccelX();
//   filteredValues[0] = ((float)NewValue * FILTER_A + (1.0 - FILTER_A) * (float)acelValues[0]);
//   return filteredValues[0];
// } 


// float getRawAccelY() {
//   ADXL345.readAccel(acelValues);
//   if (acelValues[1] > 10000) {
//     acelValues[1] -= 65536;
//   }
  
//   return acelValues[1];
// }
// // 一阶滞后滤波法
// float Filter_Y() {
//   float NewValue;
//   NewValue = getRawAccelY();
//   filteredValues[1] = ((float)NewValue * FILTER_A + (1.0 - FILTER_A) * (float)acelValues[1]);
//   return filteredValues[1];
// } 


// float Get_AD_Z() {
//   ADXL345.readAccel(acelValues);
//   if (acelValues[2] > 10000) {
//     acelValues[2] -= 65536;
//   }
//   Value_Z = acelValues[2];
//   return Value_Z;
// }
// // 一阶滞后滤波法
// float Filter_Z() {
//   float NewValue;
//   NewValue = Get_AD_Z();
//   Filter_Value_Z = ((float)NewValue * FILTER_A + (1.0 - FILTER_A) * (float)Value_Z);
//   return Filter_Value_Z;
// }

void update() {
  //encoders
  leftEncoderVal = leftEncoder.getCount();
  rightEncoderVal = rightEncoder.getCount();
  // encoder1val = encoder.getCount();
  // encoder2val = encoder2.getCount();
  // encoder.setCount(0);
  // encoder2.setCount(0);

  //roll and pitch
  updateAllAccelAxis();//MUST HAVE THIS LINE BEFORE FILTER ALL ACCELAXIS for the filter to work
  filterAllAccelAxis();//MUST HAVE THIS LINE BEFORE RP CALCULATE in order to ensure the values are updated
  ADXL345.RPCalculate(filteredValues);
  // Serial.print();
}



void setup() {
  //Serial
  Serial.begin(115200);
  //BT Serial
  serialBT.begin(device_name);
  Serial.printf("The device with name \"%s\" is started. \nNow you can pair it with Bluetooth!\n", device_name.c_str());

  //Multi thread stuff
  updater.onRun(update);
  updater.setInterval(10);
  
  motorOutput.onRun(rotateMotors);
  motorOutput.setInterval(10);

  controll.add(&updater);
  controll.add(&motorOutput);
  
  updater.enabled = true;
  motorOutput.enabled = true;

  threadInterrupts.attach_ms(100, timerCallback);
  //Encoder
  ESP32Encoder::useInternalWeakPullResistors = puType::up;

  rightEncoder.attachHalfQuad(rightRx1, rightRx2);
  leftEncoder.attachHalfQuad(leftRx1, leftRx2);

  //set starting count values after attaching
  rightEncoder.setCount(0);
  leftEncoder.setCount(0);

  //rightEncoder.clearCount();
  //leftEncoder.clearCount();

  Serial.println("Right Encoder Start = " + String((int32_t)rightEncoder.getCount()) + " Left Encoder Start = " + String((int32_t)leftEncoder.getCount()));


  //ADXL345 Initializes
  ADXL345.begin();
  ADXL345.powerOn();

  // Setting pinmodes
  pinMode(leftP1, OUTPUT);
  pinMode(leftP2, OUTPUT);
  pinMode(rightP1, OUTPUT);
  pinMode(rightP2, OUTPUT);

  pinMode(leftRx1, INPUT);
  pinMode(leftRx2, INPUT);
  pinMode(rightRx1, INPUT);
  pinMode(rightRx2, INPUT);
  
  analogWrite(rightP1, 24.5);
}

void loop() {
  // put your main code here, to run repeatedly:
  //Instead of bluetooth, I used console instead cuz I think it's faster
  waitSerial();
  // analogWrite(leftP1, 0);
  // analogWrite(leftP2, 255);
  // analogWrite(rightP1, 0);
  // analogWrite(rightP2, 255);
}
