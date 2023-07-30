//   
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//  文 件 名   : oled.c
//  作    者   : oldzhai
//  生成日期   : 2021-1-18
//  功能描述   : ESP8266 OLED显示屏I2C驱动显示
//              说明: 
//              ----------------------------------------------------------------
//              GND  电源地
//              VCC  3.3v电源
//              D0   GPIO16（SCL）
//              D1   GPIO5 （SDA）  
//              ----------------------------------------------------------------
//******************************************************************************/
#include "oledfont.h" //字库
#include "GetData.h" //文件库

//I2C管脚的定义
#define SCL D0
#define SDA D1

//输出电平的宏定义
#define I2C_SCL_L digitalWrite(SCL,LOW)//SCL
#define I2C_SCL_H digitalWrite(SCL,HIGH)

#define I2C_SDA_L digitalWrite(SDA,LOW)//SDA
#define I2C_SDA_H digitalWrite(SDA,HIGH)

//管脚模式的宏定义
#define SCL_IN   pinMode(SCL, INPUT)   
#define SCL_OUT  pinMode(SCL, OUTPUT) //SCL
#define SDA_IN   pinMode(SDA, INPUT) 
#define SDA_OUT  pinMode(SDA, OUTPUT) //SDA

#define OLED_CMD  0  //写命令
#define OLED_DATA 1 //写数据

void setup(){
  //Wifi setup
Serial.begin(9600);
  Serial.println("");

  // 连接WiFi
  WiFi.begin(ssid, password);                  // 启动网络连接
  Serial.print("Connecting to ");              // 串口监视器输出网络连接信息
  Serial.print(ssid); Serial.println(" ...");  // 告知用户NodeMCU正在尝试WiFi连接

  int i = 0;                                   // 这一段程序语句用于检查WiFi是否连接成功
  while (WiFi.status() != WL_CONNECTED) {      // WiFi.status()函数的返回值是由NodeMCU的WiFi连接状态所决定的。
    delay(1000);                               // 如果WiFi连接成功则返回值为WL_CONNECTED
    Serial.print(i++); Serial.print(' ');      // 此处通过While循环让NodeMCU每隔一秒钟检查一次WiFi.status()函数返回值
  }                                            // 同时NodeMCU将通过串口监视器输出连接时长读秒。
  // 这个读秒是通过变量i每隔一秒自加1来实现的。
  Serial.println("");                          // WiFi连接成功后
  Serial.println("Connection established!");   // NodeMCU将通过串口监视器输出"连接成功"信息。
  Serial.print("IP address:    ");             // 同时还将输出NodeMCU的IP地址。这一功能是通过调用
  Serial.println(WiFi.localIP());              // WiFi.localIP()函数来实现的。该函数的返回值即NodeMCU的IP地址。
  //OLED setup
  OLED_Init();
  OLED_ColorTurn(0);//0正常显示 1反色显示
  OLED_DisplayTurn(0);//0正常显示 1翻转180度显示
}

void loop()
{
  //wifi loop
 // 建立心知天气API当前天气请求资源地址
  String reqRes = "/v3/weather/now.json?key=" + reqUserKey +
                  + "&location=" + reqLocation +
                  "&language=en&unit=" + reqUnit;

  // 向心知天气服务器服务器请求信息并对信息进行解析
  WiFiClient client;

  // 建立http请求信息
  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" +
                       "Host: " + host + "\r\n" +
                       "Connection: close\r\n\r\n";
  Serial.println("");
  Serial.print("Connecting to "); Serial.print(host);

  // 尝试连接服务器
  if (client.connect(host, 80)) {
    Serial.println(" Success!");

    // 向服务器发送http请求信息
    client.print(httpRequest);
    Serial.println("Sending request: ");
    Serial.println(httpRequest);

    // 获取并显示服务器响应状态行
    String status_response = client.readStringUntil('\n');
    Serial.print("status_response: ");
    Serial.println(status_response);

    // 使用find跳过HTTP响应头
    if (client.find("\r\n\r\n")) {
      Serial.println("Found Header End. Start Parsing.");
    }

    // 利用ArduinoJson库解析心知天气响应信息
    parseInfo(client);
  } else {
    Serial.println(" connection failed!");
  }
  //断开客户端与服务器连接工作
  client.stop();
  delay(20000);
  //OLED loop
//wrong  OLED_ShowChar(0,0, char u8="A",8)
  //OLED_DrawBMP(0,0,128,64,BMP1); //显示图片
  delay(1000); //延时1秒
  OLED_Clear();//清除界面
  
  OLED_ShowString(0,0,"OLDZHAI",16);//x坐标，y坐标，字体或数据，字符大小
  OLED_ShowString(20,4,"2021/1/18",16);
  OLED_ShowString(0,6,"ASCII:",16);  
  OLED_ShowString(63,6,"CODE:",16);
  delay(1000); //延时1秒 delay函数的单位为ms
  OLED_Clear();
}

