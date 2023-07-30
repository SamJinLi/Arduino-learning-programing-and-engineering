//on Arduino
//"toFloat()" change string's to float's
#include <String.h>
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

void setup()
{
  // 初始化Wire库
  //  Wire.begin();

  // 初始化OLED并设置其IIC地址为 0x3C
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}
String tempature = "";

String humidity = "";

void loop() {

  tempature = "";//change ""
  humidity = "";//change ""
    // 显示之前清屏
  display.clearDisplay();
  
  display.setTextSize(1.5);
  
  display.drawBitmap(24 * 1, 0, hans_tian, 16, 16, 2);// 显示文字 (左上角x坐标*本行第n个字,左上角y坐标, 图形数组, 图形宽度像素点, 图形高度像素点, 设置颜色)

  display.drawBitmap(24 * 2, 0, hans_qi, 16, 16, 2);// 显示文字 (左上角x坐标*本行第n个字,左上角y坐标, 图形数组, 图形宽度像素点, 图形高度像素点, 设置颜色)
  
  display.drawBitmap(24 * 3, 0, hans_yu, 16, 16, 2);// 显示文字 (左上角x坐标*本行第n个字,左上角y坐标, 图形数组, 图形宽度像素点, 图形高度像素点, 设置颜色)

  display.drawBitmap(24 * 4, 0, hans_bao, 16, 16, 2);// 显示文字 (左上角x坐标*本行第n个字,左上角y坐标, 图形数组, 图形宽度像素点, 图形高度像素点, 设置颜色)

  display.setTextColor(WHITE);

  display.drawBitmap(16 * 0, 17, hans_jin, 16, 16, 2);// 显示文字 (左上角x坐标*本行第n个字,左上角y坐标, 图形数组, 图形宽度像素点, 图形高度像素点, 设置颜色)

  display.drawBitmap(16* 1, 17, hans_tian, 16, 16, 2);// 显示文字 (左上角x坐标*本行第n个字,左上角y坐标, 图形数组, 图形宽度像素点, 图形高度像素点, 设置颜色)
  
  display.display();
  
  delay(1500);
}
