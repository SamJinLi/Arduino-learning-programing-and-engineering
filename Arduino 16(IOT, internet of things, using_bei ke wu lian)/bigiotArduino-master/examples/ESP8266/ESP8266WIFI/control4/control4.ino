// remember: when changing the pin number, reference to the ESP8266's pin references or whatever board you're using. Look for the GPIO pin instead of the "D" pins!!!
/*
    此文件需安装Arduino esp8266开发环境支持，环境搭建参见：http://www.bigiot.net/talk/237.html
    本程序可以用来控制四路继电器
    ESP8266烧入此程序直接，使用高低电频控制光耦继电器来控制电灯
    我的继电器默认高电频关闭，所以在初始化时都初始化为高电频，play关闭开启，stop关闭关闭，输入1-4打开或关闭对应的引脚
    代码基于https://github.com/bigiot/bigiotArduino/blob/master/examples/ESP8266/kaiguan/kaiguan.ino
    上的代码进行调整，修复了部分bug，解决了断线重连问题，此代码可以直接烧入到nodemcu模块，分享代码希望对大家有帮助
*/

#include <ESP8266WiFi.h>
#include <aJSON.h>
#include <WiFiClientSecure.h>
//=============  此处必须修该============
String DEVICEID = "31288";   // 你的设备编号   ==
String APIKEY = "6ce273877"; // 设备密码==
//=======================================
unsigned long lastCheckInTime = 0;           // 记录上次报到时间
const unsigned long postingInterval = 40000; // 每隔40秒向服务器报到一次

const char *ssid = "TP-Link_5340";   // 无线名称
const char *password = "12709656"; // 无线密码

const char *host = "www.bigiot.net";
const int httpPort = 8181;

int LED = 15, Switch = 14, lit = 0;

void setup()
{
  Serial.begin(115200);
  delay(1000);
  WiFi.begin(ssid, password);
  // 默认输出关闭电频
  pinMode(LED, OUTPUT);
  pinMode(Switch, INPUT);
}

WiFiClient client;
void loop()
{
  //   int n = digitalRead(Switch); //创建变量n，并采集switch数字口的状态，赋值给n
  // // Serial.println(n);
  // if (n == 0) //如果n为高电平，执行下面的语句，否则跳过
  // {
  // delay(500); //消除抖动
  // digitalWrite(LED, !digitalRead(LED));
  // delay(2000); //延时2秒关闭
  // // digitalWrite(LED, LOW);
  // }


  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.print(".");
  }

  // Use WiFiClient class to create TCP connections
  if (!client.connected())
  {
    if (!client.connect(host, httpPort))
    {
      Serial.println("connection failed");
      delay(5000);
      return;
    }
  }

  if (millis() - lastCheckInTime > postingInterval || lastCheckInTime == 0)
  {
    checkIn();
  }

  // Read all the lines of the reply from server and print them to Serial
  if (client.available())
  {
    String inputString = client.readStringUntil('\n');
    inputString.trim();
    Serial.println(inputString);
    int len = inputString.length() + 1;
    if (inputString.startsWith("{") && inputString.endsWith("}"))
    {
      char jsonString[len];
      inputString.toCharArray(jsonString, len);
      aJsonObject *msg = aJson.parse(jsonString);
      processMessage(msg);
      aJson.deleteItem(msg);
    }
  }
}

void processMessage(aJsonObject *msg)
{
  aJsonObject *method = aJson.getObjectItem(msg, "M");
  aJsonObject *content = aJson.getObjectItem(msg, "C");
  aJsonObject *client_id = aJson.getObjectItem(msg, "ID");
  if (!method)
  {
    return;
  }
  String M = method->valuestring;
  if (M == "say")
  {
    String C = content->valuestring;
    String F_C_ID = client_id->valuestring;
    /*if(C == "plus"){
      if (r < 256){
        r = r++;
        }
        else {
          r = 255;
          }
      analogWrite(D2, r);
      }
      else if(C == "minus"){
      if (r > -1){
      r = r-1;
      }else{
        r = 0;
        }
      digitalWrite(D2, 0);
      }
      int g = 0;
      if(C == "up"){
      if (g < 256){
      g = g+1;
      } else{
        g = 255;
        }
      analogWrite(D5, g);
      }
      else if(C == "down"){
        if (g > -1){
        g = g-1;
        } else {
          g = 0;
          }
        analogWrite(D5, g);
      }
      int b = 0;
      if(C == "left"){
      if (b < 256){
      b = b++;
      } else {
        b = 255;
        }
      analogWrite(D6,b);
      }
      else if(C == "right"){
        if (b > -1){
          b = b--;
          } else {
            b = 0;
            }
      analogWrite(D6, b);
      }
    */

    if (C == "pause")
    {
      String stats = digitalRead(LED) ? "off" : "on";
      // Serial.println(digitalRead(LED) ? "on" : "off"); 
      sayToClient(F_C_ID, "Stats: " + stats);
    }

    if (C == "play")
    {
      digitalWrite(LED, 0);
//      lit = 1;
      sayToClient(F_C_ID, "Fish lamp on!");
      Serial.println("on");
    }
    else if (C == "stop")
    {
//      lit = 0;
      digitalWrite(LED, 1);
      sayToClient(F_C_ID, "Fish lamp off!");
      Serial.println("off");
    }
  }
}
void checkIn()
{
  String msg = "{\"M\":\"checkin\",\"ID\":\"" + DEVICEID + "\",\"K\":\"" + APIKEY + "\"}\n";
  client.print(msg);
  lastCheckInTime = millis();
}

void sayToClient(String client_id, String content)
{
  String msg = "{\"M\":\"say\",\"ID\":\"" + client_id + "\",\"C\":\"" + content + "\"}\n";
  client.print(msg);
  lastCheckInTime = millis();
}
