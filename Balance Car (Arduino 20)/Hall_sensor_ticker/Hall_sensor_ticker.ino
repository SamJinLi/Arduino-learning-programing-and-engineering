// Two motors, each with two Hall channels (C1, C2)

const int M1_C1 = 34;//UNO side motor C2
const int M1_C2 = 35;//UNO side motor C1
const int M2_C1 = 32;//S398N side motor C1
const int M2_C2 = 33;

volatile long m1Count = 0;
volatile long m2Count = 0;

void IRAM_ATTR m1ISR() {
  int c2 = digitalRead(M1_C2);
  if (c2 == HIGH) m1Count++;   // direction A
  else m1Count--;              // direction B
}

void IRAM_ATTR m2ISR() {
  int c2 = digitalRead(M2_C2);
  if (c2 == HIGH) m2Count++;   // direction A
  else m2Count--;              // direction B
}

void setup() {
  Serial.begin(115200);

  pinMode(M1_C1, INPUT);
  pinMode(M1_C2, INPUT);
  pinMode(M2_C1, INPUT);
  pinMode(M2_C2, INPUT);

  attachInterrupt(digitalPinToInterrupt(M1_C1), m1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(M2_C1), m2ISR, RISING);
}

void loop() {
  static unsigned long last = 0;
  unsigned long now = millis();

  if (now - last >= 1000) {
    last = now;

    long m1 = m1Count;
    long m2 = m2Count;

    m1Count = 0;
    m2Count = 0;

    Serial.print("Motor 1 pulses: ");
    Serial.println(m1);

    Serial.print("Motor 2 pulses: ");
    Serial.println(m2);
  }
}
