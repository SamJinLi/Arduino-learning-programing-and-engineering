#include <ESP32Encoder.h>
#include <Ticker.h>

ESP32Encoder encoder;
ESP32Encoder encoder2;

Ticker encoderUpdater;
// timer and flag for example, not needed for encoders
unsigned long encoder2lastToggled;
bool encoder2Paused = false;

float encoder1val, encoder2val;

void setup(){
	
	Serial.begin(115200);
	// Enable the weak pull down resistors

	//ESP32Encoder::useInternalWeakPullResistors=DOWN;
	// Enable the weak pull up resistors
	ESP32Encoder::useInternalWeakPullResistors=UP;

	// use pin 19 and 18 for the first encoder
	encoder.attachHalfQuad(36, 39);
	// use pin 17 and 16 for the second encoder
	encoder2.attachHalfQuad(34, 35);
		
	// set starting count value after attaching
	encoder.setCount(0);

	// clear the encoder's raw count and set the tracked count to zero
	encoder2.clearCount();
	Serial.println("Encoder Start = " + String((int32_t)encoder.getCount()));
	// set the lastToggle
	encoder2lastToggled = millis();
 
 encoderUpdater.attach_ms(100, updateEncoder);
}

void loop(){
	// Loop and read the count
//	Serial.println("Encoder count = " + String((int32_t)encoder.getCount()) + " " + String((int32_t)encoder2.getCount()));
	Serial.println(String(encoder1val) + String(encoder2val));
	delay(100);

	// every 5 seconds toggle encoder 2
//	if (millis() - encoder2lastToggled >= 5000) {
//		if(encoder2Paused) {
//			Serial.println("Resuming Encoder 2");
//			encoder2.resumeCount();
//		} else {
//			Serial.println("Paused Encoder 2");
//			encoder2.pauseCount();
//		}
//
//		encoder2Paused = !encoder2Paused;
//		encoder2lastToggled = millis();
//	}
}

void updateEncoder (){
  encoder1val = encoder.getCount();
  encoder2val = encoder2.getCount();

  encoder.setCount(0);
  encoder2.setCount(0);
  }
