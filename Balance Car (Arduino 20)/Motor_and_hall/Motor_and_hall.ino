// Two motors with Hall C1/C2 + Motor Outputs
// ESP32 Arduino
//L298N IS LEFT SIDE

// ---------- Hall Inputs ----------
const int RIGHT_C1 = 34;
const int RIGHT_C2 = 35;
const int LEFT_C1 = 32;
const int LEFT_C2 = 33;

// Counter variables for each channel
volatile long rightCount = 0;
volatile long leftCount = 0;

// Store previous states for edge detection
volatile bool lastRightC1 = false;
volatile bool lastLeftC1 = false;

// Direction flags
volatile bool rightDirection = true;  // true = forward, false = reverse
volatile bool leftDirection = true;

int targetLeftPWM = 0;
int targetRightPWM = 0;
bool serialControlActive = false;

// ---------- Motor Outputs ----------

const int RIGHT_PWM = 12; //"ENB"
const int RIGHT_M1 = 25;//"In4"
const int RIGHT_M2 = 26;//"In3"
const int LEFT_PWM = 13; //"ENA"
const int LEFT_M2 = 27;//"In2"
const int LEFT_M1 = 14;//"In1"

// ---------- Hall Interrupts (Quadrature Decoding) ----------
void IRAM_ATTR rightISR() {
  bool c1 = digitalRead(RIGHT_C1);
  bool c2 = digitalRead(RIGHT_C2);
  
  // Determine direction and count
  if (c1 != lastRightC1) {
    if (c1 == c2) {
      // Forward direction
      rightCount--;
      rightDirection = false;
    } else {
      // Reverse direction
      rightCount++;
      rightDirection = true;
    }
    lastRightC1 = c1;
  }
}

void IRAM_ATTR leftISR() {
  bool c1 = digitalRead(LEFT_C1);
  bool c2 = digitalRead(LEFT_C2);
  
  // Determine direction and count
  if (c1 != lastLeftC1) {
    if (c1 == c2) {
      // Forward direction
      leftCount--;
      leftDirection = false;
    } else {
      // Reverse direction
      leftCount++;
      leftDirection = true;
    }
    lastLeftC1 = c1;
  }
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

void rightForward(int PWMOut = 255) {
  setRightMotor(constrain(PWMOut, 0, 255), true, false);
}

void rightReverse(int PWMOut = 255) {
  setRightMotor(constrain(PWMOut, 0, 255), false, true);
}

void rightBrake(int PWMOut = 255) {
  setRightMotor(constrain(PWMOut, 0, 255), true, true);
}

void rightCoast() {
  setRightMotor(0, false, false);
}

void setLeftMotor(int PWMOut, bool in1High, bool in2High) {
  ledcWrite(LEFT_PWM, PWMOut);
  digitalWrite(LEFT_M1, in1High ? HIGH : LOW);
  digitalWrite(LEFT_M2, in2High ? HIGH : LOW);
}

void leftForward(int PWMOut = 255) {
  setLeftMotor(constrain(PWMOut, 0, 255), false, true);
}

void leftReverse(int PWMOut = 255) {
  setLeftMotor(constrain(PWMOut, 0, 255), true, false);
}

void leftBrake(int PWMOut = 255) {
  setLeftMotor(constrain(PWMOut, 0, 255), true, true);
}

void leftCoast() {
  setLeftMotor(0, false, false);
}

void updateSpeedFromSerial() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    
    int spaceIndex = input.indexOf(' ');
    if (spaceIndex > 0) {
      // Parse two integers: left and right PWM values
      int leftVal = input.substring(0, spaceIndex).toInt();
      int rightVal = input.substring(spaceIndex + 1).toInt();
      
      // Constrain values to valid range (-255 to 255)
      targetLeftPWM = constrain(leftVal, -255, 255);
      targetRightPWM = constrain(rightVal, -255, 255);
      
      serialControlActive = true;
      
      // Apply motor settings based on sign
      // Left motor control
      if (targetLeftPWM > 0) {
        leftForward(targetLeftPWM);
        Serial.print("Left Forward: ");
        Serial.println(targetLeftPWM);
      } else if (targetLeftPWM < 0) {
        leftReverse(-targetLeftPWM);
        Serial.print("Left Reverse: ");
        Serial.println(-targetLeftPWM);
      } else {
        leftCoast();
        Serial.println("Left Stop");
      }
      
      // Right motor control
      if (targetRightPWM > 0) {
        rightForward(targetRightPWM);
        Serial.print("Right Forward: ");
        Serial.println(targetRightPWM);
      } else if (targetRightPWM < 0) {
        rightReverse(-targetRightPWM);
        Serial.print("Right Reverse: ");
        Serial.println(-targetRightPWM);
      } else {
        rightCoast();
        Serial.println("Right Stop");
      }
    } 
    else if (input == "stop" || input == "s") {
      // Emergency stop
      leftCoast();
      rightCoast();
      targetLeftPWM = 0;
      targetRightPWM = 0;
      serialControlActive = false;
      Serial.println("EMERGENCY STOP - Motors stopped");
    }
    else if (input == "help" || input == "h") {
      Serial.println("=== Motor Control Commands ===");
      Serial.println("Format: [left_PWM] [right_PWM]");
      Serial.println("Range: -255 to 255");
      Serial.println("Examples:");
      Serial.println("  150 200  - Left:150 FWD, Right:200 FWD");
      Serial.println("  -100 80  - Left:100 REV, Right:80 FWD");
      Serial.println("  0 0      - Stop both motors");
      Serial.println("  stop     - Emergency stop");
      Serial.println("  help     - Show this message");
      Serial.println("==============================");
    }
  }

  // If no serial command received, you can add default behavior here
  // For testing without serial, uncomment the lines below:
  // if (!serialControlActive) {
  //   rightForward(200);
  //   leftForward(200);
  // }
}

