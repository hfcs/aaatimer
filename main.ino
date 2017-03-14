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
  Serial.println("");
  if (TimerEvent::getInstance()->isListenerListFull()) {
    Serial.println("too many listeners, please resize the library");
  }
}

void loop() {
  handleButton();
  handleLed();
  handleStopPlate();
  handleBuzzer();
  TimerEvent::getInstance()->processEvent();
  updateStopwatchDisplay();
  delay(5);
}
