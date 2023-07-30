
#include "BluetoothSerial.h"
#include <String.h>
#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
int Speed = 0, num = 0;


void setup() {
  Serial.begin(115200);
  SerialBT.begin("ESP32test"); //Bluetooth device name
  Serial.println("The device started, now you can pair it with bluetooth!");
}

void loop() {
  String date = "";
  String temp = "";

  if (SerialBT.available()) {   //

    while (SerialBT.available())
    {
      date += char (SerialBT.read());
      delay(2);

    }
    temp = date.substring(0, 1);
    Serial.println(date);
    if (temp == "F")
    {
      temp = date.substring(1, 3);
      Speed = temp.toInt();
      Serial.printf("Speed:%d\n", Speed);
    }
  }
  SerialBT.print(num);
  
  delay(20);
}
