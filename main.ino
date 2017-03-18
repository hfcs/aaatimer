#include "Arduino.h"
#include "board.h"
#include "timer_event.h"
#include "button.h"
#include "stop_plate.h"
#include "buzzer.h"
#include "display.h"
#include "state.h"
#include "stopwatch.h"

void setup() {
  setupState();
  Serial.begin(115200);
  delay(100);
  TimerEvent::getInstance()->setupTimerEvent();
  setupButton();
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
  loopButton();
  loopStopPlate();
  TimerEvent::getInstance()->processEvent();
  loopState();
  loopBuzzer();
  delay(5);
}
