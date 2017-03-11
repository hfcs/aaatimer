#include "Arduino.h"
#include "board.h"
#include "timer_event.h"
#include "button.h"
#include "led.h"
#include "stop_plate.h"
#include "buzzer.h"
#include "display.h"
#include "state.h"
#include "stopwatch.h"

const int KNOCK_ADC_THRESHOLD = 500;

void setup() {
  setupState();
  Serial.begin(115200);
  delay(100);
  TimerEvent::getInstance()->setupTimerEvent();
  setupButton();
  setupLed();
  setupStopPlate();
  setupStopwatch();
  setupBuzzer();
  setupDisplay();
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
  handleButton();
  handleLed();
  handleStopPlate();
  handleBuzzer();
  TimerEvent::getInstance()->processEvent();
  updateStopwatchDisplay();
  delay(5);
}