// Wifi 解析 (利用ArduinoJson库解析心知天气响应信息)
void parseInfo(WiFiClient client) {
  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2 * JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(6) + 230;
  DynamicJsonDocument doc(capacity);

  deserializeJson(doc, client);

  JsonObject results_0 = doc["results"][0];

  JsonObject results_0_now = results_0["now"];
  const char* results_0_now_text = results_0_now["text"]; // "Sunny"
  const char* results_0_now_code = results_0_now["code"]; // "0"
  const char* results_0_now_temperature = results_0_now["temperature"]; // "32"

  JsonObject results_0_location = results_0["location"];//go down to location file
  const char* results_0_location_name = results_0_location["name"];//go down to name file

  const char* results_0_last_update = results_0["last_update"]; // "2020-06-02T14:40:00+08:00"

  // 通过串口监视器显示以上信息
  String results_0_now_text_str = results_0_now["text"].as<String>();
  int results_0_now_code_int = results_0_now["code"].as<int>();
  int results_0_now_temperature_int = results_0_now["temperature"].as<int>();

  String results_0_location_name_str = results_0_location["name"].as<String>();

  String results_0_last_update_str = results_0["last_update"].as<String>();

  Serial.println(F("======Weahter Now======="));
  Serial.print(F("Weather Now: "));
  Serial.print(results_0_now_text_str);
  Serial.print(F(" "));
  Serial.println(results_0_now_code_int);
  Serial.print(F("Temperature: "));
  Serial.println(results_0_now_temperature_int);
  Serial.print(F("City: "));
  Serial.println(results_0_location_name_str);
  Serial.print(F("Last Update: "));
  Serial.println(results_0_last_update_str);
  Serial.println(F("========================"));
}

//I2C初始化
void I2C_Init()
{
  SDA_OUT;
  SCL_OUT;
  I2C_SDA_H;
  I2C_SCL_H;
}

//I2C开始
void I2C_Start()
{
  I2C_SCL_H;
  I2C_SDA_H;
  I2C_SDA_L;
  I2C_SCL_L;
}

//I2C结束
void I2C_Stop()
{
  I2C_SCL_H;
  I2C_SDA_L;
  I2C_SDA_H;
}

//检查应答信号
void I2C_Wait_Ack()
{
    I2C_SDA_H;
    I2C_SCL_H;
    I2C_SCL_L;
}

//I2C写入一个字节
void Write_I2C_Byte(unsigned char dat)
{
  unsigned char i;
  
  for(i=0;i<8;i++)    
  {
    I2C_SCL_L;
    if( dat & 0x80 )
    {
      I2C_SDA_H;  
    }
    else
    {
      I2C_SDA_L;
    }
    dat <<= 1;
    I2C_SCL_H;
   }
   I2C_SCL_L;
}

//发送一个字节
//向SSD1306写入一个字节。
//mode:数据/命令标志 0,表示命令;1,表示数据;
void OLED_WR_Byte(u8 dat,u8 mode)
{
  I2C_Start();
  Write_I2C_Byte(0x78);
  I2C_Wait_Ack();
  if(mode){
    Write_I2C_Byte(0x40);
  }
  else{
    Write_I2C_Byte(0x00);
  }
  I2C_Wait_Ack();
  Write_I2C_Byte(dat);
  I2C_Wait_Ack();
  I2C_Stop();
}

