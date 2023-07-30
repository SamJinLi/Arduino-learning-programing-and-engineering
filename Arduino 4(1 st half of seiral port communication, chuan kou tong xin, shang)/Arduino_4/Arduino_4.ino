void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(2,INPUT);
  pinMode(4,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(2)==0){
    Serial.println(1);
    digitalWrite(4,1);
    }
  else {
    Serial.println(0);
    digitalWrite(4,0);    
    }
  }
