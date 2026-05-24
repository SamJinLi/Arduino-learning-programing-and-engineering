// Two motors with Hall C1/C2 + Motor Outputs
// ESP32 Arduino
//L298N IS LEFT SIDE

// ---------- Hall Inputs ----------
const int RIGHT_C1 = 34;
const int RIGHT_C2 = 35;
const int LEFT_C1 = 32;
const int LEFT_C2 = 33;

// ---------- Motor Outputs ----------

const int RIGHT_PWM = 12; //"ENB"
const int RIGHT_M1 = 25;//"In4"
const int RIGHT_M2 = 26;//"In3"
const int LEFT_PWM = 13; //"ENA"
const int LEFT_M2 = 27;//"In2"
const int LEFT_M1 = 14;//"In1"

volatile long rightCount = 0;
volatile long leftCount = 0;

// ---------- Hall Interrupts ----------
void IRAM_ATTR rightISR() {
  int c2 = digitalRead(RIGHT_C2);
  if (c2) rightCount++;
  else    rightCount--;
}

void IRAM_ATTR leftISR() {
  int c2 = digitalRead(LEFT_C2);
  if (c2) leftCount++;
  else    leftCount--;
}

// PWM properties
const int freq = 1000;        // PWM frequency
const int pwmChannelA = 0;    // PWM channel for right motor
const int pwmChannelB = 1;    // PWM channel for left motor
const int resolution = 8;     // 8-bit resolution (0-255)

// ---------- Motor Control ----------
void setRightMotor(int PWMOut, bool in1High, bool in2High) {
  ledcWrite(RIGHT_PWM, PWMOut);
  digitalWrite(RIGHT_M1, in1High ? HIGH : LOW);
  digitalWrite(RIGHT_M2, in2High ? HIGH : LOW);
}
void rightForward() {
  setRightMotor(255, true, false);
}

void rightReverse() {
  setRightMotor(255,false, true);
}

void rightBrake() {
  setRightMotor(255,true,true);
}

void rightCoast() {
  setRightMotor(0,false,false);
}

void setLeftMotor(int PWMOut, bool in1High, bool in2High) {
  ledcWrite(LEFT_PWM, PWMOut);
  digitalWrite(LEFT_M1, in1High ? HIGH : LOW);
  digitalWrite(LEFT_M2, in2High ? HIGH : LOW);
}
void leftForward() {
  setLeftMotor(255, false, true);
}

void leftReverse() {
  setLeftMotor(255,true, false);
}

void leftBrake() {
  setLeftMotor(255,true,true);
}

void leftCoast() {
  setLeftMotor(0,false,false);
}

void setup() {
  Serial.begin(115200);

  // Hall inputs
  pinMode(RIGHT_C1, INPUT);
  pinMode(RIGHT_C2, INPUT);
  pinMode(LEFT_C1, INPUT);
  pinMode(LEFT_C2, INPUT);

  attachInterrupt(digitalPinToInterrupt(RIGHT_C1), rightISR, RISING);
  attachInterrupt(digitalPinToInterrupt(LEFT_C1), leftISR, RISING);

  // Motor outputs
  pinMode(RIGHT_PWM, OUTPUT);
  pinMode(RIGHT_M1, OUTPUT);
  pinMode(RIGHT_M2, OUTPUT);
  pinMode(LEFT_PWM, OUTPUT);
  pinMode(LEFT_M2, OUTPUT);
  pinMode(LEFT_M1, OUTPUT);

  ledcAttachChannel(RIGHT_PWM, freq, resolution, pwmChannelA);
  ledcAttachChannel(LEFT_PWM, freq, resolution, pwmChannelB);

  rightCoast();
  leftCoast();
}

void loop() {
  static unsigned long last = 0;
  unsigned long now = millis();

    setRightMotor(50,true,false);
    setLeftMotor(50,false,true);
  // Print hall counts every second
  if (now - last >= 250) {
    last = now;

    long right = rightCount;
    long left = leftCount;

    rightCount = 0;
    leftCount = 0;

    Serial.print(" right-pulses: ");
    Serial.print(right);

    Serial.print(" left-pulses: ");
    Serial.println(left);
  }
}