void setup() {
  Serial.begin(115200);

  // Hall inputs
  pinMode(RIGHT_C1, INPUT);
  pinMode(RIGHT_C2, INPUT);
  pinMode(LEFT_C1, INPUT);
  pinMode(LEFT_C2, INPUT);

  // Read initial states
  lastRightC1 = digitalRead(RIGHT_C1);
  lastLeftC1 = digitalRead(LEFT_C1);

  // Attach interrupts on BOTH channels for better accuracy
  attachInterrupt(digitalPinToInterrupt(RIGHT_C1), rightISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(RIGHT_C2), rightISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LEFT_C1), leftISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(LEFT_C2), leftISR, CHANGE);

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
  static unsigned long lastPrint = 0;
  static unsigned long lastRightCount = 0;
  static unsigned long lastLeftCount = 0;
  unsigned long now = millis();

  // ---------- Hall Sensor Monitoring (Always active) ----------
  // Print hall counts and speed every 250ms
  if (now - lastPrint >= 250) {
    // Disable interrupts while reading to prevent corruption
    noInterrupts();
    long right = rightCount;
    long left = leftCount;
    bool rightDir = rightDirection;
    bool leftDir = leftDirection;
    interrupts();

    // Calculate speed (pulses per second)
    float rightSpeed = (right - lastRightCount) * 4.0;  // pulses per second
    float leftSpeed = (left - lastLeftCount) * 4.0;
    
    // Store for next calculation
    lastRightCount = right;
    lastLeftCount = left;
    lastPrint = now;

    // Print results with current PWM values if serial control is active
    Serial.print("Left Motor PWM: ");
    Serial.print(targetLeftPWM);
    Serial.print(" | Left: ");
    Serial.print(left);
    Serial.print(" pulses ");
    Serial.print(leftDir ? "FWD" : "REV");
    Serial.print(" | Speed: ");
    Serial.print(leftSpeed);
    Serial.print(" pps");
    
    Serial.print(" | Right Motor PWM: ");
    Serial.print(targetRightPWM);
    Serial.print(" | Right: ");
    Serial.print(right);
    Serial.print(" pulses ");
    Serial.print(rightDir ? "FWD" : "REV");
    Serial.print(" | Speed: ");
    Serial.print(rightSpeed);
    Serial.println(" pps");
  }
  updateSpeedFromSerial();
}
