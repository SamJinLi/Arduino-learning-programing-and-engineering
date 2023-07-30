void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(8, OUTPUT);

}
void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(2) == 0) {
    if (digitalRead(3) == 0) {
      digitalWrite(8, 1);
    }
    else
    digitalWrite(8,0);
  }
  else {
    digitalWrite(8, 0);
  }
}