//反显函数
void OLED_ColorTurn(u8 i)
{
  if(!i) OLED_WR_Byte(0xA6,OLED_CMD);//正常显示
  else  OLED_WR_Byte(0xA7,OLED_CMD);//反色显示
}

//屏幕旋转180度
void OLED_DisplayTurn(u8 i)
{
  if(i==0)
  {
      OLED_WR_Byte(0xC8,OLED_CMD);//正常显示
      OLED_WR_Byte(0xA1,OLED_CMD);
  }
  else
  {
    OLED_WR_Byte(0xC0,OLED_CMD);//反转显示
    OLED_WR_Byte(0xA0,OLED_CMD);
  }
}

//坐标设置
void OLED_Set_Pos(u8 x, u8 y) 
{ 
  OLED_WR_Byte(0xb0+y,OLED_CMD);
  OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
  OLED_WR_Byte((x&0x0f),OLED_CMD);
}
       
//开启OLED显示    
void OLED_Display_On(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
  OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
  OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}

//关闭OLED显示     
void OLED_Display_Off(void)
{
  OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC命令
  OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
  OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}
            
//清屏函数,清完屏,整个屏幕是黑色的!和没点亮一样!!!   
void OLED_Clear(void)  
{  
  u8 i,n;       
  for(i=0;i<8;i++)  
  {  
    OLED_WR_Byte (0xb0+i,OLED_CMD);    //设置页地址（0~7）
    OLED_WR_Byte (0x00,OLED_CMD);      //设置显示位置—列低地址
    OLED_WR_Byte (0x10,OLED_CMD);      //设置显示位置—列高地址   
    for(n=0;n<128;n++)OLED_WR_Byte(0,OLED_DATA); 
  } //更新显示
}

//在指定位置显示一个字符
//x:0~127
//y:0~63         
//sizey:选择字体 6x8  8x16
void OLED_ShowChar(u8 x,u8 y,const u8 chr,u8 sizey)
{       
  u8 c=0,sizex=sizey/2,temp;
  u16 i=0,size1;
  if(sizey==8)size1=6;
  else size1=(sizey/8+((sizey%8)?1:0))*(sizey/2);
  c=chr-' ';//得到偏移后的值
  OLED_Set_Pos(x,y);
  for(i=0;i<size1;i++)
  {
    if(i%sizex==0&&sizey!=8) OLED_Set_Pos(x,y++);
    if(sizey==8)
    {
      temp=pgm_read_byte(&asc2_0806[c][i]);
      OLED_WR_Byte(temp,OLED_DATA);//6X8字号
    }
    else if(sizey==16) 
    {
      temp=pgm_read_byte(&asc2_1608[c][i]);
      OLED_WR_Byte(temp,OLED_DATA);//8x16字号
    }
    else return;
  }
}

//m^n函数
u32 oled_pow(u8 m,u8 n)
{
  u32 result=1;  
  while(n--)result*=m;    
  return result;
}
         
//显示数字
//x,y :起点坐标
//num:要显示的数字
//len :数字的位数
//sizey:字体大小      
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 sizey)
{           
  u8 t,temp,m=0;
  u8 enshow=0;
  if(sizey==8)m=2;
  for(t=0;t<len;t++)
  {
    temp=(num/oled_pow(10,len-t-1))%10;
    if(enshow==0&&t<(len-1))
    {
      if(temp==0)
      {
        OLED_ShowChar(x+(sizey/2+m)*t,y,' ',sizey);
        continue;
      }else enshow=1;
    }
    OLED_ShowChar(x+(sizey/2+m)*t,y,temp+'0',sizey);
  }
}

//显示一个字符号串
void OLED_ShowString(u8 x,u8 y,const char *chr,u8 sizey)
{
  u8 j=0;
  while (chr[j]!='\0')
  {   
    OLED_ShowChar(x,y,chr[j++],sizey);
    if(sizey==8)x+=6;
    else x+=sizey/2;
  }
}

