// Two motors with Hall C1/C2 + Motor Outputs
// ESP32 Arduino

// ---------- Hall Inputs ----------
const int M1_C1 = 34;
const int M1_C2 = 35;
const int M2_C1 = 32;
const int M2_C2 = 33;

// ---------- Motor Outputs ----------
/*
 * "In1" --> S298N Side motor(M2) M2
 * "In2" --> S298N Side motor (M2) M1
 * "In3" --> UNO Side motor (M1) M1
 * "In4" --> UNO Side motor (M1) M2
 * 
 */
const int M1_OUT1 = 25;//"In4"
const int M1_OUT2 = 26;//"In3"
const int M2_OUT1 = 27;//"In2"
const int M2_OUT2 = 14;//"In1"

volatile long m1Count = 0;
volatile long m2Count = 0;

// ---------- Hall Interrupts ----------
void IRAM_ATTR m1ISR() {
  int c2 = digitalRead(M1_C2);
  if (c2) m1Count++;
  else    m1Count--;
}

void IRAM_ATTR m2ISR() {
  int c2 = digitalRead(M2_C2);
  if (c2) m2Count++;
  else    m2Count--;
}

// ---------- Motor Control ----------
void motor1Forward() {
  digitalWrite(M1_OUT1, HIGH);
  digitalWrite(M1_OUT2, LOW);
}

void motor1Reverse() {
  digitalWrite(M1_OUT1, LOW);
  digitalWrite(M1_OUT2, HIGH);
}

void motor1Brake() {
  digitalWrite(M1_OUT1, HIGH);
  digitalWrite(M1_OUT2, HIGH);
}

void motor1Coast() {
  digitalWrite(M1_OUT1, LOW);
  digitalWrite(M1_OUT2, LOW);
}

void motor2Forward() {
  digitalWrite(M2_OUT1, HIGH);
  digitalWrite(M2_OUT2, LOW);
}

void motor2Reverse() {
  digitalWrite(M2_OUT1, LOW);
  digitalWrite(M2_OUT2, HIGH);
}

void motor2Brake() {
  digitalWrite(M2_OUT1, HIGH);
  digitalWrite(M2_OUT2, HIGH);
}

void motor2Coast() {
  digitalWrite(M2_OUT1, LOW);
  digitalWrite(M2_OUT2, LOW);
}

void setup() {
  Serial.begin(115200);

  // Hall inputs
  pinMode(M1_C1, INPUT);
  pinMode(M1_C2, INPUT);
  pinMode(M2_C1, INPUT);
  pinMode(M2_C2, INPUT);

  attachInterrupt(digitalPinToInterrupt(M1_C1), m1ISR, RISING);
  attachInterrupt(digitalPinToInterrupt(M2_C1), m2ISR, RISING);

  // Motor outputs
  pinMode(M1_OUT1, OUTPUT);
  pinMode(M1_OUT2, OUTPUT);
  pinMode(M2_OUT1, OUTPUT);
  pinMode(M2_OUT2, OUTPUT);

  motor1Coast();
  motor2Coast();
}

void loop() {
  static unsigned long last = 0;
  unsigned long now = millis();

  // Example: alternate motor directions every 3 seconds
  if ((now / 3000) % 2 == 0) {
    motor1Forward();
    motor2Reverse();
  } else {
    motor1Reverse();
    motor2Forward();
  }

  // Print hall counts every second
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
