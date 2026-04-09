//PID constants
double kp = 2;
double ki = 5;
double kd = 1;
 
unsigned long currentTime, previousTime;
double elapsedTime;
double error;
double lastError;
double input, output, setPoint;
double cumError, rateError;

String inputString = "";
bool stringComplete = false;
 
void setup(){
        setPoint = 0;                          //set point at zero degrees

        Serial.begin(9600);

        inputString.reserve(200);
}    

void loop(){
        int analogPin = A0;
        input = analogRead(analogPin);                //read from rotary encoder connected to A0
        output = computePID(input);
        delay(100);
          Serial.print("output = ");
          Serial.println(output);
        // analogWrite(3, output);                //control the motor based on PID value
        if(stringComplete)
        {
          String temp = inputString;
          if(temp.substring(0,1).equals("p"))
          {
            kp = temp.substring(1).toDouble();
            // Serial.write("success");
          }
          if(temp.substring(0,1).equals("i"))
          {
            ki = temp.substring(1).toDouble();
            // break;
          }
          if(temp.substring(0,1).equals("d"))
          {
            kd = temp.substring(1).toDouble();
            // break;
          }
          inputString = "";
          stringComplete = false;
        }
        String placeHolder = "kp = ";
        // Serial.println(placeHolder + kp);
}
 
double computePID(double inp){     
        currentTime = millis();                //get current time
        elapsedTime = (double)(currentTime - previousTime);        //compute time elapsed from previous computation
        
        error = setPoint - inp;                                // determine error
        cumError += error * elapsedTime;                // compute integral     
        rateError = (error - lastError)/elapsedTime;   // compute derivative
 
        double out = kp*error + ki*cumError + kd*rateError;                //PID output               
 
        lastError = error;                                //remember current error
        previousTime = currentTime;                        //remember current time
 
        return out;                                        //have function return the PID output
}

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag so the main loop can
    // do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}