//显示汉字
void OLED_ShowChinese(u8 x,u8 y,const u8 no,u8 sizey)
{
  u16 i,size1=(sizey/8+((sizey%8)?1:0))*sizey;
  u8 temp;
  for(i=0;i<size1;i++)
  {
    if(i%sizey==0) OLED_Set_Pos(x,y++);
    if(sizey==16) 
    {
      temp=pgm_read_byte(&Hzk[no][i]);
      OLED_WR_Byte(temp,OLED_DATA);//16x16字号
    }
    //else if(sizey==xx) OLED_WR_Byte(xxx[c][i],OLED_DATA);//用户添加字号
    else return;
  }       
}


//显示图片
//x,y显示坐标
//sizex,sizey,图片长宽
//BMP：要显示的图片
void OLED_DrawBMP(u8 x,u8 y,u8 sizex, u8 sizey,const u8 BMP[])
{   
  u16 j=0;
  u8 i,m,temp;
  sizey=sizey/8+((sizey%8)?1:0);
  for(i=0;i<sizey;i++)
  {
    OLED_Set_Pos(x,i+y);
    for(m=0;m<sizex;m++)
    {
       temp=pgm_read_byte(&BMP[j++]);
       OLED_WR_Byte(temp,OLED_DATA);
    }
  }
} 

//OLED的初始化
void OLED_Init(void)
{ 
  I2C_Init(); //初始化I2C 管脚输出高电平
  
  OLED_WR_Byte(0xAE,OLED_CMD);//--turn off oled panel
  OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
  OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
  OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  Set Mapping RAM Display Start Line (0x00~0x3F)
  OLED_WR_Byte(0x81,OLED_CMD);//--set contrast control register
  OLED_WR_Byte(0xCF,OLED_CMD); // Set SEG Output Current Brightness
  OLED_WR_Byte(0xA1,OLED_CMD);//--Set SEG/Column Mapping     0xa0左右反置 0xa1正常
  OLED_WR_Byte(0xC8,OLED_CMD);//Set COM/Row Scan Direction   0xc0上下反置 0xc8正常
  OLED_WR_Byte(0xA6,OLED_CMD);//--set normal display
  OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
  OLED_WR_Byte(0x3f,OLED_CMD);//--1/64 duty
  OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset Shift Mapping RAM Counter (0x00~0x3F)
  OLED_WR_Byte(0x00,OLED_CMD);//-not offset
  OLED_WR_Byte(0xd5,OLED_CMD);//--set display clock divide ratio/oscillator frequency
  OLED_WR_Byte(0x80,OLED_CMD);//--set divide ratio, Set Clock as 100 Frames/Sec
  OLED_WR_Byte(0xD9,OLED_CMD);//--set pre-charge period
  OLED_WR_Byte(0xF1,OLED_CMD);//Set Pre-Charge as 15 Clocks & Discharge as 1 Clock
  OLED_WR_Byte(0xDA,OLED_CMD);//--set com pins hardware configuration
  OLED_WR_Byte(0x12,OLED_CMD);
  OLED_WR_Byte(0xDB,OLED_CMD);//--set vcomh
  OLED_WR_Byte(0x40,OLED_CMD);//Set VCOM Deselect Level
  OLED_WR_Byte(0x20,OLED_CMD);//-Set Page Addressing Mode (0x00/0x01/0x02)
  OLED_WR_Byte(0x02,OLED_CMD);//
  OLED_WR_Byte(0x8D,OLED_CMD);//--set Charge Pump enable/disable
  OLED_WR_Byte(0x14,OLED_CMD);//--set(0x10) disable
  OLED_WR_Byte(0xA4,OLED_CMD);// Disable Entire Display On (0xa4/0xa5)
  OLED_WR_Byte(0xA6,OLED_CMD);// Disable Inverse Display On (0xa6/a7) 
  OLED_Clear();
  OLED_WR_Byte(0xAF,OLED_CMD); /*display ON*/ 
}
