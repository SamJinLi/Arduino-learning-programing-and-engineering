/**********************************************************************
  项目名称/Project          : 零基础入门学用物联网
  程序名称/Program name     : weather_now
  团队/Team                : 太极创客团队 / Taichi-Maker (www.taichi-maker.com)
  作者/Author              : CYNO朔
  日期/Date（YYYYMMDD）     : 20200602
  程序目的/Purpose          :
  通过心知天气(www.seniverse.com)免费服务获取实时天气信息。
  -----------------------------------------------------------------------
  其它说明 / Other Description
  心知天气API文档说明: https://www.seniverse.com/docs

  本程序为太极创客团队制作的免费视频教程《零基础入门学用物联网 》中一部分。该教程系统的
  向您讲述ESP8266的物联网应用相关的软件和硬件知识。以下是该教程目录页：
  http://www.taichi-maker.com/homepage/esp8266-nodemcu-iot/
***********************************************************************/
#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

const char* ssid     = "Frontier4960";       // 连接WiFi名（此处使用taichi-maker为示例）
// 请将您需要连接的WiFi名填入引号中
const char* password = "1782597198";          // 连接WiFi密码（此处使用12345678为示例）
// 请将您需要连接的WiFi密码填入引号中

const char* host = "api.seniverse.com";     // 将要连接的服务器地址
const int httpPort = 80;                    // 将要连接的服务器端口

// 心知天气HTTP请求所需信息
String reqUserKey = "4bbiq9s5vcc4behu";   // 私钥
String reqLocation = "Qingdao";            // 城市
String reqUnit = "c";                      // 摄氏/华氏
