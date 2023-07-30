void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int t = 10; t>0 ; t--) {
    Serial.println(t);
    delay(1000);
  }
}
