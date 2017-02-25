#include "Arduino.h"

#define ADC_PIN A0


const int KNOCK_ADC_THRESHOLD = 500;
enum TIMER_STATE { // Timer state machine reflects stop plate for simplicity
  TIMER_RESET,
  TIMER_START,
  TIMER_TIME_START,
  TIMER_LED_1,
  TIMER_LED_2
} timer_state;

void setup() {
  Serial.begin(115200);
  delay(100);
  pinMode(ADC_PIN, INPUT);
}

bool pollKnock() {
  if ((unsigned int) analogRead(ADC_PIN) >= KNOCK_ADC_THRESHOLD) {
    //Serial.println("Knock");
    return true;
  }
  return false;
}

void loop() {
  pollKnock();
  delay(1000);
}
