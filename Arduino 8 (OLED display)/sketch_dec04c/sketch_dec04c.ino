#include <Wire.h>
#include "text.h"
// 引入驱动OLED0.96所需的库
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#define SCREEN_WIDTH 128 // 设置OLED宽度,单位:像素
#define SCREEN_HEIGHT 64 // 设置OLED高度,单位:像素
// 自定义重置引脚,虽然教程未使用,但却是Adafruit_SSD1306库文件所必需的
#define OLED_RESET 4
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#include "DHT.h" // include DHT.h library
#define DHTPIN 3 // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11 // Define the type of DHT Sensor as DHT 11
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  // 初始化Wire库
  //  Wire.begin();

  // 初始化OLED并设置其IIC地址为 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  dht.begin(); // 初始化DHT11
}
float tempature = 0;

float humidity = 0;

void loop() {

  tempature = dht.readTemperature();
  humidity = dht.readHumidity();

  display.clearDisplay();    // 显示之前清屏

  display.drawBitmap(20 * 1, 0, hans_wen, 20, 20, 1); // 显示文字 (左上角x坐标,右上角y坐标, 图形数组, 图形宽度像素点, 图形高度像素点, 设置颜色)

  display.drawBitmap(20 * 2, 0, hans_du, 20, 20, 1); // 显示文字 (左上角x坐标,右上角y坐标, 图形数组, 图形宽度像素点, 图形高度像素点, 设置颜色)

  display.setTextColor(WHITE);

  display.setTextSize(1.9);

  display.setCursor(61,5);

  display.print(":");

  display.print(tempature);

  display.print(" C ");

  display.setCursor(61, 41);

  display.drawBitmap(20 * 1, 31, hans_shi, 20, 20, 1);

  display.drawBitmap(20 * 2, 31, hans_du, 20, 20, 1);

  display.print(":");

  display.print(humidity);

  display.print("%");
  
  //显示图形

  display.display();   
  
  delay(1500);
}
