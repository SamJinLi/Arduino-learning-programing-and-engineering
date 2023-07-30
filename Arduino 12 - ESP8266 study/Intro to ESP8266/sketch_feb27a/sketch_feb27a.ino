void setup() {
  // put your setup code here, to run once:
  pinMode(D0,OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  digitalWrite(D0,1);
  delay(500);
  digitalWrite(D0,0);
  delay(500);
}
