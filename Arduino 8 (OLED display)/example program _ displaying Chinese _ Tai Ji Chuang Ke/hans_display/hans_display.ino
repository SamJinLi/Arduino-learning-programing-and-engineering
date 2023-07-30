// 引入IIC通讯所需的Wire库文件
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
    // 初始化OLED并设置其IIC地址为 0x3C
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
}
 
void loop()
{
    hans_display_0();
    hans_display_1();
}
 
void hans_display_0(void)
{
    // 显示之前清屏
    display.clearDisplay();
 
    // 显示文字 (左上角x坐标,左上角y坐标, 图形数组, 图形宽度像素点, 图形高度像素点, 设置颜色)
    display.drawBitmap(20 * 1, 16, hans_ni, 16, 16, 1);
    display.drawBitmap(20 * 2, 16, hans_hao, 16, 16, 1);
    display.drawBitmap(20 * 3, 16, hans_chuang, 16, 16, 1);
    display.drawBitmap(20 * 4, 16, hans_ke, 16, 16, 1);
 
    //显示图形
    display.display();
    delay(2000);
}
 
void hans_display_1(void)
{
    // 显示之前清屏
    display.clearDisplay();
 
    // 显示文字 (左上角x坐标,右上角y坐标, 图形数组, 图形宽度像素点, 图形高度像素点, 设置颜色)
    display.drawBitmap(20 * 1, 16, hans_tai1, 32, 32, 1);
    display.drawBitmap(20 * 2, 16, hans_ji1, 32, 32, 1);
    display.drawBitmap(20 * 3, 16, hans_chuang1, 32, 32, 1);
    display.drawBitmap(20 * 4, 16, hans_ke1, 32, 32, 1);
 
    //显示图形
    display.display();
    delay(2000);
}
 
