//Credit:https://randomnerdtutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/

// Basic demo for accelerometer readings from Adafruit MPU6050

// ESP32 Guide: https://RandomNerdTutorials.com/esp32-mpu-6050-accelerometer-gyroscope-arduino/
// ESP8266 Guide: https://RandomNerdTutorials.com/esp8266-nodemcu-mpu-6050-accelerometer-gyroscope-arduino/
// Arduino Guide: https://RandomNerdTutorials.com/arduino-mpu-6050-accelerometer-gyroscope/

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

Adafruit_MPU6050 mpu;

float currentAccelX = 0;
float currentAccelY = 0;
float currentAccelZ = 0;
float currentGyroX = 0;
float currentGyroY = 0;
float currentGyroZ = 0;
float currentTemp = 0;

float getAccelX();
float getAccelY();
float getAccelZ();
float getGyroX();
float getGyroY();
float getGyroZ();
float getTemperature();
void updateMPUData();  // New function to update all readings

void setup(void) {
  Serial.begin(115200);
  while (!Serial)
    delay(10); // will pause Zero, Leonardo, etc until serial console opens

  Serial.println("Adafruit MPU6050 test!");

  mpu.begin();
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  Serial.print("Accelerometer range set to: ");
  switch (mpu.getAccelerometerRange()) {
  case MPU6050_RANGE_2_G:
    Serial.println("+-2G");
    break;
  case MPU6050_RANGE_4_G:
    Serial.println("+-4G");
    break;
  case MPU6050_RANGE_8_G:
    Serial.println("+-8G");
    break;
  case MPU6050_RANGE_16_G:
    Serial.println("+-16G");
    break;
  }
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  Serial.print("Gyro range set to: ");
  switch (mpu.getGyroRange()) {
  case MPU6050_RANGE_250_DEG:
    Serial.println("+- 250 deg/s");
    break;
  case MPU6050_RANGE_500_DEG:
    Serial.println("+- 500 deg/s");
    break;
  case MPU6050_RANGE_1000_DEG:
    Serial.println("+- 1000 deg/s");
    break;
  case MPU6050_RANGE_2000_DEG:
    Serial.println("+- 2000 deg/s");
    break;
  }

  mpu.setFilterBandwidth(MPU6050_BAND_94_HZ);
  Serial.print("Filter bandwidth set to: ");
  switch (mpu.getFilterBandwidth()) {
  case MPU6050_BAND_260_HZ:
    Serial.println("260 Hz");
    break;
  case MPU6050_BAND_184_HZ:
    Serial.println("184 Hz");
    break;
  case MPU6050_BAND_94_HZ:
    Serial.println("94 Hz");
    break;
  case MPU6050_BAND_44_HZ:
    Serial.println("44 Hz");
    break;
  case MPU6050_BAND_21_HZ:
    Serial.println("21 Hz");
    break;
  case MPU6050_BAND_10_HZ:
    Serial.println("10 Hz");
    break;
  case MPU6050_BAND_5_HZ:
    Serial.println("5 Hz");
    break;
  }

  Serial.println("");
  delay(100);
}

void loop() {
  updateMPUData();  // Update all sensor readings
  
  static unsigned long lastPrint = 0;
  if (millis() - lastPrint >= 250) {
    lastPrint = millis();
    Serial.print("accel(m/s^2)-X:");
    Serial.print(currentAccelX);
    Serial.print(",");
    Serial.print("Y:");
    Serial.print(currentAccelY);
    Serial.print(",");
    Serial.print("Z:");
    Serial.print(currentAccelZ);
    Serial.print(",");
    
    Serial.print("Velx`(rad/s)-X:");
    Serial.print(currentGyroX);
    Serial.print(",");
    Serial.print("Y:");
    Serial.print(currentGyroY);
    Serial.print(",");
    Serial.print("Z:");
    Serial.print(currentGyroZ);
    Serial.print(",");
    
    Serial.print("Temperature(degC):");
    Serial.println(currentTemp);
  }
}

void updateMPUData() {
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);
  
  currentAccelX = a.acceleration.x;
  currentAccelY = a.acceleration.y;
  currentAccelZ = a.acceleration.z;
  currentGyroX = g.gyro.x;
  currentGyroY = g.gyro.y;
  currentGyroZ = g.gyro.z;
  currentTemp = temp.temperature;
}

float getAccelX() { return currentAccelX; }
float getAccelY() { return currentAccelY; }
float getAccelZ() { return currentAccelZ; }
float getGyroX() { return currentGyroX; }
float getGyroY() { return currentGyroY; }
float getGyroZ() { return currentGyroZ; }
float getTemperature() { return currentTemp; }

float getPitchRads() {
  // pitch = atan2(-accelX, sqrt(accelY^2 + accelZ^2))
  float pitch = atan2(-currentAccelX, sqrt(currentAccelY * currentAccelY + currentAccelZ * currentAccelZ));
  return pitch;  // Returns in radians
}

float getRollRads() {
  float roll = atan2(currentAccelY, currentAccelZ);
  return roll;  // Returns in radians
}
