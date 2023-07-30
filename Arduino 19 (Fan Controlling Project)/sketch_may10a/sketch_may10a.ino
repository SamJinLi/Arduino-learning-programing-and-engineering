//This is a document for STEM Fair 2023, Created by Lucas Te and Samuel Li
/*This code enabled people to use a fan on Arduino Uno, port 11 controls the speed, oprt A4 is the variable resistence that could change the speed of the fan
*/

const int PWM = 11;
const int Interval = 100;
float frequency = 9999;
int Speed;
int sreader = 13;

void setup() {
  // put your setup code here, to run once:

  pinMode(PWM, OUTPUT);
  pinMode(A5, INPUT);
  pinMode(sreader,INPUT);
  Serial.begin(9600);
}
void loop() {
  // put your main code here, to run repeatedly:
    String date;
      if (Serial.available() > 0 )
    {
      while (Serial.available() > 0)
      {
        date += char(Serial.read()); // a+=1   a=a+1
        delay(2);   //等待缓存数据
      }
      frequency = date.toInt();
      Serial.println(frequency);
    }
  
    Speed = map (analogRead(A4),0,1023,0,frequency);
    Serial.println(analogRead(sreader));
    digitalWrite(PWM,HIGH);
    delayMicroseconds(Speed);
    digitalWrite(PWM,LOW);
    delayMicroseconds(frequency-Speed);
    Serial.println(digitalRead(8));
  //Speed = map (analogRead(A4),0,1023,0,255);
  //analogWrite(PWM,Speed);
  //delay(5);
  //  Serial.println("Works fine");
}
